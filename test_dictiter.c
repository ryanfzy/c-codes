#include <stdlib.h>
#include <check.h>
#include "dict.h"
#include "dictiter.h"

#define SIZEOF(str) (str), (strlen((str))+1)

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_dictiter_create)
{
    Dict *pdict = dict_create();
    DictIter *pIter = dict_get_iter(pdict);

    ck_assert_msg(pIter->pDict == pdict, "dict is not the same");
    ck_assert_int_eq(pIter->iNode, -1);
    ck_assert_int_eq(pIter->iPos, -1);

    dictiter_free(pIter);
    dict_free(pdict);
}
END_TEST

START_TEST(test_dictiter_move)
{
    TestData t1, t2, t3;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue =3;
    Dict *pdict = dict_create();

    dict_add(pdict, SIZEOF("test1\0"), (char*)&t1, sizeof(TestData));
    dict_add(pdict, SIZEOF("test2\0"), (char*)&t2, sizeof(TestData));
    dict_add(pdict, SIZEOF("test3\0"), (char*)&t3, sizeof(TestData));

    DictIter *pIter = dict_get_iter(pdict);

    while (dictiter_move_next(pIter))
    {
        char *pKey = dictiter_get_key(pIter);
        TestData *pValue = (TestData*)dictiter_get_value(pIter);

        if (strcmp(pKey, "test1") == 0)
            ck_assert_int_eq(pValue->ivalue, 1);
        else if (strcmp(pKey, "test2") == 0)
            ck_assert_int_eq(pValue->ivalue, 2);
        else if (strcmp(pKey, "test3") == 0)
            ck_assert_int_eq(pValue->ivalue, 3);
        else
            ck_assert_msg(1 == 0, "cannot find the key");
    }

    dictiter_free(pIter);
    dict_free(pdict);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("dictiter");
    TCase *tc_dictiter = tcase_create("dictiter");
    tcase_add_test(tc_dictiter, test_dictiter_create);
    tcase_add_test(tc_dictiter, test_dictiter_move);
    suite_add_tcase(s, tc_dictiter);
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
