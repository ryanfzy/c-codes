#include <stdlib.h>
#include <check.h>
#include "binary.h"
#include "mem_d.h"
#include "bmath.h"

START_TEST(test_gcd1)
{
    char szRet[65] = {0};
    Bin b1 = bin_create_uint(888);
    Bin b2 = bin_create_uint(54);
    Bin ret = bmath_gcd(b1, b2);
    bin2xstr2(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+6") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_gcd2)
{
    char szRet[65] = {0};
    Bin b1 = bin_create_uint(17);
    Bin b2 = bin_create_uint(14);
    Bin ret = bmath_gcd(b1, b2);
    bin2xstr2(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+1") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_inverse_mod)
{
    char szRet[65] = {0};
    Bin a = bin_create_uint(14);
    Bin m = bin_create_uint(17);
    Bin ret = bmath_inverse_mod(a, m);
    bin2xstr2(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+b") == 0, "bin add result is wrong");
    bin_free(a);
    bin_free(m);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_inverse_mod_big)
{
    char szRet[65] = {0};
    Bin a = bin_create("x9075b4ee4d4788cabb49f7f81c221151fa2f68914d0aa833388fa11ff621a970");
    //Bin a = bin_create_uint(14);
    //Bin m = bin_create_uint(17);
    Bin m = bin_create("xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    Bin ret = bmath_inverse_mod(a, m);
    bin2xstr(ret, szRet, 64);
    printf("%s\n", szRet);
    ck_assert_msg(strcmp(szRet, "+b") == 0, "bin add result is wrong");
    bin_free(a);
    bin_free(m);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("math");
    TCase *tc_stack = tcase_create("math");
    /*
    tcase_add_test(tc_stack, test_gcd1);
    tcase_add_test(tc_stack, test_gcd2);
    tcase_add_test(tc_stack, test_inverse_mod);
    */
    tcase_add_test(tc_stack, test_inverse_mod_big);
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
