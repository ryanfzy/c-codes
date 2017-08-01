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

START_TEST(test_addbinary)
{
    char *szB1 = "10011010010";
    char *szB2 = "1011000101110";
    char szRet[33] = {0};

    Bin bin1, bin2, binRet;
    bin_init_bstr(&bin1, szB1, strlen(szB1));
    bin_init_bstr(&bin2, szB2, strlen(szB2));
    bin_add(&bin1, &bin2, &binRet);
    bin2bstr(&binRet, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000001101100000000") == 0, "add result is wrong");

    szB1 = "11";
    szB2 = "00";
    bin_init_bstr(&bin1, szB1, strlen(szB1));
    bin_init_bstr(&bin2, szB2, strlen(szB2));
    bin_add(&bin1, &bin2, &binRet);
    bin2bstr(&binRet, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000000011") == 0, "add result is wrong");

    Bin32 b1 = {{0x00,0x00,0x04,0xd2}};
    Bin32 b2 = {{0x00,0x00,0x16,0x2e}};
    Bin32 bRet = {{0x00,0x00,0x00,0x00}};
    bin32_add(&b1, &b2, &bRet);

    char szBin32a[33] = {0};
    char szBin32b[33] = {0};
    bin322bstr(&b1, szBin32a, 32);
    bin322bstr(&b2, szBin32b, 32);
    bin322bstr(&bRet, szRet, 32);
    printf(" b1:%s\n", szBin32a);
    printf(" b2:%s\n", szBin32b);
    printf("add:%s\n", szRet);

    ck_assert_msg(strcmp(szRet, "00000000000000000001101100000000") == 0, "bin32 add result is wrong");
}
END_TEST

START_TEST(test_bin_mul)
{
    char *szB1 = "1111011";
    char *szB2 = "1101";
    char szRet[33] = {0};

    Bin bin1, bin2, binRet;
    bin_init_bstr(&bin1, szB1, strlen(szB1));
    bin_init_bstr(&bin2, szB2, strlen(szB2));
    bin_mul(&bin1, &bin2, &binRet);
    bin2bstr(&binRet, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000011000111111") == 0, "mul result is wrong");
}
END_TEST

START_TEST(test_bin_sub)
{
    char *szB1 = "00111000";
    char *szB2 = "00101011";
    char szRet[33] = {0};

    Bin bin1, bin2, binRet;
    bin_init_bstr(&bin1, szB1, strlen(szB1));
    bin_init_bstr(&bin2, szB2, strlen(szB2));
    bin_sub(&bin1, &bin2, &binRet);
    bin2bstr(&binRet, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000001101") == 0, "sub result is wrong");
}
END_TEST

START_TEST(test_bin_div)
{
    char *szB1 = "11000111111";
    char *szB2 = "1111011";
    char szRet[33] = {0};

    Bin bin1, bin2, binRet;
    bin_init_bstr(&bin1, szB1, strlen(szB1));
    bin_init_bstr(&bin2, szB2, strlen(szB2));
    bin_div(&bin1, &bin2, &binRet);
    bin2bstr(&binRet, szRet, 32);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000001101") == 0, "div result is wrong");
}
END_TEST

START_TEST(test_bin_int2bin)
{
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
}
END_TEST

START_TEST(test_bin32_init)
{
    Bin32 bin32;
    bin32_init(&bin32);
    for (int i = 0; i < CHAR_NUM; i++)
        ck_assert_int_eq(bin32.cBin[i], 0x00);
}
END_TEST

START_TEST(test_bin32_lshift)
{
    Bin32 bin32 = {{0xff,0xff,0xff,0xff}};
    bin32_lshift(&bin32, 11);
    ck_assert_int_eq(bin32.cBin[0], 0xff);
    ck_assert_int_eq(bin32.cBin[1], 0xff);
    ck_assert_int_eq(bin32.cBin[2], 0xf8);
    ck_assert_int_eq(bin32.cBin[3], 0x00);

    Bin32 bin2 = {{0xff,0xff,0xff,0xff}};
    bin32_lshift(&bin2, 3);
    ck_assert_int_eq(bin2.cBin[0], 0xff);
    ck_assert_int_eq(bin2.cBin[1], 0xff);
    ck_assert_int_eq(bin2.cBin[2], 0xff);
    ck_assert_int_eq(bin2.cBin[3], 0xf8);

    Bin32 bin3 = {{0xff,0xff,0xff,0xff}};
    bin32_lshift(&bin3, 24);
    ck_assert_int_eq(bin3.cBin[0], 0xff);
    ck_assert_int_eq(bin3.cBin[1], 0x00);
    ck_assert_int_eq(bin3.cBin[2], 0x00);
    ck_assert_int_eq(bin3.cBin[3], 0x00);
}
END_TEST

START_TEST(test_bin32_2bstr)
{
    Bin32 bin32 = {{0xff,0xff,0xff,0xff}};
    char szBin[33] = {0};
    bin322bstr(&bin32, szBin, 32);
    ck_assert_msg(strcmp(szBin, "11111111111111111111111111111111") == 0, "bin32 to bstr is wrong");

    Bin32 bin2 = {{0x00,0xff,0x00,0xff}};
    bin322bstr(&bin2, szBin, 32);
    ck_assert_msg(strcmp(szBin, "00000000111111110000000011111111") == 0, "bin2 to bstr is wrong");

    Bin32 bin3 = {{0xff,0xff,0xff,0xff}};
    bin32_lshift(&bin3, 11);
    bin322bstr(&bin3, szBin, 32);
    ck_assert_msg(strcmp(szBin, "11111111111111111111100000000000") == 0, "bin3 to bstr is wrong");
}
END_TEST

START_TEST(test_bin32_rshift)
{
    Bin32 bin32 = {{0xff,0xff,0xff,0xff}};
    bin32_rshift(&bin32, 11);
    ck_assert_int_eq(bin32.cBin[0], 0x00);
    ck_assert_int_eq(bin32.cBin[1], 0x1f);
    ck_assert_int_eq(bin32.cBin[2], 0xff);
    ck_assert_int_eq(bin32.cBin[3], 0xff);

    Bin32 bin2 = {{0xff,0xff,0xff,0xff}};
    bin32_rshift(&bin2, 3);
    ck_assert_int_eq(bin2.cBin[0], 0x1f);
    ck_assert_int_eq(bin2.cBin[1], 0xff);
    ck_assert_int_eq(bin2.cBin[2], 0xff);
    ck_assert_int_eq(bin2.cBin[3], 0xff);

    Bin32 bin3 = {{0xff,0xff,0xff,0xff}};
    bin32_rshift(&bin3, 24);
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
    tcase_add_test(tc_stack, test_addbinary);
    tcase_add_test(tc_stack, test_bin_mul);
    tcase_add_test(tc_stack, test_bin_sub);
    tcase_add_test(tc_stack, test_bin_div);
    tcase_add_test(tc_stack, test_bin_int2bin);
    tcase_add_test(tc_stack, test_bin_float2bin);
    tcase_add_test(tc_stack, test_bin32_init);
    tcase_add_test(tc_stack, test_bin32_lshift);
    tcase_add_test(tc_stack, test_bin32_2bstr);
    tcase_add_test(tc_stack, test_bin32_rshift);
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
