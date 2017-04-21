#include <stdlib.h>
#include <check.h>
#include "dict.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_dict_create1)
{
    TestData t;
    t.ivalue = 1;
    Dict *pdict = dict_create("key1", (char*)&t, sizeof(TestData));
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("dict");
    TCase *tc_dict = tcase_create("dict");
    tcase_add_test(tc_dict, test_dict_create1);
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
