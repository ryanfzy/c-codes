#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "base64.h"

START_TEST(test_encode1)
{
    char *msg = "Man";
    char b64[32] = {0};
    base64_encode(msg, 3, b64, 32);
    ck_assert_msg(strcmp(b64, "TWFu") == 0, "test1 is wrong.");
}
END_TEST

START_TEST(test_encode2)
{
    char *msg = "Ma";
    char b64[32] = {0};
    base64_encode(msg, 2, b64, 32);
    ck_assert_msg(strcmp(b64, "TWE=") == 0, "test2 is wrong.");
}
END_TEST

START_TEST(test_encode3)
{
    char *msg = "M";
    char b64[32] = {0};
    base64_encode(msg, 1, b64, 32);
    ck_assert_msg(strcmp(b64, "TQ==") == 0, "test3 is wrong.");
}
END_TEST

START_TEST(test_encode4)
{
    char *msg = "People";
    char b64[32] = {0};
    base64_encode(msg, 6, b64, 32);
    ck_assert_msg(strcmp(b64, "UGVvcGxl") == 0, "test4 is wrong.");
}
END_TEST

START_TEST(test_encode5)
{
    char *msg = "Peopl";
    char b64[32] = {0};
    base64_encode(msg, 5, b64, 32);
    ck_assert_msg(strcmp(b64, "UGVvcGw=") == 0, "test5 is wrong.");
}
END_TEST

START_TEST(test_encode6)
{
    char *msg = "Peop";
    char b64[32] = {0};
    base64_encode(msg, 4, b64, 32);
    ck_assert_msg(strcmp(b64, "UGVvcA==") == 0, "test6 is wrong.");
}
END_TEST

START_TEST(test_decode1)
{
    char *b64 = "TWFu";
    char msg[32] = {0};
    base64_decode(b64, 4, msg, 32);
    ck_assert_msg(strcmp(msg, "Man") == 0, "test decode 1 is wrong.");
}
END_TEST

START_TEST(test_decode2)
{
    char *b64 = "TWE=";
    char msg[32] = {0};
    base64_decode(b64, 4, msg, 32);
    ck_assert_msg(strcmp(msg, "Ma") == 0, "test decode 2 is wrong.");
}
END_TEST

START_TEST(test_decode3)
{
    char *b64 = "TQ==";
    char msg[32] = {0};
    base64_decode(b64, 4, msg, 32);
    ck_assert_msg(strcmp(msg, "M") == 0, "test decode 3 is wrong.");
}
END_TEST

START_TEST(test_decode4)
{
    char *b64 = "UGVvcGxl";
    char msg[32] = {0};
    base64_decode(b64, 8, msg, 32);
    ck_assert_msg(strcmp(msg, "People") == 0, "test decode 4 is wrong.");
}
END_TEST

START_TEST(test_decode5)
{
    char *b64 = "UGVvcGw=";
    char msg[32] = {0};
    base64_decode(b64, 8, msg, 32);
    ck_assert_msg(strcmp(msg, "Peopl") == 0, "test decode 5 is wrong.");
}
END_TEST

START_TEST(test_decode6)
{
    char *b64 = "UGVvcA==";
    char msg[32] = {0};
    base64_decode(b64, 8, msg, 32);
    ck_assert_msg(strcmp(msg, "Peop") == 0, "test decode 6 is wrong.");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("base64");
    TCase *tc_base64 = tcase_create("base64");
    tcase_add_test(tc_base64, test_encode1);
    tcase_add_test(tc_base64, test_encode2);
    tcase_add_test(tc_base64, test_encode3);
    tcase_add_test(tc_base64, test_encode4);
    tcase_add_test(tc_base64, test_encode5);
    tcase_add_test(tc_base64, test_encode6);
    tcase_add_test(tc_base64, test_decode1);
    tcase_add_test(tc_base64, test_decode2);
    tcase_add_test(tc_base64, test_decode3);
    tcase_add_test(tc_base64, test_decode4);
    tcase_add_test(tc_base64, test_decode5);
    tcase_add_test(tc_base64, test_decode6);
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
