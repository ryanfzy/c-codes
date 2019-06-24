#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "btree.h"
#include "mem_d.h"

START_TEST(test_btree_create)
{
    BTree btree = create_btree();
    free_btree(btree);
    ck_assert_msg(check_mem(), "memory leak");
    /*
    ck_assert_msg(ptree->parent == NULL, "parent is not NULL");
    ck_assert_msg(ptree->pchildren == NULL, "children is not NULL");
    ck_assert_msg(ptree->pnode != NULL, "node is NULL");
    ck_assert_int_eq(((TestData*)(ptree->pnode->pdata))->ivalue, 1);

    tree_free(ptree);
    */
}
END_TEST

START_TEST(test_btree_insert)
{
    char strTree[1024] = {0};
    BTree btree = create_btree();
    btree_insert(btree, 10);
    btree_insert(btree, 20);
    btree_insert(btree, 30);
    btree_insert(btree, 40);
    btree_insert(btree, 50);
    btree_insert(btree, 60);
    btree_insert(btree, 70);
    btree_insert(btree, 80);
    btree_insert(btree, 90);
    //btree_traverse(btree);
    btree2str(btree, strTree, 1024);
    free_btree(btree);
    //printf("%s\n", strTree);
    ck_assert_msg(strcmp(strTree, "((10)20(30)40(50)60(70,80,90))") == 0, "tree is wrong");
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_btree_insert2)
{
    char strTree[1024] = {0};
    BTree btree = create_btree();
    btree_insert(btree, 90);
    btree_insert(btree, 50);
    btree_insert(btree, 10);
    btree_insert(btree, 80);
    btree_insert(btree, 20);
    btree_insert(btree, 70);
    btree_insert(btree, 30);
    btree_insert(btree, 60);
    btree_insert(btree, 40);
    //btree_traverse(btree);
    btree2str(btree, strTree, 1024);
    free_btree(btree);
    //printf("%s\n", strTree);
    ck_assert_msg(strcmp(strTree, "((10,20,30,40)50(60,70,80,90))") == 0, "tree is wrong");
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_btree_insert3)
{
    char strTree[1024] = {0};
    BTree btree = create_btree();
    btree_insert(btree, 39);
    btree_insert(btree, 22);
    btree_insert(btree, 97);
    btree_insert(btree, 41);
    btree_insert(btree, 53);
    btree_insert(btree, 13);
    btree_insert(btree, 21);
    btree_insert(btree, 40);
    btree_insert(btree, 30);
    btree_insert(btree, 27);
    btree_insert(btree, 33);
    btree_insert(btree, 36);
    btree_insert(btree, 35);
    btree_insert(btree, 34);
    btree_insert(btree, 24);
    btree_insert(btree, 29);
    btree_insert(btree, 26);
    btree_insert(btree, 42);
    //btree_traverse(btree);
    btree2str(btree, strTree, 1024);
    free_btree(btree);
    //printf("%s\n", strTree);
    ck_assert_msg(strcmp(strTree, "(((13)21(22,24,26))27((29,30)33(34,35,36)39(40)41(42,53,97)))") == 0, "tree is wrong");
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("btree");
    TCase *tc_tree = tcase_create("btree");
    tcase_add_test(tc_tree, test_btree_create);
    tcase_add_test(tc_tree, test_btree_insert);
    tcase_add_test(tc_tree, test_btree_insert2);
    tcase_add_test(tc_tree, test_btree_insert3);
    suite_add_tcase(s, tc_tree);
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
