#include <stdlib.h>
#include <check.h>
#include "list.h"

#define CK_ASSERT_INT_EQ(E,A) ck_assert_msg(E == A, "Expected %d, but actual %d.\n", E, A)
#define CK_ASSERT_TRUE(A) ck_assert_msg(A, "Expected true, but actual false.\n")
#define CK_ASSERT_FALSE(A) ck_assert_msg(!A, "Expected false, but actual true.\n")

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_list_create1)
{
    List *list = list_create();
    ck_assert_msg(list->first == NULL, "first is not NULL");
    ck_assert_msg(list->last == NULL, "last is not NULL");
    CK_ASSERT_INT_EQ(0, list->count);
    list_free(list);
}
END_TEST

START_TEST(test_list_create2)
{
    List list;
    list_init(&list);
    ck_assert_msg(list.first == NULL, "first is not NULL");
    ck_assert_msg(list.last == NULL, "last is not NULL");
    CK_ASSERT_INT_EQ(0, list.count);
    list_destroy(&list);
}
END_TEST

START_TEST(test_list_add1)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 123;
    list_add(list, (char*)&t, sizeof(TestData));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first == list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next == NULL, "next is not NULL");
    ck_assert_msg(list->first->prev == NULL, "prev is not NULL");
    CK_ASSERT_INT_EQ(1, list->count);

    ck_assert_msg(((TestData*)(list->first->data))->ivalue == 123, "test data is not pushed into list");
    ck_assert_msg(list->first->data != (char*)&t, "test data is not copied into list");

    t.ivalue = 321;
    ck_assert_msg(((TestData*)(list->first->data))->ivalue == 123, "test data in the list changed");

    list_free(list);
}
END_TEST

START_TEST(test_list_add2)
{
    TestData t;
    List list;
    t.ivalue = 1;
    list_init(&list);
    list_add(&list, (char*)&t, sizeof(TestData));
    ck_assert_msg(list.first != NULL, "first is NULL");
    ck_assert_msg(list.last != NULL, "last is NULL");
    ck_assert_msg(list.first == list.last, "first and last not pointing to the same object");
    ck_assert_msg(list.first->next == NULL, "next is not NULL");
    ck_assert_msg(list.first->prev == NULL, "prev is not NULL");
    CK_ASSERT_INT_EQ(1, list.count);

    t.ivalue = 2;
    list_add(&list, (char*)&t,sizeof(TestData));
    ck_assert_msg(list.first != NULL, "first is NULL");
    ck_assert_msg(list.last != NULL, "last is NULL");
    ck_assert_msg(list.first != list.last, "first and last are pointing to the same object");
    CK_ASSERT_INT_EQ(2, list.count);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list.first->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list.last->data))->ivalue);
    ck_assert_msg(list.first->next == list.last, "first's next is not pointing to last");
    ck_assert_msg(list.first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list.last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list.last->prev == list.first, "last's prev is not pointing to first");

    t.ivalue = 3;
    list_add(&list, (char*)&t, sizeof(TestData));
    ck_assert_msg(list.first != NULL, "first is NULL");
    ck_assert_msg(list.last != NULL, "last is NULL");
    ck_assert_msg(list.first != list.last, "first and last are pointing to the same object");
    CK_ASSERT_INT_EQ(3, list.count);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list.first->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list.first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(3, ((TestData*)(list.last->data))->ivalue);
    ck_assert_msg(list.first->next != NULL, "first's next is NULL");
    ck_assert_msg(list.first->next != list.last, "first's next is pointing to last");
    ck_assert_msg(list.first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list.last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list.last->prev != NULL, "last's prev is NULL");
    ck_assert_msg(list.last->prev != list.first, "last's prev is pointing to first");
    ck_assert_msg(list.first->next == list.last->prev, "first's next and last's prev is not pointing to the same object");

    list_destroy(&list);
}
END_TEST

START_TEST(test_list_free1)
{
    TestData t;
    List list;
    t.ivalue = 1;
    list_init(&list);
    list_add(&list, (char*)&t, sizeof(TestData));

    CK_ASSERT_INT_EQ(1, list.count);

    list_destroy(&list);
    CK_ASSERT_INT_EQ(0, list.count);
    ck_assert_msg(list.first == NULL, "pfirst is not NULL");
    ck_assert_msg(list.last == NULL, "plast is not NULL");
}
END_TEST

START_TEST(test_list_insert1)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 123;
    CK_ASSERT_TRUE(list_insert(list, 0, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first == list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next == NULL, "next is not NULL");
    ck_assert_msg(list->first->prev == NULL, "prev is not NULL");
    CK_ASSERT_INT_EQ(1, list->count);

    ck_assert_msg(((TestData*)(list->first->data))->ivalue == 123, "test data is not pushed into list");
    ck_assert_msg(list->first->data != (char*)&t, "test data is not copied into list");

    t.ivalue = 321;
    ck_assert_msg(((TestData*)(list->first->data))->ivalue == 123, "test data in the list changed");

    list_free(list);
}
END_TEST

START_TEST(test_list_insert2)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 123;
    CK_ASSERT_TRUE(list_insert(list, -1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first == list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next == NULL, "next is not NULL");
    ck_assert_msg(list->first->prev == NULL, "prev is not NULL");
    CK_ASSERT_INT_EQ(1, list->count);

    ck_assert_msg(((TestData*)(list->first->data))->ivalue == 123, "test data is not pushed into list");
    ck_assert_msg(list->first->data != (char*)&t, "test data is not copied into list");

    t.ivalue = 321;
    ck_assert_msg(((TestData*)(list->first->data))->ivalue == 123, "test data in the list changed");

    list_free(list);
}
END_TEST

START_TEST(test_list_insert3)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 123;
    CK_ASSERT_FALSE(list_insert(list, 1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first == NULL, "first is not NULL");
    ck_assert_msg(list->last == NULL, "last is not NULL");
    CK_ASSERT_INT_EQ(0, list->count);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert4)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 123;
    CK_ASSERT_FALSE(list_insert(list, -2, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first == NULL, "first is not NULL");
    ck_assert_msg(list->last == NULL, "last is not NULL");
    CK_ASSERT_INT_EQ(0, list->count);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert5)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    CK_ASSERT_TRUE(list_insert(list, 0, (char*)&t, sizeof(TestData)));
    
    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next == list->last, "first's next is not last");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev == list->first, "last's prev is not first");
    CK_ASSERT_INT_EQ(2, list->count);
    
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert6)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    CK_ASSERT_TRUE(list_insert(list, -1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next == list->last, "first's next is not last");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev == list->first, "last's prev is not first");
    CK_ASSERT_INT_EQ(2, list->count);

    CK_ASSERT_INT_EQ(2, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert7)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    CK_ASSERT_FALSE(list_insert(list, 1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first == list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next == NULL, "next is not NULL");
    ck_assert_msg(list->first->prev == NULL, "prev is not NULL");
    CK_ASSERT_INT_EQ(1, list->count);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert8)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 3;
    CK_ASSERT_TRUE(list_insert(list, 1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next != list->last, "first's next is last");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->first->next->next == list->last, "second's next is not last");
    ck_assert_msg(list->first->next->prev == list->first, "second's prev is not first");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev != list->first, "last's prev is first");
    ck_assert_msg(list->last->prev->prev == list->first, "second last's prev is first");
    ck_assert_msg(list->last->prev->next == list->last, "second last's prev is last");
    ck_assert_msg(list->first->next == list->last->prev, "first's next is not last's prev");
    CK_ASSERT_INT_EQ(3, list->count);

    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(3, ((TestData*)(list->first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert9)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 3;
    CK_ASSERT_TRUE(list_insert(list, -1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next != list->last, "first's next is last");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->first->next->next == list->last, "second's next is not last");
    ck_assert_msg(list->first->next->prev == list->first, "second's prev is not first");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev != list->first, "last's prev is first");
    ck_assert_msg(list->last->prev->prev == list->first, "second last's prev is first");
    ck_assert_msg(list->last->prev->next == list->last, "second last's prev is last");
    ck_assert_msg(list->first->next == list->last->prev, "first's next is not last's prev");
    CK_ASSERT_INT_EQ(3, list->count);

    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(3, ((TestData*)(list->first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert10)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 3;
    CK_ASSERT_TRUE(list_insert(list, 0, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next != list->last, "first's next is last");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->first->next->next == list->last, "second's next is not last");
    ck_assert_msg(list->first->next->prev == list->first, "second's prev is not first");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev != list->first, "last's prev is first");
    ck_assert_msg(list->last->prev->prev == list->first, "second last's prev is first");
    ck_assert_msg(list->last->prev->next == list->last, "second last's prev is last");
    ck_assert_msg(list->first->next == list->last->prev, "first's next is not last's prev");
    CK_ASSERT_INT_EQ(3, list->count);

    CK_ASSERT_INT_EQ(3, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert11)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 3;
    CK_ASSERT_TRUE(list_insert(list, -2, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next != list->last, "first's next is last");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->first->next->next == list->last, "second's next is not last");
    ck_assert_msg(list->first->next->prev == list->first, "second's prev is not first");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev != list->first, "last's prev is first");
    ck_assert_msg(list->last->prev->prev == list->first, "second last's prev is first");
    ck_assert_msg(list->last->prev->next == list->last, "second last's prev is last");
    ck_assert_msg(list->first->next == list->last->prev, "first's next is not last's prev");
    CK_ASSERT_INT_EQ(3, list->count);

    CK_ASSERT_INT_EQ(3, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert12)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 3;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 4;
    CK_ASSERT_TRUE(list_insert(list, 1, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next != NULL, "first's next is NULL");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev != NULL, "last's prev is NULL");
    ck_assert_msg(list->first->next == list->last->prev->prev, "first's next is not last's prev's prev");
    CK_ASSERT_INT_EQ(4, list->count);

    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(4, ((TestData*)(list->first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->first->next->next->data))->ivalue);
    CK_ASSERT_INT_EQ(3, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_insert13)
{
    TestData t;
    List *list = list_create();
    t.ivalue = 1;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 2;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 3;
    list_add(list, (char*)&t, sizeof(TestData));
    t.ivalue = 4;
    CK_ASSERT_TRUE(list_insert(list, -2, (char*)&t, sizeof(TestData)));

    ck_assert_msg(list->first != NULL, "first is NULL");
    ck_assert_msg(list->last != NULL, "last is NULL");
    ck_assert_msg(list->first != list->last, "first and last not pointing to the same object");
    ck_assert_msg(list->first->next != NULL, "first's next is NULL");
    ck_assert_msg(list->first->prev == NULL, "first's prev is not NULL");
    ck_assert_msg(list->last->next == NULL, "last's next is not NULL");
    ck_assert_msg(list->last->prev != NULL, "last's prev is NULL");
    ck_assert_msg(list->first->next == list->last->prev->prev, "first's next is not last's prev's prev");
    CK_ASSERT_INT_EQ(4, list->count);

    CK_ASSERT_INT_EQ(1, ((TestData*)(list->first->data))->ivalue);
    CK_ASSERT_INT_EQ(4, ((TestData*)(list->first->next->data))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)(list->first->next->next->data))->ivalue);
    CK_ASSERT_INT_EQ(3, ((TestData*)(list->last->data))->ivalue);

    list_free(list);
}
END_TEST

START_TEST(test_list_get1)
{
    TestData t1, *t2, *t3;
    t1.ivalue = 1;
    List *list = list_create();
    list_add(list, (char*)&t1, sizeof(TestData));
    t2 = (TestData*)list_get(list, 0);
    CK_ASSERT_INT_EQ(1, t2->ivalue);
    t1.ivalue = 2;
    list_add(list, (char*)&t1, sizeof(TestData));
    t2 = (TestData*)list_get(list, 1);
    CK_ASSERT_INT_EQ(2, t2->ivalue);
    t3 = (TestData*)list_get(list, 1);
    ck_assert_msg(t3 == t2, "not returning the same object");
}
END_TEST

START_TEST(test_list_get2)
{
    TestData t1, *t2, *t3;
    t1.ivalue = 1;
    List *list = list_create();
    list_add(list, (char*)&t1, sizeof(TestData));
    t2 = (TestData*)list_get(list, -1);
    CK_ASSERT_INT_EQ(1, t2->ivalue);
    t1.ivalue = 2;
    list_insert(list, 0, (char*)&t1, sizeof(TestData));
    t2 = (TestData*)list_get(list, -2);
    CK_ASSERT_INT_EQ(2, t2->ivalue);
    t3 = (TestData*)list_get(list, -2);
    ck_assert_msg(t3 == t2, "not returning the same object");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("list");
    TCase *tc_list = tcase_create("list");
    tcase_add_test(tc_list, test_list_create1);
    tcase_add_test(tc_list, test_list_create2);
    tcase_add_test(tc_list, test_list_add1);
    tcase_add_test(tc_list, test_list_add2);
    tcase_add_test(tc_list, test_list_free1);
    tcase_add_test(tc_list, test_list_insert1);
    tcase_add_test(tc_list, test_list_insert2);
    tcase_add_test(tc_list, test_list_insert3);
    tcase_add_test(tc_list, test_list_insert4);
    tcase_add_test(tc_list, test_list_insert5);
    tcase_add_test(tc_list, test_list_insert6);
    tcase_add_test(tc_list, test_list_insert7);
    tcase_add_test(tc_list, test_list_insert8);
    tcase_add_test(tc_list, test_list_insert9);
    tcase_add_test(tc_list, test_list_insert10);
    tcase_add_test(tc_list, test_list_insert11);
    tcase_add_test(tc_list, test_list_insert12);
    tcase_add_test(tc_list, test_list_insert13);
    tcase_add_test(tc_list, test_list_get1);
    tcase_add_test(tc_list, test_list_get2);
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
