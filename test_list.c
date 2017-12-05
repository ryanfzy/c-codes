#include <stdlib.h>
#include <check.h>
#include "list.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_list_create1)
{
    SList *plist = slist_create();
    ck_assert_msg(plist->pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(plist->plast == NULL, "plast is not NULL");
    ck_assert_int_eq(plist->icount, 0);
}
END_TEST

START_TEST(test_list_create2)
{
    SList list;
    slist_init(&list);
    ck_assert_msg(list.pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(list.plast == NULL, "plast is not NULL");
    ck_assert_int_eq(list.icount, 0);
}
END_TEST

START_TEST(test_list_push1)
{
    TestData t;
    SList *plist = slist_create();
    t.ivalue = 123;
    slist_push(plist, (char*)&t, sizeof(TestData));

    ck_assert_msg(plist->pfirst != NULL, "pfirst is NULL");
    ck_assert_msg(plist->plast != NULL, "plast is NULL");
    ck_assert_msg(plist->pfirst == plist->plast, "pfirst and plast not refering same object");
    ck_assert_int_eq(plist->icount, 1);

    ck_assert_msg(((TestData*)(plist->pfirst->pdata))->ivalue == 123, "test data is not pushed into list");
    ck_assert_msg(plist->pfirst->pdata != (char*)&t, "test data is not copied into list");

    t.ivalue = 321;
    ck_assert_msg(((TestData*)(plist->pfirst->pdata))->ivalue == 123, "test data in the list changed");

    slist_free(plist);
    //ck_assert_msg(plist->icount, 0);
    //ck_assert_msg(plist->pfirst == NULL, "pfirst is not NULL after freed");
}
END_TEST

START_TEST(test_list_push2)
{
    TestData t;
    SList list;
    t.ivalue = 1;
    slist_init(&list);
    slist_push(&list, (char*)&t, sizeof(TestData));

    ck_assert_msg(list.pfirst != NULL, "pfirst is NULL");
    ck_assert_msg(list.plast != NULL, "plast is NULL");
    ck_assert_msg(list.pfirst == list.plast, "pfirst and plast not refering same object");
    ck_assert_int_eq(list.icount, 1);

    t.ivalue = 2;
    slist_push(&list, (char*)&t,sizeof(TestData));

    ck_assert_msg(list.pfirst != list.plast, "pfirst and plast are refering to the same object");
    ck_assert_int_eq(list.icount, 2);
    ck_assert_int_eq(((TestData*)(list.pfirst->pdata))->ivalue, 1);
    ck_assert_int_eq(((TestData*)(list.plast->pdata))->ivalue, 2);

    t.ivalue = 3;
    slist_push(&list, (char*)&t, sizeof(TestData));
    ck_assert_int_eq(list.icount, 3);
    ck_assert_int_eq(((TestData*)(list.plast->pdata))->ivalue, 3);

    slist_destroy(&list);
    ck_assert_int_eq(list.icount, 0);
    ck_assert_msg(list.pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(list.plast == NULL, "plast is not NULL");
}
END_TEST

START_TEST(test_list_free1)
{
    TestData t;
    SList list;
    t.ivalue = 1;
    slist_init(&list);
    slist_push(&list, (char*)&t, sizeof(TestData));

    ck_assert_int_eq(list.icount, 1);

    slist_destroy(&list);
    ck_assert_int_eq(list.icount, 0);
    ck_assert_msg(list.pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(list.plast == NULL, "plast is not NULL");
}
END_TEST

START_TEST(test_list_remove1)
{
    TestData t1, t2;
    SList *plist = slist_create();
    t1.ivalue = 1;
    t2.ivalue = 2;
    slist_push(plist, (char*)&t1, sizeof(TestData));

    slist_remove_first(plist, (char*)&t2, sizeof(TestData));

    ck_assert_int_eq(plist->icount, 0);
    ck_assert_msg(plist->pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(plist->plast == NULL, "plast is not NULL");

    ck_assert_int_eq(t2.ivalue, 1);
    
    slist_free(plist);
}
END_TEST

START_TEST(test_list_remove2)
{
    TestData t1, t2, t3, t4;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    SList list;
    slist_init(&list);
    slist_push(&list, (char*)&t1, sizeof(TestData));
    slist_push(&list, (char*)&t2, sizeof(TestData));
    slist_push(&list, (char*)&t3, sizeof(TestData));

    ck_assert_int_eq(((TestData*)(list.plast->pdata))->ivalue, 3);

    ck_assert_int_eq(list.icount, 3);
    ck_assert_msg(list.pfirst != NULL, "pfirst is NULL");
    ck_assert_msg(list.plast != NULL, "plast is NULL");

    slist_remove_first(&list, (char*)&t4, sizeof(TestData));
    ck_assert_int_eq(list.icount, 2);
    ck_assert_int_eq(t4.ivalue, 1);
    slist_remove_first(&list, (char*)&t4, sizeof(TestData));
    ck_assert_int_eq(list.icount, 1);
    ck_assert_int_eq(t4.ivalue, 2);
    ck_assert_msg(list.pfirst == list.plast, "pfirst and plast are not referring the same object");
    slist_remove_first(&list, (char*)&t4, sizeof(TestData));
    ck_assert_int_eq(list.icount, 0);
    ck_assert_int_eq(t4.ivalue, 3);
    ck_assert_msg(list.pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(list.plast == NULL, "plast is not NULL");
}
END_TEST

START_TEST(test_list_remove3)
{
    TestData t1, t2, t3, t4, t5, t6, t7;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    t4.ivalue = 4;
    t5.ivalue = 5;
    t6.ivalue = 6;
    SList *plist = slist_create();
    slist_push(plist, (char*)&t1, sizeof(TestData));
    slist_push(plist, (char*)&t2, sizeof(TestData));
    slist_push(plist, (char*)&t3, sizeof(TestData));
    slist_push(plist, (char*)&t4, sizeof(TestData));
    slist_push(plist, (char*)&t5, sizeof(TestData));
    slist_push(plist, (char*)&t6, sizeof(TestData));

    ck_assert_int_eq(plist->icount, 6);
    ck_assert_int_eq(((TestData*)(plist->plast->pdata))->ivalue, 6);
    
    slist_remove(plist, 0, (char*)&t7, sizeof(TestData)); 
    ck_assert_int_eq(t7.ivalue, 1);
    ck_assert_int_eq(plist->icount, 5);
    slist_remove(plist, 1, (char*)&t7, sizeof(TestData)); 
    ck_assert_int_eq(t7.ivalue, 3);
    ck_assert_int_eq(plist->icount, 4);
    slist_remove(plist, 2, (char*)&t7, sizeof(TestData)); 
    ck_assert_int_eq(t7.ivalue, 5);
    ck_assert_int_eq(plist->icount, 3);
    slist_remove(plist, 2, (char*)&t7, sizeof(TestData)); 
    ck_assert_int_eq(t7.ivalue, 6);
    ck_assert_int_eq(plist->icount, 2);
    slist_remove_first(plist, (char*)&t7, sizeof(TestData));
    ck_assert_int_eq(t7.ivalue, 2);
    ck_assert_int_eq(plist->icount, 1);
    ck_assert_msg(plist->pfirst == plist->plast, "pfirst and plast not referring the same object");
}
END_TEST

START_TEST(test_list_remove4)
{
    TestData t1, t2, t3, t4;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    SList list;
    slist_init(&list);
    slist_push(&list, (char*)&t1, sizeof(TestData));
    slist_push(&list, (char*)&t2, sizeof(TestData));
    slist_push(&list, (char*)&t3, sizeof(TestData));

    slist_remove_last(&list, (char*)&t4, sizeof(TestData));
    ck_assert_int_eq(t4.ivalue, 3);
    ck_assert_int_eq(list.icount, 2);
    slist_remove_last(&list, (char*)&t4, sizeof(TestData));
    ck_assert_int_eq(t4.ivalue, 2);
    ck_assert_int_eq(list.icount, 1);
    slist_remove_last(&list, (char*)&t4, sizeof(TestData));
    ck_assert_int_eq(t4.ivalue, 1);
    ck_assert_int_eq(list.icount, 0);
    ck_assert_msg(list.pfirst == NULL, "pfirst is not NULL");
    ck_assert_msg(list.plast == NULL, "pfirst is not NULL");
}
END_TEST

START_TEST(test_list_get)
{
    TestData t1, *t2, *t3;
    t1.ivalue = 1;
    SList *plist = slist_create();
    slist_push(plist, (char*)&t1, sizeof(TestData));
    t2 = (TestData*)slist_get(plist, 0);
    ck_assert_int_eq(t2->ivalue, 1);
    t1.ivalue = 2;
    slist_push(plist, (char*)&t1, sizeof(TestData));
    t2 = (TestData*)slist_get(plist, 1);
    ck_assert_int_eq(t2->ivalue, 2);
    t3 = (TestData*)slist_get(plist, 1);
    ck_assert_msg(t3 == t2, "not returning the same object");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("list");
    TCase *tc_list = tcase_create("list");
    tcase_add_test(tc_list, test_list_create1);
    tcase_add_test(tc_list, test_list_create2);
    tcase_add_test(tc_list, test_list_push1);
    tcase_add_test(tc_list, test_list_push2);
    tcase_add_test(tc_list, test_list_free1);
    tcase_add_test(tc_list, test_list_remove1);
    tcase_add_test(tc_list, test_list_remove2);
    tcase_add_test(tc_list, test_list_remove3);
    tcase_add_test(tc_list, test_list_remove4);
    tcase_add_test(tc_list, test_list_get);
    suite_add_tcase(s, tc_list);
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
