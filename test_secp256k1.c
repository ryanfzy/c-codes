#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "sha256.h"
#include "binary.h"
#include "secp256k1.h"
#include "ecc.h"
#include "mem_d.h"

// this is slow, before make it fast, don't run it
START_TEST(test_sign)
{
    Bin d = bin_create("x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef");
    Hash hash = sha_create("abc");
    char szStr[66] = {0};
    szStr[0] = 'x';
    sha2xstr(hash, szStr+1, 64);
    printf("has of \"abc\": %s\n", szStr);
    Bin h = bin_create(szStr);
    bin2xstr(h, szStr, 64);
    printf("h: %s\n", szStr);
    bin2xstr(d, szStr, 64);
    printf("d: %s\n", szStr);
    Bin s = 0;
    Bin r = 0;
    sign(d, h, &s, &r);
    bin2xstr(r, szStr, 64);
    printf("r: %s\n", szStr);
    bin2xstr(s, szStr, 64);
    printf("s: %s\n", szStr);
    //ck_assert_msg(strcmp(act, exp) == 0, "short is wrong.");
    sha_free(hash);
    bin_free(d);
    bin_free(h);
    bin_free(s);
    bin_free(r);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("secp256k1");
    TCase *tc_stack = tcase_create("secp256k1");
    // this is slow, before make it fast, don't run it
    //tcase_add_test(tc_stack, test_sign);
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
