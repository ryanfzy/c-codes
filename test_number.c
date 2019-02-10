#include <stdlib.h>
#include <check.h>
#include "number.h"
#include "binary.h"

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

START_TEST(test_bin_add)
{
    char szRet[33] = {0};
    Bin b1 = bin_create("x4d2", 4); // 1234
    Bin b2 = bin_create("x162e", 5); // 5678
    Bin ret = bin_add(b1, b2);
    bin2bstr(ret, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000001101100000000") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
}
END_TEST

START_TEST(test_bin_mul)
{
    char szRet[33] = {0};
    Bin b1 = bin_create("x7b", 3);  // 123
    Bin b2 = bin_create("xd", 2);   // 13
    Bin br = bin_mul(b1, b2);
    bin2bstr(br, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000011000111111") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_sub)
{
    char szRet[33] = {0};
    Bin b1 = bin_create("x38", 3);  // 56
    Bin b2 = bin_create("x2b", 3);  // 43
    Bin br = bin_sub(b1, b2);
    bin2bstr(br, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000001101") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_div)
{
    char szRet[33] = {0};
    Bin b1 = bin_create("x63f", 4);  // 1599
    Bin b2 = bin_create("x7b", 3);   // 123
    Bin br = bin_div(b1, b2);
    bin2bstr(br, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000001101") == 0, "div result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_float2bin)
{
    /*
    char *szFloat = "10.7";
    char szRet[33] = {0};
    Bin binFloat;
    bin_init_fstr(&binFloat, szFloat, strlen(szFloat));
    bin2bstr(&binFloat, szRet, 33);
    ck_assert_msg(strcmp(szRet, "01000001001010110011001100110011") == 0, "float 2 bin is wrong");

    bin_init_fstr(&binFloat, "26.87", 5);
    bin2bstr(&binFloat, szRet, 33);
    ck_assert_msg(strcmp(szRet, "01000001110101101111010111000011") == 0, "float 2 bin is wrong");

    bin_init_fstr(&binFloat, "0.004", 5);
    bin2bstr(&binFloat, szRet, 33);
    ck_assert_msg(strcmp(szRet, "00111011100000110001001001101111") == 0, "float 2 bin is wrong");
    */
}
END_TEST

/*
START_TEST(test_bin_lshift)
{
    Bin bin = {{0xff,0xff,0xff,0xff}};
    bin_lshift(&bin, 11);
    ck_assert_int_eq(bin.cBin[0], 0xff);
    ck_assert_int_eq(bin.cBin[1], 0xff);
    ck_assert_int_eq(bin.cBin[2], 0xf8);
    ck_assert_int_eq(bin.cBin[3], 0x00);

    Bin bin2 = {{0xff,0xff,0xff,0xff}};
    bin_lshift(&bin2, 3);
    ck_assert_int_eq(bin2.cBin[0], 0xff);
    ck_assert_int_eq(bin2.cBin[1], 0xff);
    ck_assert_int_eq(bin2.cBin[2], 0xff);
    ck_assert_int_eq(bin2.cBin[3], 0xf8);

    Bin bin3 = {{0xff,0xff,0xff,0xff}};
    bin_lshift(&bin3, 24);
    ck_assert_int_eq(bin3.cBin[0], 0xff);
    ck_assert_int_eq(bin3.cBin[1], 0x00);
    ck_assert_int_eq(bin3.cBin[2], 0x00);
    ck_assert_int_eq(bin3.cBin[3], 0x00);
}
END_TEST

START_TEST(test_bin_2bstr)
{
    Bin bin = {{0xff,0xff,0xff,0xff}};
    char szBin[33] = {0};
    bin2bstr(&bin, szBin, 32);
    ck_assert_msg(strcmp(szBin, "11111111111111111111111111111111") == 0, "bin to bstr is wrong");

    Bin bin2 = {{0x00,0xff,0x00,0xff}};
    bin2bstr(&bin2, szBin, 32);
    ck_assert_msg(strcmp(szBin, "00000000111111110000000011111111") == 0, "bin2 to bstr is wrong");

    Bin bin3 = {{0xff,0xff,0xff,0xff}};
    bin_lshift(&bin3, 11);
    bin2bstr(&bin3, szBin, 32);
    ck_assert_msg(strcmp(szBin, "11111111111111111111100000000000") == 0, "bin3 to bstr is wrong");
}
END_TEST

START_TEST(test_bin_rshift)
{
    Bin bin = {{0xff,0xff,0xff,0xff}};
    bin_rshift(&bin, 11);
    ck_assert_int_eq(bin.cBin[0], 0x00);
    ck_assert_int_eq(bin.cBin[1], 0x1f);
    ck_assert_int_eq(bin.cBin[2], 0xff);
    ck_assert_int_eq(bin.cBin[3], 0xff);

    Bin bin2 = {{0xff,0xff,0xff,0xff}};
    bin_rshift(&bin2, 3);
    ck_assert_int_eq(bin2.cBin[0], 0x1f);
    ck_assert_int_eq(bin2.cBin[1], 0xff);
    ck_assert_int_eq(bin2.cBin[2], 0xff);
    ck_assert_int_eq(bin2.cBin[3], 0xff);

    Bin bin3 = {{0xff,0xff,0xff,0xff}};
    bin_rshift(&bin3, 24);
    ck_assert_int_eq(bin3.cBin[0], 0x00);
    ck_assert_int_eq(bin3.cBin[1], 0x00);
    ck_assert_int_eq(bin3.cBin[2], 0x00);
    ck_assert_int_eq(bin3.cBin[3], 0xff);
}
END_TEST
*/

Suite* make_add_suit(void)
{
    Suite *s = suite_create("number");
    TCase *tc_stack = tcase_create("number");
    tcase_add_test(tc_stack, test_int2str);
    tcase_add_test(tc_stack, test_bin_add);
    tcase_add_test(tc_stack, test_bin_mul);
    tcase_add_test(tc_stack, test_bin_sub);
    tcase_add_test(tc_stack, test_bin_div);
    tcase_add_test(tc_stack, test_bin_float2bin);
    //tcase_add_test(tc_stack, test_bin_lshift);
    //tcase_add_test(tc_stack, test_bin_2bstr);
    //tcase_add_test(tc_stack, test_bin_rshift);
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
