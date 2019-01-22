#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "sha256.h"

START_TEST(test_short)
{
    char input[] = "abc";
    Hash hash = {{0}};
    sha256(input, 3, &hash);
    for (int i = 0; i < 32; i++)
        printf("%02x", hash.X[i]);
    printf("\n");
    unsigned char expected[32] = {0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad};
    for (int i = 0; i < 32; i++)
        printf("%02x", expected[i]);
    printf("\n");
    for (int i = 0; i < 32; i++)
    {
        if (hash.X[i] != expected[i])
            ck_assert_msg(false, "wrong hash of \"abc\"");
    }
}
END_TEST

START_TEST(test_long)
{
    char pInput[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    Hash hash = {{0}};
    sha256(pInput, sizeof(pInput)-1, &hash);
    for (int i = 0; i < 32; i++)
        printf("%02x", hash.X[i]);
    printf("\n");
    unsigned char expected[32] = {0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8, 0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39, 0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67, 0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1};
    for (int i = 0; i < 32; i++)
        printf("%02x", expected[i]);
    printf("\n");
    for (int i = 0; i < 32; i++)
    {
        if (hash.X[i] != expected[i])
            ck_assert_msg(false, "wrong long hash");
    }
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("sha256");
    TCase *tc_stack = tcase_create("sha256");
    tcase_add_test(tc_stack, test_short);
    tcase_add_test(tc_stack, test_long);
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
