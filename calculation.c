#include <stdio.h>
#include <string.h>
#include "calculation.h"

// this only takes prefix expression
static long do_get_result(Stack *ps)
{
    CalItemInfo sinfo;
    stack_pop(ps, (char*)&sinfo, sizeof(CalItemInfo));
    if (sinfo.etype == CALITEMTYPE_NUM)
        return sinfo.lnum;
    else
    {
        long num1;
        long num2;

        if (((CalItemInfo*)ps->ptop->pdata)->etype == CALITEMTYPE_NUM)
        {
            CalItemInfo sinfo2;
            stack_pop(ps, (char*)&sinfo2, sizeof(CalItemInfo));
            num1 = sinfo2.lnum;
        }
        else
            num1 = do_get_result(ps);

        if (((CalItemInfo*)ps->ptop->pdata)->etype == CALITEMTYPE_NUM)
        {
            CalItemInfo sinfo2;
            stack_pop(ps, (char*)&sinfo2, sizeof(CalItemInfo));
            num2 = sinfo2.lnum;
        }
        else
            num2 = do_get_result(ps);

        switch (sinfo.etype)
        {
            case CALITEMTYPE_PLUS:
                return num1 + num2;
            case CALITEMTYPE_MINUS:
                return num1 - num2;
            case CALITEMTYPE_MULTIPLY:
                return num1 * num2;
            case CALITEMTYPE_DIVIDE:
                return num1 / num2;
        }
    }
    return 0;
}

long calculate_result(Stack *ps, wchar_t *pret)
{
    long ret = 0;
    if (ps->icount >0)
    {
        Stack s1, s2, s3;
        stack_init(&s1);
        stack_init(&s2);
        stack_init(&s3);

        CalItemInfo sinfo;
        while (ps->icount > 0)
        {
            stack_pop(ps, (char*)&sinfo, sizeof(CalItemInfo));
            if (sinfo.etype == CALITEMTYPE_NUM)
            {
                stack_push(&s1, (char*)&sinfo, sizeof(CalItemInfo));
                stack_push(&s3, (char*)&sinfo, sizeof(CalItemInfo));
            }
            else if (sinfo.etype == CALITEMTYPE_PLUS || sinfo.etype == CALITEMTYPE_MINUS)
            {
                if (s2.icount > 0)
                {
                    CalItemInfo *psinfo = (CalItemInfo*)s2.ptop->pdata;
                    while (psinfo != NULL && (psinfo->etype == CALITEMTYPE_MULTIPLY || psinfo->etype == CALITEMTYPE_DIVIDE))
                    {
                        CalItemInfo sinfo2;
                        stack_pop(&s2, (char*)&sinfo2, sizeof(CalItemInfo));
                        stack_push(&s1, (char*)&sinfo2, sizeof(CalItemInfo));
                        stack_push(&s3, (char*)&sinfo2, sizeof(CalItemInfo));
                        psinfo = s2.icount > 0 ? (CalItemInfo*)s2.ptop->pdata : NULL;
                    }
                }
                stack_push(&s2, (char*)&sinfo, sizeof(CalItemInfo));
            }
            else if (sinfo.etype == CALITEMTYPE_MULTIPLY || sinfo.etype == CALITEMTYPE_DIVIDE)
            {
                stack_push(&s2, (char*)&sinfo, sizeof(CalItemInfo));
            }
        }

        while (s2.icount > 0)
        {
            stack_pop(&s2, (char*)&sinfo, sizeof(CalItemInfo));
            stack_push(&s1, (char*)&sinfo, sizeof(CalItemInfo));
            stack_push(&s3, (char*)&sinfo, sizeof(CalItemInfo));
        }

        while (s1.icount > 0)
        {
            stack_pop(&s1, (char*)&sinfo, sizeof(CalItemInfo));
            if (sinfo.etype == CALITEMTYPE_NUM)
                _snwprintf(pret + wcslen(pret), 128, L"%ld ", sinfo.lnum);
            else if (sinfo.etype == CALITEMTYPE_PLUS)
                wcscat(pret, L"+ ");
            else if (sinfo.etype == CALITEMTYPE_MINUS)
                wcscat(pret, L"- ");
            else if (sinfo.etype == CALITEMTYPE_MULTIPLY)
                wcscat(pret, L"* ");
            else if (sinfo.etype == CALITEMTYPE_DIVIDE)
                wcscat(pret, L"/ ");
        }

        ret = do_get_result(&s3);
    }
    return ret;
}

