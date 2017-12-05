#include <stdlib.h>
#include <check.h>
#include "dict.h"

#define SIZEOF(str) (str), strlen((str))

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_dict_create1)
{
    Dict *pdict = dict_create();
    for (int i = 0; i < 256; i++)
        ck_assert_int_eq(pdict->nodes[i].icount, 0);
    dict_free(pdict);
}
END_TEST

START_TEST(test_dict_create2)
{
    Dict dict;
    dict_init(&dict);
    for (int i = 0; i < 256; i++)
        ck_assert_int_eq(dict.nodes[i].icount, 0);
    dict_destroy(&dict);
}
END_TEST

START_TEST(test_dict_add1)
{
    TestData t;
    t.ivalue = 1;
    Dict *pdict = dict_create();
    dict_add(pdict, SIZEOF("key1"), (char*)&t, sizeof(TestData));

    ck_assert_int_eq(dict_get_count(pdict), 1);

    TestData *t1 = (TestData*)dict_get(pdict, SIZEOF("key1"));
    ck_assert_msg(t1 != &t, "dict did not make a copy of input data");
    ck_assert_int_eq(t1->ivalue, 1);

    dict_add(pdict, SIZEOF("key2"), (char*)&t, sizeof(TestData));
    TestData *t2 = (TestData*)dict_get(pdict, SIZEOF("key2"));
    ck_assert_msg(t1 != t2, "cannot add the same data with two different keys");

    dict_free(pdict);
}
END_TEST

START_TEST(test_dict_add2)
{
    TestData t1, t2, *pt1, *pt2;
    t1.ivalue = 1;
    t2.ivalue = 2;
    Dict dict;
    dict_init(&dict);
    dict_add(&dict, SIZEOF("key1"), (char*)&t1, sizeof(TestData));
    dict_add(&dict, SIZEOF("key2"), (char*)&t2, sizeof(TestData));

    ck_assert_int_eq(dict_get_count(&dict), 2);
    pt1 = (TestData*)dict_get(&dict, SIZEOF("key1"));
    ck_assert_int_eq(pt1->ivalue, 1);
    pt2 = (TestData*)dict_get(&dict, SIZEOF("key2"));
    ck_assert_int_eq(pt2->ivalue, 2);

    dict_destroy(&dict);
    ck_assert_int_eq(dict_get_count(&dict), 0);
}
END_TEST

START_TEST(test_dict_add3)
{
    TestData t, *pt;
    t.ivalue = 1;
    Dict *pdict = dict_create();
    dict_add(pdict, SIZEOF("key1"), (char*)&t, sizeof(TestData));

    pt = (TestData*)dict_get(pdict, SIZEOF("key1"));
    ck_assert_int_eq(pt->ivalue, 1);

    t.ivalue = 2;
    dict_add(pdict, SIZEOF("key1"), (char*)&t, sizeof(TestData));
    pt = (TestData*)dict_get(pdict, SIZEOF("key1"));
    ck_assert_int_eq(pt->ivalue, 1);
}
END_TEST

START_TEST(test_dict_contains1)
{
    TestData t1, t2;
    Dict *pdict = dict_create();
    dict_add(pdict, SIZEOF("somekey1"), (char*)&t1, sizeof(TestData));
    ck_assert_msg(dict_contains(pdict, SIZEOF("somekey1")), "dict cannot add the data with a key");
    dict_add(pdict, SIZEOF("somekey2"), (char*)&t2, sizeof(TestData));
    ck_assert_msg(dict_contains(pdict, SIZEOF("somekey2")), "dict cannot add the data with a key");
    dict_free(pdict);
}
END_TEST

START_TEST(test_dict_set1)
{
    TestData t, *pt;
    t.ivalue = 1;
    Dict dict;
    dict_init(&dict);
    dict_add(&dict, SIZEOF("key1"), (char*)&t, sizeof(TestData));

    pt = (TestData*)dict_get(&dict, SIZEOF("key1"));
    ck_assert_int_eq(pt->ivalue, 1);

    t.ivalue = 2;
    dict_set(&dict, SIZEOF("key1"), (char*)&t, sizeof(TestData));
    pt = (TestData*)dict_get(&dict, SIZEOF("key1"));
    ck_assert_int_eq(pt->ivalue, 2);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("dict");
    TCase *tc_dict = tcase_create("dict");
    tcase_add_test(tc_dict, test_dict_create1);
    tcase_add_test(tc_dict, test_dict_create2);
    tcase_add_test(tc_dict, test_dict_add1);
    tcase_add_test(tc_dict, test_dict_add2);
    tcase_add_test(tc_dict, test_dict_add3);
    tcase_add_test(tc_dict, test_dict_contains1);
    tcase_add_test(tc_dict, test_dict_set1);
    suite_add_tcase(s, tc_dict);
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
