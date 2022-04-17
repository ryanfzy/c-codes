#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "bptree.h"
#include "mem_d.h"

START_TEST(test_bptree_create)
{
    BpTree tree = create_bptree();
    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_insert2)
{
    char strTree[] = "((1,2)3(3,4)5(5,6,7,8))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));

    char strTree2[1024] = {0};
    bptree_insert(tree, 9);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2)3(3,4)5(5,6)7(7,8,9))") == 0, "tree is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_insert3)
{
    char strTree[] = "(((1,2)3(3,4)5(5,6))7((7,8)9(9,10)11(11,12)13(13,14)15(15,16)))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));

    bptree_insert(tree, 17);
    char strTree2[1024] = {0};
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "(((1,2)3(3,4)5(5,6))7((7,8)9(9,10))11((11,12)13(13,14)15(15,16,17)))") == 0, "tree is wrong");


    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

/*
START_TEST(test_bptree_insert3)
{
    char strTree[1024] = {0};
    BpTree tree = create_bptree();
    bptree_insert(tree, 39);
    bptree_insert(tree, 22);
    bptree_insert(tree, 97);
    bptree_insert(tree, 41);
    bptree_insert(tree, 53);
    bptree_insert(tree, 13);
    bptree_insert(tree, 21);
    bptree_insert(tree, 40);
    bptree_insert(tree, 30);
    bptree_insert(tree, 27);
    bptree_insert(tree, 33);
    bptree_insert(tree, 36);
    bptree_insert(tree, 35);
    bptree_insert(tree, 34);
    bptree_insert(tree, 24);
    bptree_insert(tree, 29);
    bptree_insert(tree, 26);
    bptree_insert(tree, 42);
    //btree_traverse(btree);
    bptree2str(tree, strTree, 1024);
    ck_assert_msg(strcmp(strTree, "((13,21)22(22,24,26,27,29)30(30,33,34,35)36(36,39,40)41(41,42,53,97))") == 0, "tree is wrong");

    memset(strTree, 0, 1024);
    bptree_keys2str(tree, strTree, 1024);
    ck_assert_msg(strcmp(strTree, "13,21,22,24,26,27,29,30,33,34,35,36,39,40,41,42,53,97") == 0, "tree is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST
*/

START_TEST(test_bptree_insert1)
{
    char strTree[1024] = {0};
    BpTree tree = create_bptree();
    bptree_insert(tree, 5);
    bptree_insert(tree, 8);
    bptree_insert(tree, 10);
    bptree_insert(tree, 15);
    bptree_insert(tree, 16);
    bptree_insert(tree, 17);
    bptree_insert(tree, 18);
    bptree_insert(tree, 6);
    bptree_insert(tree, 9);
    bptree_insert(tree, 19);
    bptree_insert(tree, 20);
    bptree_insert(tree, 21);
    bptree_insert(tree, 22);
    bptree_insert(tree, 7);
    bptree2str(tree, strTree, 1024);
    ck_assert_msg(strcmp(strTree, "(((5,6,7)8(8,9)10(10,15))16((16,17)18(18,19)20(20,21,22)))") == 0, "tree is wrong");

    memset(strTree, 0, 1024);
    bptree_keys2str(tree, strTree, 1024);
    ck_assert_msg(strcmp(strTree, "5,6,7,8,9,10,15,16,17,18,19,20,21,22") == 0, "tree is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_build)
{
    char strTree[] = "(((5,6)7(7,8,9)10(10,15))16((16,17)18(18,19)20(20,21,22)))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));

    char strTree2[1024] = {0};
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree, strTree2) == 0, "tree build is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "5,6,7,8,9,10,15,16,17,18,19,20,21,22") == 0, "tree build is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_delete1)
{
    char strTree[] = "(1,2,3,4)";
    BpTree tree = bptree_build(strTree, sizeof(strTree));

    char strTree2[1024] = {0};

    bptree_delete(tree, 4);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "(1,2,3)") == 0, "tree build is wrong");
    
    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3") == 0, "tree build is wrong");

    bptree_delete(tree, 2);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "(1,3)") == 0, "tree build is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,3") == 0, "tree build is wrong");

    bptree_delete(tree, 1);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "(3)") == 0, "tree build is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "3") == 0, "tree build is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_delete2)
{
    char strTree[] = "((1,2,3)4(4,5,6))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 4);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2,3)5(5,6))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,5,6") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST


START_TEST(test_bptree_delete3)
{
    char strTree[] = "((1,2,3)4(4,5))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 4);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2)3(3,5))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,5") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

/*
// when both current node and its left sibiling contains MIN_KEY_LEN child nodes
// merge them, no need to check its right sibiling
START_TEST(test_bptree_delete4)
{
    char strTree[] = "((1,2)4(4,5)6(6,7,8))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 4);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2)5(5,6)7(7,8))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,5,6,7,8") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST
*/

START_TEST(test_bptree_delete5)
{
    char strTree[] = "((1,2)3(3,4)5(5,6)7(7,8))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 5);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2)3(3,4,6)7(7,8))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,4,6,7,8") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_delete6)
{
    char strTree[] = "(((1,2)3(3,4)5(5,6)7(7,8))9((9,10,11)12(12,13)))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 9);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "(((1,2)3(3,4)5(5,6))7((7,8)10(10,11)12(12,13)))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,4,5,6,7,8,10,11,12,13") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_delete7)
{
    char strTree[] = "(((1,2)3(3,4)5(5,6)7(7,8))9((9,10,11)12(12,13)))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 11);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "(((1,2)3(3,4)5(5,6))7((7,8)9(9,10)12(12,13)))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,4,5,6,7,8,9,10,12,13") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_delete8)
{
    char strTree[] = "(((1,2)3(3,4)5(5,6))7((7,8,9)10(10,11)))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 8);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2)3(3,4)5(5,6)7(7,9)10(10,11))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,4,5,6,7,9,10,11") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

START_TEST(test_bptree_delete9)
{
    char strTree[] = "((1,2,3)4(4,5,6))";
    BpTree tree = bptree_build(strTree, sizeof(strTree));
    
    char strTree2[1024] = {0};

    bptree_delete(tree, 6);
    bptree2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "((1,2,3)4(4,5))") == 0, "deletion is wrong");

    memset(strTree2, 0, 1024);
    bptree_keys2str(tree, strTree2, 1024);
    ck_assert_msg(strcmp(strTree2, "1,2,3,4,5") == 0, "deletion is wrong");

    free_bptree(tree);
    ck_assert_msg(check_mem(), "memory leak");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("bptree");
    TCase *tc_tree = tcase_create("bptree");
    tcase_add_test(tc_tree, test_bptree_create);
    tcase_add_test(tc_tree, test_bptree_insert1);
    tcase_add_test(tc_tree, test_bptree_insert2);
    tcase_add_test(tc_tree, test_bptree_insert3);
    tcase_add_test(tc_tree, test_bptree_build);
    tcase_add_test(tc_tree, test_bptree_delete1);
    tcase_add_test(tc_tree, test_bptree_delete2);
    tcase_add_test(tc_tree, test_bptree_delete3);
    //tcase_add_test(tc_tree, test_bptree_delete4);
    tcase_add_test(tc_tree, test_bptree_delete5);
    tcase_add_test(tc_tree, test_bptree_delete6);
    tcase_add_test(tc_tree, test_bptree_delete7);
    tcase_add_test(tc_tree, test_bptree_delete8);
    tcase_add_test(tc_tree, test_bptree_delete9);
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
