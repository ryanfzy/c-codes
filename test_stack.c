#include <stdlib.h>
#include <check.h>
#include "stack.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_stack_create)
{
    //fail_unless(add(2,3) == 5, "error, 2+3!=5");
    Stack *pstack = stack_create();
    ck_assert_int_eq(pstack->icount, 0);
    ck_assert_msg(pstack->ptop == NULL, "the top of a newly created stack is not null");

    ck_assert_msg(stack_isempty(pstack) == true, "stack_isempty() return false for newly created statck");
}
END_TEST

START_TEST(test_stack_create2)
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
    ck_assert_msg(stack_isempty(ps) == false, "stack_isempty() return true for stack containing one item");

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
    ck_assert_msg(stack_isempty(&s) == false, "stack_isempty() return true for stack containing 2 items");

    ck_assert_int_eq(((TestData*)s.ptop->pdata)->ivalue, 10);
    ck_assert(((TestData*)s.ptop->pdata)->dvalue == (double)20);
    ck_assert_int_eq(s.ptop->isize, sizeof(TestData));

    ck_assert_msg(s.ptop->pnext != NULL, "stack nodes didn't linked when pushing more than one item into stack");

    ck_assert_msg(s.ptop->pnext->pdata != (char*)&t1, "stack didn't make a new copy when push");

    ck_assert_int_eq(((TestData*)s.ptop->pnext->pdata)->ivalue, 1);
    ck_assert(((TestData*)s.ptop->pnext->pdata)->dvalue ==(double)2);
    ck_assert_int_eq(s.ptop->pnext->isize, sizeof(TestData));
}
END_TEST

START_TEST(test_stack_pop)
{
    TestData t1, t2;
    Stack *ps = stack_create();
    t1.ivalue = 123;
    t1.dvalue = 124;

    stack_push(ps, (char*)&t1, sizeof(TestData));

    TestData *pt = (TestData*)ps->ptop->pdata;
    ck_assert_int_eq(pt->ivalue, 123);
    ck_assert_int_eq(pt->dvalue, (double)124);

    stack_pop(ps, (char*)&t2, sizeof(TestData));
    ck_assert_int_eq(ps->icount, 0);
    ck_assert_int_eq(t2.ivalue, 123);
    ck_assert_int_eq(t2.dvalue, (double)124);

    ck_assert_msg(ps->ptop == NULL, "after stack pop the last item, ptop is not NULL");

    // this will crash because TestData pointed by pt is freed when it is popped out
    //ck_assert_msg(pt->ivalue != 123, "stack didn't free the item when pop");
}
END_TEST

START_TEST(test_stack_pop2)
{
    TestData t1, t2, t3;
    Stack s;

    stack_init(&s);
    t1.ivalue = 123;
    t1.dvalue = 124;
    t2.ivalue = 223;
    t2.dvalue = 224;

    stack_push(&s, (char*)&t1, sizeof(TestData));
    stack_push(&s, (char*)&t2, sizeof(TestData));

    stack_pop(&s, (char*)&t3, sizeof(TestData));
    ck_assert_int_eq(s.icount, 1);
    ck_assert_int_eq(t3.ivalue, 223);
    ck_assert_int_eq(t3.dvalue, (double)224);

    stack_pop(&s, (char*)&t3, sizeof(TestData));
    ck_assert_int_eq(s.icount, 0);
    ck_assert_int_eq(t3.ivalue, 123);
    ck_assert_int_eq(t3.dvalue, (double)124);
}
END_TEST

START_TEST(test_stack_free)
{
    TestData t;
    Stack *ps = stack_create();

    t.ivalue = 1;
    t.dvalue = 2;
    stack_push(ps, (char*)&t, sizeof(TestData));

    stack_free(ps);
    // cannot test this
    //ck_assert_msg(ps == NULL, "stack pointer is not null after the stack is freed");
}
END_TEST

START_TEST(test_stack_destroy)
{
    TestData t;
    Stack s;
    stack_init(&s);

    t.ivalue = 1;
    t.dvalue = 2;
    stack_push(&s, (char*)&t, sizeof(TestData));

    stack_destroy(&s);
    ck_assert_msg(s.ptop == NULL, "ptop is not NULL after the stack is destroyed");
    ck_assert_int_eq(s.icount, 0);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("stack");
    TCase *tc_stack = tcase_create("stack");
    tcase_add_test(tc_stack, test_stack_create);
    tcase_add_test(tc_stack, test_stack_create2);
    tcase_add_test(tc_stack, test_stack_push);
    tcase_add_test(tc_stack, test_stack_push2);
    tcase_add_test(tc_stack, test_stack_pop);
    tcase_add_test(tc_stack, test_stack_pop2);
    tcase_add_test(tc_stack, test_stack_free);
    tcase_add_test(tc_stack, test_stack_destroy);
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
