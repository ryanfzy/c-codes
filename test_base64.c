#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "base64.h"

START_TEST(test1)
{
    char *msg = "Man";
    char b64[32] = {0};
    base64_encode(msg, 3, b64, 32);
    ck_assert_msg(strcmp(b64, "TWFu") == 0, "test1 is wrong.");
}
END_TEST

START_TEST(test2)
{
    char *msg = "Ma";
    char b64[32] = {0};
    base64_encode(msg, 2, b64, 32);
    ck_assert_msg(strcmp(b64, "TWE=") == 0, "test2 is wrong.");
}
END_TEST

START_TEST(test3)
{
    char *msg = "M";
    char b64[32] = {0};
    base64_encode(msg, 1, b64, 32);
    ck_assert_msg(strcmp(b64, "TQ==") == 0, "test3 is wrong.");
}
END_TEST

START_TEST(test4)
{
    char *msg = "People";
    char b64[32] = {0};
    base64_encode(msg, 6, b64, 32);
    ck_assert_msg(strcmp(b64, "UGVvcGxl") == 0, "test4 is wrong.");
}
END_TEST

START_TEST(test5)
{
    char *msg = "Peopl";
    char b64[32] = {0};
    base64_encode(msg, 5, b64, 32);
    ck_assert_msg(strcmp(b64, "UGVvcGw=") == 0, "test5 is wrong.");
}
END_TEST

START_TEST(test6)
{
    char *msg = "Peop";
    char b64[32] = {0};
    base64_encode(msg, 4, b64, 32);
    ck_assert_msg(strcmp(b64, "UGVvcA==") == 0, "test6 is wrong.");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("base64");
    TCase *tc_base64 = tcase_create("base64");
    tcase_add_test(tc_base64, test1);
    tcase_add_test(tc_base64, test2);
    tcase_add_test(tc_base64, test3);
    tcase_add_test(tc_base64, test4);
    tcase_add_test(tc_base64, test5);
    tcase_add_test(tc_base64, test6);
    suite_add_tcase(s, tc_base64);
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
