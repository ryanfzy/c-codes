#include <stdlib.h>
#include <check.h>
#include "tree.h"

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_tree_create1)
{
    TestData t;
    t.ivalue = 1;
    Tree *ptree = tree_create((char*)&t, sizeof(TestData));

    ck_assert_msg(ptree->parent == NULL, "parent is not NULL");
    ck_assert_msg(ptree->pchildren == NULL, "children is not NULL");
    ck_assert_msg(ptree->pnode != NULL, "node is NULL");
    ck_assert_int_eq(((TestData*)(ptree->pnode->pdata))->ivalue, 1);
}
END_TEST

START_TEST(test_tree_create2)
{
    TestData t;
    t.ivalue = 1;
    Tree tree;
    tree_init(&tree, (char*)&t, sizeof(TestData));

    ck_assert_msg(tree.parent == NULL, "parent is not NULL");
    ck_assert_msg(tree.pchildren == NULL, "children is not NULL");
    ck_assert_msg(tree.pnode != NULL, "node is NULL");
    ck_assert_int_eq(((TestData*)(tree.pnode->pdata))->ivalue, 1);
}
END_TEST

START_TEST(test_tree_add1)
{
    TestData t;
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("list");
    TCase *tc_list = tcase_create("list");
    tcase_add_test(tc_list, test_tree_create1);
    tcase_add_test(tc_list, test_tree_create2);
    tcase_add_test(tc_list, test_tree_add1);
    suite_add_tcase(s, tc_list);
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
