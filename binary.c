#include <string.h>
#include "binary.h"
#include "mem_d.h"

#define _MALLOC malloc_d
#define _FREE free_d

#define BIN_LEN 256
#define CHAR_BITS 8
#define CHAR_NUM BIN_LEN/CHAR_BITS

typedef struct _bin32 _bin;

typedef struct _bin32
{
    unsigned char cBin[CHAR_NUM];
    _bin *pnext;
} _bin;

void bin_lshift(_bin*, int);
void bin_rshift(_bin*, int);
void _bin_add(_bin*, _bin*, _bin*);
void _bin_mul(_bin*, _bin*, _bin*);
void _bin_sub(_bin*, _bin*, _bin*);
void _bin_div(_bin*, _bin*, _bin*, _bin*);
void bin_printb(_bin*);
void bin_printx(_bin*);

static char _x2ch(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return ch;
}

static char _ch2x(char ch)
{
    if (ch >= 0 && ch <= 9)
        return ch + '0';
    else if (ch >= 10 && ch <= 16)
        return ch - 10 + 'a';
    return ch;
}

void bin_init(_bin *pbin)
{
    if (pbin != NULL)
    {
        for (int i = 0; i < CHAR_NUM; i++)
            pbin->cBin[i] = 0;
        pbin->pnext = NULL;
    }
}

_bin* _bin_create()
{
    _bin *pbin = _MALLOC(sizeof(_bin));
    bin_init(pbin);
    return pbin;
}

void _bin_extend_if_null(_bin **ppa, _bin **ppb)
{
    if (*ppa == NULL)
    {
        (*ppb)->pnext = _bin_create();
        *ppa = (*ppb)->pnext;
    }
}

_bin* _bin_create_x(const char *pstr, unsigned int isize)
{
    _bin *pa = _bin_create();
    _bin *pa_tmp = pa;
    _bin *pa_next = pa_tmp;
    int inext = isize;
    while (inext > 0)
    {
        _bin_extend_if_null(&pa_next, &pa_tmp);
        for (int i = CHAR_NUM-1; i >= 0 && inext >= 0; i--)
        {
            char ch1 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
            char ch2 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
            pa_next->cBin[i] = (ch2 << 4 | ch1);
        }
        pa_next = pa_next->pnext;
    }
    return pa;
}


Bin bin_create(const char *pstr)
{
    if (pstr != NULL)
    {
        int len = 0;
        for(; pstr[len] != '\0'; len++);
        return bin_create2(pstr, len);
    }
    return 0;
}

Bin bin_create2(const char *pstr, unsigned int isize)
{
    if (pstr != NULL)
    {
        if (pstr[0] == 'x')
            return (Bin)_bin_create_x(pstr+1, isize-1);
    }
    return 0;
}


_bin* _bin_create3(_bin *pa)
{
    if (pa != NULL)
    {
        _bin *pa2 = _bin_create();
        memcpy(pa2->cBin, pa->cBin, sizeof(pa->cBin));
        if (pa->pnext != NULL)
            pa2->pnext = _bin_create3(pa->pnext);
        return pa2;
    }
    return NULL;
}

Bin bin_create3(Bin a)
{
    _bin *pa = (_bin*)a;
    if (pa != NULL)
        return (Bin)_bin_create3(pa);
    return 0;
}

void _bin_free(_bin *pa)
{
    if (pa != NULL)
    {
        _bin_free(pa->pnext);
        _FREE(pa);
    }
}

void bin_free(Bin bin)
{
    _bin *pa = (_bin*)bin;
    if (pa != NULL)
        _bin_free(pa);
}

void _bin_init2(_bin *pa, _bin *pb)
{
    if (pa != NULL && pb != NULL)
    {
        memcpy(pa->cBin, pb->cBin, sizeof(pb->cBin));
        if (pb->pnext != NULL)
        {
            if (pa->pnext == NULL)
                pa->pnext = _bin_create();
            _bin_init2(pa->pnext, pb->pnext);
        }
    }
}

void _bin_shift_next(_bin **ppa, _bin **ppb)
{
    *ppa = *ppb;
    *ppb = (*ppa)->pnext;
}

void _bin_lshift1(_bin *pa)
{
    if (pa != NULL)
    {
        int iBit = 0;
        _bin *pa_tmp = pa;
        _bin *pa_next = pa_tmp;
        while (iBit > 0 || pa_next != NULL)
        {
            _bin_extend_if_null(&pa_next, &pa_tmp);
            for (int i = CHAR_NUM-1; i >= 0; i--)
            {
                int inext = pa_next->cBin[i] >> 7;
                pa_next->cBin[i] = ((pa_next->cBin[i] << 1) | iBit);
                iBit = inext;
            }
            _bin_shift_next(&pa_tmp, &pa_next);
        }
    }
}

void bin_lshift(_bin *pbin, int iShift)
{
    if (pbin != NULL)
    {
        for (int i = 0; i < iShift; i++)
            _bin_lshift1(pbin);
    }
}

/*
// TODO
bool bin_init_istr(_bin *pbin, char *szStr, size_t iLen)
{
}
*/

int _bin_is_zero(_bin *pa)
{
    if (pa != NULL)
    {
        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pa->cBin[i] > 0)
                return false;
        }
    }
    return true;
}

void _bin_clean(_bin *pa)
{
    if (pa != NULL)
    {
        _bin_clean(pa->pnext);
        if (pa->pnext != NULL && _bin_is_zero(pa->pnext))
        {
            _bin_free(pa->pnext);
            pa->pnext = NULL;
        }
    }
}

int _bin_rshift1(_bin *pa)
{
    int ibit = 0;
    if (pa != NULL)
    {
        ibit = _bin_rshift1(pa->pnext);
        for (int i = 0; i < CHAR_NUM; i++)
        {
            int ibit_tmp = pa->cBin[i] & 1;
            pa->cBin[i] = (ibit << 7) | (pa->cBin[i] >> 1);
            ibit = ibit_tmp;
        }
    }
    return ibit;
}

void bin_rshift(_bin *pa, int iShift)
{
    if (pa != NULL)
    {
        for (int i = 0; i < iShift; i++)
            _bin_rshift1(pa);
        _bin_clean(pa);
    }
}

Bin bin_add(Bin a, Bin b)
{
    Bin r = 0;
    bin_add2(a, b, &r);
    return r;
}

void bin_add2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)(*ppr);
    if (pr == NULL)
    {
        pr = _bin_create();
        *ppr = (Bin)pr;
    }
    else
        bin_init(pr);
    _bin_add(pa, pb, pr);
}


void _bin_add(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px = _bin_create3(pa);
        _bin *py = _bin_create3(pb);
        _bin *pa_tmp = NULL, *pa_next = px;
        _bin *pb_tmp = NULL, *pb_next = py;
        _bin *pr_tmp = NULL, *pr_next = pr;
        char chCarry = 0x00;
        while (chCarry > 0 || pa_next != NULL || pb_next != NULL)
        {
            _bin_extend_if_null(&pa_next, &pa_tmp);
            _bin_extend_if_null(&pb_next, &pb_tmp);
            _bin_extend_if_null(&pr_next, &pr_tmp);

            for (int i = CHAR_NUM-1; i >= 0; i--)
            {
                for (int j = 0; j < 8; j++)
                {
                    char chCur = 0x00;
                    int iMsk = 1 << j;
                    if ((pa_next->cBin[i] & iMsk) == (pb_next->cBin[i] & iMsk))
                    {
                        chCur = chCarry;
                        chCarry = (pa_next->cBin[i] & iMsk) ? 0x01 : 0x00;
                    }
                    else
                        chCur = (chCarry == 0x01) ? 0x00 : 0x01;
                    if (chCur)
                        pr_next->cBin[i] |= (chCur << j);
                    else
                        pr_next->cBin[i] &= ~(1 << j);
                }
            }
            _bin_shift_next(&pa_tmp, &pa_next);
            _bin_shift_next(&pb_tmp, &pb_next);
            _bin_shift_next(&pr_tmp, &pr_next);
        }
        _bin_free(px);
        _bin_free(py);
    }
}

Bin bin_mul(Bin a, Bin b)
{
    Bin r = 0;
    bin_mul2(a, b, &r);
    return r;
}

void bin_mul2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)(*ppr);
    if (pr == NULL)
    {
        pr = (_bin*)bin_create("x0");
        *ppr = (Bin)pr;
    }
    else
        bin_init(pr);
    _bin_mul(pa, pb, pr);
}

void _bin_mul(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px = _bin_create3(pa);
        _bin *py = _bin_create3(pb);
        for (int i = CHAR_NUM-1; i >= 0; i--)
        {
            for (int j = 0; j < 8; j++)
            {
                if (py->cBin[i] & (1 << j))
                    _bin_add(pr, px, pr);
                bin_lshift(px, 1);
            }
        }
        _bin_free(px);
        _bin_free(py);
    }
}

int bin_eq(_bin *pa, _bin *pb)
{
    if (pa == NULL && pb == NULL)
        return 0;
    else if (pa != NULL && pb == NULL)
        return 1;
    else if (pa == NULL && pb != NULL)
        return -1;
    int ret = bin_eq(pa->pnext, pb->pnext);
    if (ret == 0)
    {
        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pa->cBin[i] > pb->cBin[i])
                return 1;
            else if (pa->cBin[i] < pb->cBin[i])
                return -1;
        }
    }
    return ret;
}

int _bin_get_startIndex(_bin *pa)
{
    if (pa != NULL)
    {
        _bin *pa_next = pa;
        for (; pa_next->pnext != NULL; pa_next = pa_next->pnext);

        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pa_next->cBin[i] > 0)
            {
                for (int j = 7; j >= 0; j--)
                {
                    if (pa_next->cBin[i] & (1 << j))
                        return i * CHAR_BITS + 8-j-1;
                }
            }
        }
        pa_next = pa_next->pnext;
    }
    return -1;
}

_bin* _bin_align(_bin *pa, _bin *pb)
{
    if (pa != NULL && pb != NULL)
    {
        int ia = _bin_get_startIndex(pa);
        int ib = _bin_get_startIndex(pb);
        _bin *pa_next = pa;
        int iext = -1;
        for (; pa_next != NULL; iext++, pa_next = pa_next->pnext);
        ib = ib + iext * BIN_LEN;
        _bin *pr = _bin_create3(pb);
        bin_lshift(pr, ib - ia);
        return pr;
    }
    return NULL;
}

Bin bin_mod(Bin a, Bin b)
{
    Bin r = 0;
    bin_mod2(a, b, &r);
    return r;
}

void bin_mod2(Bin a, Bin b, Bin *ppm)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin r;
    bin_init(&r);
    _bin *pm = (_bin*)(*ppm);
    if (pm == NULL)
    {
        pm = (_bin*)bin_create("x0");
        *ppm = (Bin)pm;
    }
    else
        bin_init(pm);
    _bin_div(pa, pb, &r, pm);
}

Bin bin_div(Bin a, Bin b)
{
    Bin r = 0;
    bin_div2(a, b, &r);
    return r;
}

void bin_div2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)(*ppr);
    if (pr == NULL)
    {
        pr = (_bin*)bin_create("x0");
        *ppr = (Bin)pr;
    }
    else
        bin_init(pr);
    _bin_div(pa, pb, pr, NULL);
}

void _bin_div(_bin *pa, _bin *pb, _bin *pr, _bin *pm)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin *px = _bin_create3(pa);
        _bin *paligned = _bin_align(px, pb);
        while (bin_eq(paligned, pb) >= 0)
        {
            bin_lshift(pr, 1);
            if (bin_eq(px, paligned) >= 0)
            {
                _bin_sub(px, paligned, px);
                pr->cBin[CHAR_NUM-1] |= 0x01;
            }
            bin_rshift(paligned, 1);
            //bin_printx(px);
        }
        if (pm != NULL)
            _bin_init2(pm, px);
        _bin_free(paligned);
        _bin_free(px);
    }
}

Bin bin_sub(Bin a, Bin b)
{
    Bin r = 0;
    bin_sub2(a, b, &r);
    return r;
}

void bin_sub2(Bin a, Bin b, Bin *ppr)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)(*ppr);
    if (pr == NULL)
    {
        pr = (_bin*)bin_create("x0");
        *ppr = (Bin)pr;
    }
    else
        bin_init(pr);
    _bin_sub(pa, pb, pr);
}

void _bin_complement(_bin *pa)
{
    if (pa != NULL)
    {
        _bin *pa_next = pa;
        while (pa_next != NULL)
        {
            for (int i = 0; i < CHAR_NUM; i++)
                pa_next->cBin[i] = ~(pa_next->cBin[i]);
            pa_next = pa_next->pnext;
        }
    }
}

bool _bin_discard_first_digit(_bin *pa)
{
    if (pa != NULL)
    {
        if (pa->pnext == NULL)
            return true;
        if (_bin_discard_first_digit(pa->pnext))
        {
            _bin_free(pa->pnext);
            pa->pnext = NULL;
        }
    }
    return false;
}

void _bin_extend_if_required(_bin *pa, _bin *pb)
{
    if (pa == NULL)
        return;
    if (pa->pnext != NULL && pb->pnext == NULL)
        pb->pnext = _bin_create();
    _bin_extend_if_required(pa->pnext, pb->pnext);
}

void _bin_sub(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        _bin* pOne = (_bin*)bin_create("x1");
        _bin *py =  _bin_create3(pb);
        _bin_extend_if_required(pa, py);
        _bin_complement(py);
        _bin *p2cmp = (_bin*)bin_add((Bin)py, (Bin)pOne);
        _bin_add(pa, p2cmp, pr);
        // assume the first block is 1 so free it
        _bin_discard_first_digit(pr);
        _bin_clean(pr);
        _bin_free(p2cmp);
        _bin_free(py);
        _bin_free(pOne);
    }
}

void _bin_reverse_str(char *szStr, size_t iLen)
{
    int iLastIndex = iLen - 1;
    for (int i = 0; i < iLen / 2; i++)
    {
        int iIndexToSwap = iLastIndex-i;
        if (iIndexToSwap > i)
        {
            char ch = szStr[i];
            szStr[i] = szStr[iIndexToSwap];
            szStr[iIndexToSwap] = ch;
        }
    }
}

void bin_printb(_bin *pbin)
{
    char szStr[257] = {0};
    bin2bstr((Bin)pbin, szStr, 256);
    int i = 0;
    for (; i < 257; i++)
    {
        if (szStr[i] == '1')
            break;
    }
    printf("%10s\n", szStr+i);
}

void _bin_printx(_bin *pa)
{
    if (pa != NULL)
    {
        _bin_printx(pa->pnext);
        char szStr[65] = {0};
        bin2xstr((Bin)pa, szStr, 64);
        printf("%s", szStr);
    }
}

void bin_printx(_bin *pa)
{
    _bin_printx(pa);
    printf("\n");
}

void bin2bstr(Bin bin, char *szStr, size_t iLen)
{
    _bin *pbin = (_bin*)bin;
    if (pbin != NULL && szStr != NULL && iLen > 0)
    {
        for (int i = BIN_LEN - 1, j = 0; i >= 0; i--, j++)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            if (j < iLen)
                szStr[j] = (pbin->cBin[iIndex] & (1 << iBitOffset)) ? '1' : '0';
        }
    }
    _bin_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
}

void bin2xstr(Bin bin, char *szStr, size_t iLen)
{
    _bin *pa_next = (_bin*)bin;
    if (pa_next != NULL && szStr != NULL && iLen > 0)
    {
        //TODO: remove
        for (int i = 0; i < iLen; szStr[i++] = '0');

        int j = 0;
        while (pa_next != NULL && j < iLen) 
        {
            for (int i = CHAR_NUM-1; i >= 0 && j < iLen; i--, j+=2)
            {
                char ch1 = _ch2x(pa_next->cBin[i] >> 4);
                char ch2 = _ch2x(pa_next->cBin[i] & 0x0f);
                szStr[j] = ch2;
                if (j+1 < iLen)
                    szStr[j+1] = ch1;
            }
            pa_next = pa_next->pnext;
        }
        _bin_reverse_str(szStr, iLen);
    }
}

/*
// float
void _fstr2bin(_bin *pbin, char *szStr, size_t iLen)
{
}

bool bin_init_fstr(_bin *pbin, char *szFloat, size_t iLen)
{
}

void _fstr_get_mantissa(_bin *pB1, _bin *pB2, _bin *pBRet, int *piExp)
{
}

void _fstr_add_parts(_bin *pB1, _bin *pB2, _bin *pBRet)
{
}
*/
