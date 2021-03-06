#include <stdlib.h>
#include <check.h>
#include <limits.h>
#include "binary.h"
#include "mem_d.h"

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
    ck_assert_msg(strcmp(szRet, "1b00") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_add_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x4d2");  // 1234
    Bin b2 = bin_create("-x162e"); // 5678
    Bin ret = bin_add(b1, b2);
    bin2xstr2(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-115c") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_add)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x4d2");  // 1234
    Bin b2 = bin_create("x162e"); // 5678
    Bin ret = bin_add(b1, b2);
    bin2xstr2(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+115c") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_add_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x4d2");  // 1234
    Bin b2 = bin_create("-x162e"); // 5678
    Bin ret = bin_add(b1, b2);
    bin2xstr2(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-1b00") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_add_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2"); // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin b2 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad3"); // 1234567890123456789012345678901234567890123456789012345678901234567891
    Bin ret = bin_add(b1, b2);
    bin2xstr(ret, szRet, 64);
    ck_assert_msg(strcmp(szRet, "5b95d8985be84d126ecc8873745e2c5f85aed33197599fe32d9c7e15a5") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_add_big_big)
{
    char szRet[82] = {0};
    Bin b1 = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    Bin b2 = bin_create("xfedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321");
    Bin ret = bin_add(b1, b2);
    bin2xstr(ret, szRet, 81);
    ck_assert_msg(strcmp(szRet, "111111082181111111111108218111111111110821811111111111082181111111111108218111110") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_add_big_big_to_big)
{
    char szRet[81] = {0};
    Bin b1 = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    Bin b2 = bin_create("xfedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321");
    Bin ret = bin_add(b1, b2);
    bin2xstr(ret, szRet, 80);
    ck_assert_msg(strcmp(szRet, "1234567890abcdf01111108218111111111110821811111111111082181111111111108218111110") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_add_big_to_big_big)
{
    char szRet[81] = {0};
    Bin b1 = bin_create("xfedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321");
    Bin b2 = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    Bin ret = bin_add(b1, b2);
    bin2xstr(ret, szRet, 80);
    ck_assert_msg(strcmp(szRet, "1234567890abcdf01111108218111111111110821811111111111082181111111111108218111110") == 0, "bin add result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(ret);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mul)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x7b");  // 123
    Bin b2 = bin_create("xd");   // 13
    Bin br = bin_mul(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "63f") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mul_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x7b");  // 123
    Bin b2 = bin_create("-xd");   // 13
    Bin br = bin_mul(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-63f") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_mul)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x7b");  // 123
    Bin b2 = bin_create("xd");   // 13
    Bin br = bin_mul(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-63f") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_mul_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x7b");  // 123
    Bin b2 = bin_create("-xd");   // 13
    Bin br = bin_mul(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+63f") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mul_big)
{
    char szRet[128] = {0};
    Bin b1 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2");   // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin b2 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2");   // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin br = bin_mul(b1, b2);
    bin2xstr(br, szRet, 127);
    ck_assert_msg(strcmp(szRet, "830f7ec8a7ce63e8648ff6321e9e69637ef2d74cc0afbc5cf00526149ba9e864644df1a6622d80ee7e4a714e5cccc1c12b2ba0597084bd11444") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mul_big2)
{
    char szRet[256] = {0};
    Bin b1 = bin_create_uint(3);
    Bin b2 = bin_create("x39e58a8055b6fb264b75ec8c646509784204ac15a8c24e05babc9729ab9b055c3a9458e4ce3289560a38e08ba8175a9446ce14e608245ab3a9978a8bd8acaa40");
    bin_mul2(b1, b2, &b2);
    bin2xstr(b2, szRet, 255);
    ck_assert_msg(strcmp(szRet, "adb09f810124f172e261c5a52d2f1c68c60e0440fa46ea113035c57d02d11014afbd0aae6a979c021eaaa1a2f8460fbcd46a3eb2186d101afcc69fa38a05fec0") == 0, "mul result is wrong");
    bin_free(b1);
    bin_free(b2);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_sub)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x38");  // 56
    Bin b2 = bin_create("x2b");  // 43
    Bin br = bin_sub(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_sub_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x38");  // 56
    Bin b2 = bin_create("-x2b");  // 43
    Bin br = bin_sub(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+63") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_sub)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x38");  // 56
    Bin b2 = bin_create("x2b");  // 43
    Bin br = bin_sub(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-63") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_sub_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x38");  // 56
    Bin b2 = bin_create("-x2b");  // 43
    Bin br = bin_sub(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_zero_sub)
{
    char szRet[65] = {0};
    Bin b1 = bin_create_uint(0);
    Bin b2 = bin_create_uint(1);
    Bin br = bin_sub(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-1") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_sub_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x16e5762997174f7c6156ef3d489860784fa5870d95ed51e67d751c67eea");  // 9876543210987654321098765432109876543210987654321098765432109876543210
    Bin b2 = bin_create("x2dcaec4c2df4268937664439ba2f162fc2d76998cbaccff196ce3f0ad2");   // 1234567890123456789012345678901234567890123456789012345678901234567890
    Bin br = bin_sub(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "1408c764d4380d13cde08af9acf56f1553781074093284e764083877418") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_sub_big_big)
{
    char szRet[81] = {0};
    Bin b1 = bin_create("xfedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321");
    Bin b2 = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    Bin br = bin_sub(b1, b2);
    bin2xstr(br, szRet, 80);
    ck_assert_msg(strcmp(szRet, "eca86390f6b97532eca86390f6b97532eca86390f6b97532eca86390f6b97532eca86390f6b97532") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_sub_big_big_to_big)
{
    char szRet[81] = {0};
    Bin b1 = bin_create("xfedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321fedcba0987654321");
    Bin b2 = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    Bin br = bin_sub(b1, b2);
    bin2xstr(br, szRet, 80);
    ck_assert_msg(strcmp(szRet, "fedcba0987654321eca86390f6b97532eca86390f6b97532eca86390f6b97532eca86390f6b97532") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_div)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x63f");  // 1599
    Bin b2 = bin_create("x7b");   // 123
    Bin br = bin_div(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_div_rem)
{
    char szRet[65] = {0};
    Bin b1 = bin_create_uint(17);
    Bin b2 = bin_create_uint(14);
    Bin rem = 0;
    Bin br = bin_div_rem(b1, b2, &rem);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "1") == 0, "div result is wrong");
    bin2xstr(rem, szRet, 64);
    ck_assert_msg(strcmp(szRet, "3") == 0, "div remainder is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    bin_free(rem);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_div_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x63f");  // 1599
    Bin b2 = bin_create("-x7b");   // 123
    Bin br = bin_div(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_div)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x63f");  // 1599
    Bin b2 = bin_create("x7b");   // 123
    Bin br = bin_div(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "-d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_neg_div_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x63f");  // 1599
    Bin b2 = bin_create("-x7b");   // 123
    Bin br = bin_div(b1, b2);
    bin2xstr2(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "+d") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_div_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x136ccc118300207d2e6cfe0022e5d56a89116ec6de5d5f3ff4");  // 123456789012345678901234567890 * 987654321098765432109876543210
    Bin b2 = bin_create("x18ee90ff6c373e0ee4e3f0ad2");   // 123456789012345678901234567890
    Bin br = bin_div(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "c7748819dffb62438d1c67eea") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mod)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x9ab5");  // 39605
    Bin b2 = bin_create("x7b");    // 123
    Bin br = bin_mod(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "7a") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mod_neg)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("-x1c");  // -28
    Bin b2 = bin_create("x11");    // 17
    Bin br = bin_mod(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "6") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mod_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x136ccc118300207d2e6cfe002474be7a7fd4e2a7ccab9e4ac5");  // 123456789012345678901234567890 * 987654321098765432109876543210 + 123456789012345678901234567889
    Bin b2 = bin_create("x18ee90ff6c373e0ee4e3f0ad2");   // 123456789012345678901234567890
    Bin br = bin_mod(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "18ee90ff6c373e0ee4e3f0ad1") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_mod_big_big)
{
    char szRet[65] = {0};
    Bin b1 = bin_create("x39e58a8055b6fb264b75ec8c646509784204ac15a8c24e05babc9729ab9b055c3a9458e4ce3289560a38e08ba8175a9446ce14e608245ab3a9978a8bd8acaa40");
    Bin b2 = bin_create("xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    Bin br = bin_mod(b1, b2);
    bin2xstr(br, szRet, 64);
    ck_assert_msg(strcmp(szRet, "8550e7d238fcf3086ba9adcf0fb52a9de3652194d06cb5bb38d50229b854fc49") == 0, "sub result is wrong");
    bin_free(b1);
    bin_free(b2);
    bin_free(br);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_create_big)
{
    char szRet[129] = {0};
    Bin pa = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    bin2xstr(pa, szRet, 128);
    ck_assert_msg(strcmp(szRet, "1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef") == 0, "create big is wrong");
    bin_free(pa);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_create_uint1)
{
    char szRet[129] = {0};
    Bin pa = bin_create_uint(5);
    bin2xstr2(pa, szRet, 128);
    ck_assert_msg(strcmp(szRet, "+5") == 0, "create uint 5 is wrong");
    bin_free(pa);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_create_uint2)
{
    char szRet[129] = {0};
    Bin pa = bin_create_uint(123456);
    bin2xstr2(pa, szRet, 128);
    ck_assert_msg(strcmp(szRet, "+1e240") == 0, "create uint 123456 is wrong");
    bin_free(pa);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bin_create_uint_max)
{
    char szRet[129] = {0};
    Bin pa = bin_create_uint(UINT_MAX);
    bin2xstr2(pa, szRet, 128);
    ck_assert_msg(strcmp(szRet, "+ffffffff") == 0, "create max uint is wrong");
    bin_free(pa);
    ck_assert_msg(check_mem(), "memory leak");
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
    tcase_add_test(tc_stack, test_bin_add_neg);
    tcase_add_test(tc_stack, test_bin_neg_add);
    tcase_add_test(tc_stack, test_bin_neg_add_neg);
    tcase_add_test(tc_stack, test_bin_add_big);
    tcase_add_test(tc_stack, test_bin_add_big_big);
    tcase_add_test(tc_stack, test_bin_add_big_big_to_big);
    tcase_add_test(tc_stack, test_bin_add_big_to_big_big);
    tcase_add_test(tc_stack, test_bin_mul);
    tcase_add_test(tc_stack, test_bin_mul_neg);
    tcase_add_test(tc_stack, test_bin_neg_mul);
    tcase_add_test(tc_stack, test_bin_neg_mul_neg);
    tcase_add_test(tc_stack, test_bin_mul_big);
    tcase_add_test(tc_stack, test_bin_mul_big2);
    tcase_add_test(tc_stack, test_bin_sub);
    tcase_add_test(tc_stack, test_bin_sub_neg);
    tcase_add_test(tc_stack, test_bin_neg_sub);
    tcase_add_test(tc_stack, test_bin_neg_sub_neg);
    tcase_add_test(tc_stack, test_bin_zero_sub);
    tcase_add_test(tc_stack, test_bin_sub_big);
    tcase_add_test(tc_stack, test_bin_sub_big_big);
    tcase_add_test(tc_stack, test_bin_sub_big_big_to_big);
    tcase_add_test(tc_stack, test_bin_div);
    tcase_add_test(tc_stack, test_bin_div_rem);
    tcase_add_test(tc_stack, test_bin_div_neg);
    tcase_add_test(tc_stack, test_bin_neg_div);
    tcase_add_test(tc_stack, test_bin_neg_div_neg);
    tcase_add_test(tc_stack, test_bin_div_big);
    tcase_add_test(tc_stack, test_bin_mod);
    tcase_add_test(tc_stack, test_bin_mod_neg);
    tcase_add_test(tc_stack, test_bin_mod_big);
    tcase_add_test(tc_stack, test_bin_mod_big_big);
    tcase_add_test(tc_stack, test_bin_create_big);
    tcase_add_test(tc_stack, test_bin_create_uint1);
    tcase_add_test(tc_stack, test_bin_create_uint2);
    tcase_add_test(tc_stack, test_bin_create_uint_max);
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
