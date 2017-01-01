#include <stdlib.h>
#include <check.h>
#include "stack.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_stack_init)
{
    //fail_unless(add(2,3) == 5, "error, 2+3!=5");
    Stack *pstack = stack_create();
    ck_assert_int_eq(pstack->icount, 0);
    ck_assert_msg(pstack->ptop == NULL, "the top of a newly created stack is not null");
}
END_TEST

START_TEST(test_stack_init2)
{
    Stack s;
    stack_init(&s);
    ck_assert_int_eq(s.icount, 0);
    ck_assert_msg(s.ptop == NULL, "the top of a initialised stack is not null");
}
END_TEST

START_TEST(test_stack_push)
{
    TestData t;
    Stack *ps = stack_create();

    t.ivalue = 1;
    t.dvalue = 2;
    stack_push(ps, (char*)&t, sizeof(TestData));
    ck_assert_int_eq(ps->icount, 1);
    ck_assert_int_eq(((TestData*)(ps->ptop->pdata))->ivalue, 1);
    ck_assert_int_eq(((TestData*)(ps->ptop->pdata))->dvalue, 2);
    
    ck_assert_msg((char*)&t != ps->ptop->pdata, "stack didn't make a new copy when push");

    ck_assert_int_eq(ps->ptop->isize, sizeof(TestData));

    ck_assert_msg(ps->ptop->pnext == NULL, "the only one item in the stack has a not null pnext");

    t.ivalue = 3;
    t.dvalue = 4;
    ck_assert_int_eq(((TestData*)(ps->ptop->pdata))->ivalue, 1);
    ck_assert_int_eq(((TestData*)(ps->ptop->pdata))->dvalue, 2);
}
END_TEST

START_TEST(test_stack_push2)
{
    TestData t1, t2;
    Stack s;
    stack_init(&s);

    t1.ivalue = 1;
    t1.dvalue = 2;
    t2.ivalue = 10;
    t2.dvalue = 20;

    stack_push(&s, (char*)&t1, sizeof(TestData));
    stack_push(&s, (char*)&t2, sizeof(TestData));

    ck_assert_int_eq(s.icount, 2);
    ck_assert_int_eq(((TestData*)s.ptop->pdata)->ivalue, 10);
    ck_assert(((TestData*)s.ptop->pdata)->dvalue == (double)20);
    ck_assert_int_eq(s.ptop->isize, sizeof(TestData));

    ck_assert_int_eq(((TestData*)s.ptop->pnext->pdata)->ivalue, 1);
    ck_assert(((TestData*)s.ptop->pnext->pdata)->dvalue ==(double)2);
    ck_assert_int_eq(s.ptop->pnext->isize, sizeof(TestData));
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("stack");
    TCase *tc_stack = tcase_create("stack");
    tcase_add_test(tc_stack, test_stack_init);
    tcase_add_test(tc_stack, test_stack_init2);
    tcase_add_test(tc_stack, test_stack_push);
    tcase_add_test(tc_stack, test_stack_push2);
    suite_add_tcase(s, tc_stack);
    return s;
}

int main(void)
{
    int n;
    SRunner *sr;
    Suite *s = make_add_suit();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    n = srunner_ntests_failed(sr);
    srunner_free(sr);
    return n == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
