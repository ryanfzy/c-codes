#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "sha256.h"
#include "binary.h"
#include "ecc.h"
#include "mem_d.h"

START_TEST(test_add1)
{
    Ecc ecc = ecc_create_uint(2, 2);
    Bin x1 = bin_create_uint(13);
    Bin y1 = bin_create_uint(7);
    Bin x2 = bin_create_uint(16);
    Bin y2 = bin_create_uint(4);
    Bin mod = bin_create_uint(17);
    Bin rx = 0;
    Bin ry = 0;
    ecc_add(ecc, x1, y1, x2, y2, mod, &rx, &ry);
    char szRet[66] = {0};
    bin2xstr2(rx, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+6") == 0, "x is wrong.");
    bin2xstr2(ry, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+3") == 0, "y is wrong.");
    bin_free(x1);
    bin_free(y1);
    bin_free(x2);
    bin_free(y2);
    bin_free(mod);
    bin_free(rx);
    bin_free(ry);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_add2)
{
    Ecc ecc = ecc_create_uint(2, 2);
    Bin x1 = bin_create_uint(13);
    Bin y1 = bin_create_uint(10);
    Bin x2 = bin_create_uint(10);
    Bin y2 = bin_create_uint(11);
    Bin mod = bin_create_uint(17);
    Bin rx = 0;
    Bin ry = 0;
    ecc_add(ecc, x1, y1, x2, y2, mod, &rx, &ry);
    char szRet[66] = {0};
    bin2xstr2(rx, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+d") == 0, "x is wrong.");
    bin2xstr2(ry, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+7") == 0, "y is wrong.");
    bin_free(x1);
    bin_free(y1);
    bin_free(x2);
    bin_free(y2);
    bin_free(mod);
    bin_free(rx);
    bin_free(ry);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

/*
START_TEST(test_add3)
{
    Ecc ecc = ecc_create_uint(0, 7);
    Bin x = bin_create("xc6047f9441ed7d6d3045406e95c07cd85c778e4b8cef3ca7abac09b95c709ee5");
    Bin y = bin_create("x1ae168fea63dc339a3c58419466ceaeef7f632653266d0e1236431a950cfe52a");
    Bin mod = bin_create("xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    
    Bin rx = 0;
    Bin ry = 0;
    ecc_double(ecc, x, y, mod, &rx, &ry);
    char szRet[66] = {0};
    bin2xstr2(rx, szRet, 65);
    printf("%s\n", szRet);
    //ck_assert_msg(strcmp(szRet, "+d") == 0, "x is wrong.");
    bin2xstr2(ry, szRet, 65);
    printf("%s\n", szRet);
    ck_assert_msg(strcmp(szRet, "+7") == 0, "y is wrong.");
    bin_free(x);
    bin_free(y);
    bin_free(mod);
    bin_free(rx);
    bin_free(ry);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST
*/

START_TEST(test_double)
{
    Ecc ecc = ecc_create_uint(2, 2);
    Bin x = bin_create_uint(13);
    Bin y = bin_create_uint(7);
    Bin mod = bin_create_uint(17);
    Bin rx = 0;
    Bin ry = 0;
    ecc_double(ecc, x, y, mod, &rx, &ry);
    char szRet[66] = {0};
    bin2xstr2(rx, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+a") == 0, "x is wrong.");
    bin2xstr2(ry, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+b") == 0, "y is wrong.");
    bin_free(x);
    bin_free(y);
    bin_free(mod);
    bin_free(rx);
    bin_free(ry);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_ecc_create)
{
    Ecc ecc = ecc_create_uint(2, 2);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_mul)
{
    Ecc ecc = ecc_create_uint(2, 2);
    Bin x = bin_create_uint(5);
    Bin y = bin_create_uint(1);
    Bin mod = bin_create_uint(17);
    Bin n = bin_create_uint(123);
    Bin rx = 0;
    Bin ry = 0;
    ecc_mul(ecc, x, y, n, mod, &rx, &ry);
    char szRet[66] = {0};
    bin2xstr2(rx, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+7") == 0, "x is wrong.");
    bin2xstr2(ry, szRet, 65);
    ck_assert_msg(strcmp(szRet, "+6") == 0, "y is wrong.");
    bin_free(x);
    bin_free(y);
    bin_free(n);
    bin_free(mod);
    bin_free(rx);
    bin_free(ry);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

/*
START_TEST(test_mul_big)
{
    Ecc ecc = ecc_create_uint(0, 7);
    //Bin x = bin_create("xc6047f9441ed7d6d3045406e95c07cd85c778e4b8cef3ca7abac09b95c709ee5");
    Bin x = bin_create("x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
    Bin y = bin_create("x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");
    //Bin y = bin_create("x1ae168fea63dc339a3c58419466ceaeef7f632653266d0e1236431a950cfe52a");
    Bin mod = bin_create("xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    Bin n = bin_create_uint(2);
    Bin rx = 0;
    Bin ry = 0;
    ecc_mul(ecc, x, y, n, mod, &rx, &ry);
    char szRet[66] = {0};
    bin2xstr2(rx, szRet, 65);
    printf("x:%s\n", szRet);
    //ck_assert_msg(strcmp(szRet, "+7") == 0, "x is wrong.");
    bin2xstr2(ry, szRet, 65);
    printf("y:%s\n", szRet);
    ck_assert_msg(strcmp(szRet, "+6") == 0, "y is wrong.");
    bin_free(x);
    bin_free(y);
    bin_free(n);
    bin_free(mod);
    bin_free(rx);
    bin_free(ry);
    ecc_free(ecc);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST
*/

Suite* make_add_suit(void)
{
    Suite *s = suite_create("ecc");
    TCase *tc_stack = tcase_create("ecc");
    // this is slow, before make it fast, don't run it
    //tcase_add_test(tc_stack, test_sign);
    tcase_add_test(tc_stack, test_ecc_create);
    tcase_add_test(tc_stack, test_add1);
    tcase_add_test(tc_stack, test_add2);
    tcase_add_test(tc_stack, test_double);
    tcase_add_test(tc_stack, test_mul);
    //tcase_add_test(tc_stack, test_mul_big);
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
