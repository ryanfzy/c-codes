#include <stdlib.h>
#include <check.h>
#include "tree.h"

#define CK_ASSERT_INT_EQ(E,A) ck_assert_msg(E == A, "Expected %d, but actual %d.\n", E, A)

typedef struct _TestData
{
    int ivalue;
    double dvalue;
} TestData;

START_TEST(test_tree_create)
{
    TestData t;
    t.ivalue = 1;
    Tree *tree = tree_create((char*)&t, sizeof(TestData));

    ck_assert_msg(tree->parent == NULL, "parent is not NULL");
    CK_ASSERT_INT_EQ(0, list_count(&tree->children));
    ck_assert_msg(tree->node != NULL, "node is NULL");
    TestData *data = (TestData*)tree_get_data(tree);
    CK_ASSERT_INT_EQ(1, data->ivalue);

    tree_free(tree);
}
END_TEST

START_TEST(test_tree_init)
{
    TestData t;
    t.ivalue = 1;
    Tree tree;
    tree_init(&tree, (char*)&t, sizeof(TestData));

    ck_assert_msg(tree.parent == NULL, "parent is not NULL");
    CK_ASSERT_INT_EQ(0, list_count(&tree.children));
    ck_assert_msg(tree.node != NULL, "node is NULL");
    TestData *data = (TestData*)tree_get_data(&tree);
    CK_ASSERT_INT_EQ(1, data->ivalue);

}
END_TEST

START_TEST(test_tree_destroy1)
{
    TestData t;
    t.ivalue = 1;
    Tree tree;
    tree_init(&tree, (char*)&t, sizeof(TestData));
    
    ck_assert_msg(tree.node != NULL, "node is NULL");

    tree_destroy(&tree);
    ck_assert_msg(tree.node == NULL, "node is not NULL");
}
END_TEST

START_TEST(test_tree_destroy2)
{
    TestData t1, t2;
    t1.ivalue = 1;
    t2.ivalue = 2;
    Tree tree;
    tree_init(&tree, (char*)&t1, sizeof(TestData));
    tree_add_child(&tree, (char*)&t2, sizeof(TestData));

    tree_destroy(&tree);
}
END_TEST

START_TEST(test_tree_getdata)
{
    TestData t, *pt;
    t.ivalue = 1;
    Tree *tree = tree_create((char*)&t, sizeof(TestData));
    pt = (TestData*)tree_get_data(tree);

    ck_assert_msg(&t != pt, "test data is not copied");
    CK_ASSERT_INT_EQ(1, pt->ivalue);
    ck_assert_msg(tree->node != NULL, "node is NULL");
    TestData *data = (TestData*)tree_get_data(tree);
    CK_ASSERT_INT_EQ(1, data->ivalue);
}
END_TEST

START_TEST(test_tree_count)
{
    TestData t1, t2, t3;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    Tree tree;
    tree_init(&tree, (char*)&t1, sizeof(TestData));
    tree_add_child(&tree, (char*)&t2, sizeof(TestData));
    tree_add_child(&tree, (char*)&t3, sizeof(TestData));

    CK_ASSERT_INT_EQ(tree_children_count(&tree), 2);
}
END_TEST

START_TEST(test_tree_add1)
{
    TestData t1, t2, t3;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    Tree *pa = tree_create((char*)&t1, sizeof(TestData));
    Tree *pb = tree_add_child(pa, (char*)&t2, sizeof(TestData)); 
    Tree *pc = tree_add_child(pa, (char*)&t3, sizeof(TestData));

    CK_ASSERT_INT_EQ(tree_children_count(pa), 2);
    CK_ASSERT_INT_EQ(1, ((TestData*)tree_get_data(pa))->ivalue);
    CK_ASSERT_INT_EQ(2, ((TestData*)tree_get_data(pb))->ivalue);
    CK_ASSERT_INT_EQ(3, ((TestData*)tree_get_data(pc))->ivalue);

    ck_assert_msg(tree_get_child(pa, 0) != NULL, "child 0 is NULL");
    ck_assert_msg(tree_get_child(pa, 1) != NULL, "child 1 is NULL");
    ck_assert_msg(tree_get_child(pa, 0) == pb, "child 0 is not pb");
    ck_assert_msg(tree_get_child(pa, 1) == pc, "child 1 is not pc");

    ck_assert_msg(pa->parent == NULL, "pa parent is not NULL");
    ck_assert_msg(pb->parent == pa, "pb parent is not pa");
    ck_assert_msg(pc->parent == pa, "pc parent is not pa");

    CK_ASSERT_INT_EQ(0, list_count(&pb->children));
    CK_ASSERT_INT_EQ(0, list_count(&pc->children));

    tree_free(pa);
    //CK_ASSERT_INT_EQ(((TestData*)tree_get_data(pc))->ivalue, 1);
}
END_TEST

START_TEST(test_tree_add2)
{
    TestData t1, t2, t3, t4;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    t4.ivalue = 4;
    Tree ta, *pb, *pc, *pd;
    tree_init(&ta, (char*)&t1, sizeof(TestData));
    pb = tree_add_child(&ta, (char*)&t2, sizeof(TestData));
    pc = tree_add_child(pb, (char*)&t3, sizeof(TestData));
    pd = tree_add_child(pb, (char*)&t4, sizeof(TestData));

    CK_ASSERT_INT_EQ(1, tree_children_count(&ta));
    CK_ASSERT_INT_EQ(1, ((TestData*)tree_get_data(&ta))->ivalue);
    ck_assert_msg(tree_has_children(&ta), "ta has no children");

    CK_ASSERT_INT_EQ(2, tree_children_count(pb));
    CK_ASSERT_INT_EQ(((TestData*)tree_get_data(pb))->ivalue, 2);
    ck_assert_msg(pb->parent == &ta, "pb parent is not ta");
    ck_assert_msg(tree_has_children(pb), "pa has no children");

    ck_assert_msg(tree_get_child(pb, 0) == pc, "pc is not child 0 of pb");
    ck_assert_msg(tree_get_child(pb, 1) == pd, "pd is not child 1 of pb");
    CK_ASSERT_INT_EQ(3, ((TestData*)tree_get_data(pc))->ivalue);
    CK_ASSERT_INT_EQ(4, ((TestData*)tree_get_data(pd))->ivalue);
    ck_assert_msg(pc->parent == pb, "pc parent is not pb");
    ck_assert_msg(pd->parent == pb, "pd parent is not pb");

    ck_assert_msg(!tree_has_children(pc), "pc has children");
    ck_assert_msg(!tree_has_children(pd), "pd has chlidren");

    tree_destroy(&ta);
}
END_TEST

START_TEST(test_tree_add3)
{
    TestData t1, t2, t3, t4, t5;
    t1.ivalue = 1;
    t2.ivalue = 2;
    t3.ivalue = 3;
    t4.ivalue = 4;
    t5.ivalue = 5;
    Tree *pa = tree_create((char*)&t1, sizeof(TestData));
    Tree *pb = tree_add_child(pa, (char*)&t2, sizeof(TestData));
    Tree *pc = tree_add_child(pa, (char*)&t3, sizeof(TestData));
    Tree *pd = tree_add_child(pc, (char*)&t4, sizeof(TestData));
    Tree *pe = tree_add_child(pc, (char*)&t5, sizeof(TestData));

    CK_ASSERT_INT_EQ(2, tree_children_count(pa));
    CK_ASSERT_INT_EQ(0, tree_children_count(pb));
    CK_ASSERT_INT_EQ(2, tree_children_count(pc));
    CK_ASSERT_INT_EQ(0, tree_children_count(pd));
    CK_ASSERT_INT_EQ(0, tree_children_count(pe));

    ck_assert_msg(tree_get_child(pa, 0) == pb, "pb is not child 0 of pa");
    ck_assert_msg(tree_get_child(pa, 1) == pc, "pc is not child 1 of pa");
    ck_assert_msg(tree_get_child(pc, 0) == pd, "pd is not child 0 of pc");
    ck_assert_msg(tree_get_child(pc, 1) == pe, "pe is not child 1 of pc");

    tree_free(pa);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("tree");
    TCase *tc_tree = tcase_create("tree");
    tcase_add_test(tc_tree, test_tree_create);
    tcase_add_test(tc_tree, test_tree_init);
    tcase_add_test(tc_tree, test_tree_destroy1);
    tcase_add_test(tc_tree, test_tree_destroy2);
    tcase_add_test(tc_tree, test_tree_getdata);
    tcase_add_test(tc_tree, test_tree_count);
    tcase_add_test(tc_tree, test_tree_add1);
    tcase_add_test(tc_tree, test_tree_add2);
    tcase_add_test(tc_tree, test_tree_add3);
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
