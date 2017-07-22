#include <stdlib.h>
#include <check.h>
#include "number.h"

START_TEST(test_int2str)
{
    int iNum = 123456;
    char szStr[128] = {0};
    int2str(iNum, 16, szStr, 128);
    ck_assert_msg(strcmp(szStr, "1e240") == 0, "int2str is not working");

    char *pStr = "1e240";
    int iNum2 = str2int(pStr, strlen(pStr), 16);
    ck_assert_int_eq(iNum2, 123456);
}
END_TEST

START_TEST(test_addbinary)
{
    char *szB1 = "10011010010";
    char *szB2 = "1011000101110";
    char szRet[32] = {0};
    bin_add_ex(szB1, strlen(szB1), szB2, strlen(szB2), szRet, 128);
    ck_assert_msg(strcmp(szRet, "1101100000000") == 0, "add result is wrong");
}
END_TEST

START_TEST(test_bin_mul)
{
    char *szB1 = "1111011";
    char *szB2 = "1101";
    char szRet[32] = {0};
    bin_mul_ex(szB1, strlen(szB1), szB2, strlen(szB2), szRet, 32);
    ck_assert_msg(strcmp(szRet, "11000111111") == 0, "mul result is wrong");
}
END_TEST

START_TEST(test_bin_sub)
{
    char *szB1 = "00111000";
    char *szB2 = "00101011";
    char szRet[32] = {0};
    bin_sub_ex(szB1, strlen(szB1), szB2, strlen(szB2), szRet, 32);
    ck_assert_msg(strcmp(szRet, "1101") == 0, "sub result is wrong");
}
END_TEST

START_TEST(test_bin_div)
{
    char *szB1 = "11000111111";
    char *szB2 = "1111011";
    char szRet[32] = {0};
    bin_div_ex(szB1, strlen(szB1), szB2, strlen(szB2), szRet, 32);
    printf("%s\n", szRet);
    ck_assert_msg(strcmp(szRet, "1101") == 0, "div result is wrong");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("number");
    TCase *tc_stack = tcase_create("number");
    tcase_add_test(tc_stack, test_int2str);
    tcase_add_test(tc_stack, test_addbinary);
    tcase_add_test(tc_stack, test_bin_mul);
    tcase_add_test(tc_stack, test_bin_sub);
    tcase_add_test(tc_stack, test_bin_div);
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
