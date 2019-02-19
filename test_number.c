#include <stdlib.h>
#include <check.h>
#include "number.h"
#include "binary.h"

/*
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
*/

START_TEST(test_bin_add)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x4d2");  // 1234
    Bin b2 = bin_create("x162e"); // 5678
    Bin ret = bin_add(b1, b2);
    bin2xstr(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "0000000000000000000000000000000000000000000000000000000000001b00") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
}
END_TEST

START_TEST(test_bin_add_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2"); // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin b2 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad3"); // 1234567890123456789012345678901234567890123456789012345678901234567891
    Bin ret = bin_add(b1, b2);
    bin2xstr(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "0000005b95d8985be84d126ecc8873745e2c5f85aed33197599fe32d9c7e15a5") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
}
END_TEST

START_TEST(test_bin_mul)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x7b");  // 123
    Bin b2 = bin_create("xd");   // 13
    Bin br = bin_mul(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "000000000000000000000000000000000000000000000000000000000000063f") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_mul_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2");   // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin b2 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2");   // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin br = bin_mul(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "526149ba9e864644df1a6622d80ee7e4a714e5cccc1c12b2ba0597084bd11444") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_sub)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x38");  // 56
    Bin b2 = bin_create("x2b");  // 43
    Bin br = bin_sub(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "000000000000000000000000000000000000000000000000000000000000000d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_sub_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x16e5762997174f7c6156ef3d489860784fa5870d95ed51e67d751c67eea");  // 9876543210987654321098765432109876543210987654321098765432109876543210
    Bin b2 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2");   // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin br = bin_sub(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "000001408c764d4380d13cde08af9acf56f1553781074093284e764083877418") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_div)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x63f");  // 1599
    Bin b2 = bin_create("x7b");   // 123
    Bin br = bin_div(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "000000000000000000000000000000000000000000000000000000000000000d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_div_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x136ccc118300207d2e6cfe0022e5d56a89116ec6de5d5f3ff4");  // 123456789012345678901234567890 * 987654321098765432109876543210
    Bin b2 = bin_create("x18ee90ff6c373e0ee4e3f0ad2");   // 123456789012345678901234567890
    Bin br = bin_div(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "000000000000000000000000000000000000000c7748819dffb62438d1c67eea") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_mod)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x9ab5");  // 39605
    Bin b2 = bin_create("x7b");    // 123
    Bin br = bin_mod(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "000000000000000000000000000000000000000000000000000000000000007a") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

START_TEST(test_bin_mod_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x136ccc118300207d2e6cfe002474be7a7fd4e2a7ccab9e4ac5");  // 123456789012345678901234567890 * 987654321098765432109876543210 + 123456789012345678901234567889
    Bin b2 = bin_create("x18ee90ff6c373e0ee4e3f0ad2");   // 123456789012345678901234567890
    Bin br = bin_mod(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "00000000000000000000000000000000000000018ee90ff6c373e0ee4e3f0ad1") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
}
END_TEST

/*
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
    //tcase_add_test(tc_stack, test_int2str);
    tcase_add_test(tc_stack, test_bin_add);
    tcase_add_test(tc_stack, test_bin_add_big);
    tcase_add_test(tc_stack, test_bin_mul);
    tcase_add_test(tc_stack, test_bin_mul_big);
    tcase_add_test(tc_stack, test_bin_sub);
    tcase_add_test(tc_stack, test_bin_sub_big);
    tcase_add_test(tc_stack, test_bin_div);
    tcase_add_test(tc_stack, test_bin_div_big);
    tcase_add_test(tc_stack, test_bin_mod);
    tcase_add_test(tc_stack, test_bin_mod_big);
    //tcase_add_test(tc_stack, test_bin_float2bin);
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
