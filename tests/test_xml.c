#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "..\xml\xml.h"

START_TEST(test_1)
{
    char *test_data = "<tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_2)
{
    char *test_data = "<tag></tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_3)
{
    char *test_data = "<tag>text</tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_4)
{
    char *test_data = "<tag attr>text</tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_5)
{
    char *test_data = "<tag attr=\"value\">text</tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_6)
{
    char *test_data = "<tag attr=\"value\" attr2>text</tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_7)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\">text</tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_8)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\"><subtag>text</subtag></tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_9)
{
    char *test_data = "<tag>text<subtag>subtext</subtag>text</tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

START_TEST(test_10)
{
    char *test_data = "<tag><subtag attr=\"value\" attr2=\"value2\">subtext</subtag></tag>";
    XmlParser parser = xmlparser_create();
    int i = 0;
    for (; i < strlen(test_data); i++)
    {
        if (!xmlparser_feed(parser, test_data[i]))
            break;
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("xml");
    TCase *tc_xml = tcase_create("xml");
    tcase_add_test(tc_xml, test_1);
    tcase_add_test(tc_xml, test_2);
    tcase_add_test(tc_xml, test_3);
    tcase_add_test(tc_xml, test_4);
    tcase_add_test(tc_xml, test_5);
    tcase_add_test(tc_xml, test_6);
    tcase_add_test(tc_xml, test_7);
    tcase_add_test(tc_xml, test_8);
    tcase_add_test(tc_xml, test_9);
    tcase_add_test(tc_xml, test_10);
    suite_add_tcase(s, tc_xml);
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
