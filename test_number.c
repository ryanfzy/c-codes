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
    Bin b1 = {{0x00,0x00,0x04,0xd2}}; //10011010010
    Bin b2 = {{0x00,0x00,0x16,0x2e}}; //1011000101110
    Bin bRet = {{0x00,0x00,0x00,0x00}};
    bin_add(&b1, &b2, &bRet);
    bin2bstr(&bRet, szRet, 32);

    ck_assert_msg(strcmp(szRet, "00000000000000000001101100000000") == 0, "bin add result is wrong");
}
END_TEST

START_TEST(test_bin_mul)
{
    char szRet[33] = {0};
    Bin b1 = {{0x00,0x00,0x00,0x7b}}; //1111011
    Bin b2 = {{0x00,0x00,0x00,0x0d}}; //1101
    Bin br = {{0x00,0x00,0x00,0x00}};
    bin_mul(&b1, &b2, &br);
    bin2bstr(&br, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000011000111111") == 0, "mul result is wrong");
}
END_TEST

START_TEST(test_bin_sub)
{
    char szRet[33] = {0};
    Bin b1 = {{0x00,0x00,0x00,0x38}}; //00111000
    Bin b2 = {{0x00,0x00,0x00,0x2b}}; //00101011
    Bin br = {{0x00,0x00,0x00,0x00}};
    bin_sub(&b1, &b2, &br);
    bin2bstr(&br, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000001101") == 0, "sub result is wrong");
}
END_TEST

START_TEST(test_bin_div)
{
    char szRet[33] = {0};
    Bin b1 = {{0x00,0x00,0x06,0x3f}}; //11000111111
    Bin b2 = {{0x00,0x00,0x00,0x7b}}; //1111011
    Bin br = {{0x00,0x00,0x00,0x00}};
    bin_div(&b1, &b2, &br);
    bin2bstr(&br, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000001101") == 0, "div result is wrong");
}
END_TEST

START_TEST(test_bin_int2bin)
{
    /*
    char *szInt = "123";
    char szRet[33] = {0};

    Bin bin;
    bin_init_istr(&bin, szInt, strlen(szInt));
    bin2bstr(&bin, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000001111011") == 0, "int to bin is wrong");
    */

    char *szInt = "123";
    char szRet[33] = {0};

    Bin bin;
    bin_init_istr(&bin, szInt, strlen(szInt));
    bin2bstr(&bin, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000001111011") == 0, "int to bin is wrong");
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

START_TEST(test_bin_init)
{
    Bin bin;
    bin_init(&bin);
    for (int i = 0; i < CHAR_NUM; i++)
        ck_assert_int_eq(bin.cBin[i], 0x00);
}
END_TEST

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

Suite* make_add_suit(void)
{
    Suite *s = suite_create("number");
    TCase *tc_stack = tcase_create("number");
    tcase_add_test(tc_stack, test_int2str);
    tcase_add_test(tc_stack, test_bin_add);
    tcase_add_test(tc_stack, test_bin_mul);
    tcase_add_test(tc_stack, test_bin_sub);
    tcase_add_test(tc_stack, test_bin_div);
    tcase_add_test(tc_stack, test_bin_int2bin);
    tcase_add_test(tc_stack, test_bin_float2bin);
    tcase_add_test(tc_stack, test_bin_init);
    tcase_add_test(tc_stack, test_bin_lshift);
    tcase_add_test(tc_stack, test_bin_2bstr);
    tcase_add_test(tc_stack, test_bin_rshift);
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
