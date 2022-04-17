#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "sha256.h"

START_TEST(test_short)
{
    Hash hash = sha_create("abc");
    char *exp = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
    char act[65] = {0};
    sha2xstr(hash, act, 64);
    ck_assert_msg(strcmp(act, exp) == 0, "short is wrong.");
    sha_free(hash);
}
END_TEST

START_TEST(test_long)
{
    Hash hash = sha_create("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    char *exp = "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1";
    char act[65] = {0};
    sha2xstr(hash, act, 64);
    ck_assert_msg(strcmp(act, exp) == 0, "long is wrong.");
    sha_free(hash);
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
