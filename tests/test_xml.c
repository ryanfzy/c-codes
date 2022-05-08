#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "..\xml\xml.h"

START_TEST(test_1)
{
    char *test_data = "<tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 1, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_1_1)
{
    char *test_data = "<tag/>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 1, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_2)
{
    char *test_data = "<tag></tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 3, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_3)
{
    char *test_data = "<tag>text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 3, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_4)
{
    char *test_data = "<tag attr>text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 4, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_4_1)
{
    char *test_data = "<tag attr attr2>text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 5, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == ATTR, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_5)
{
    char *test_data = "<tag attr=\"value\">text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 5, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_6)
{
    char *test_data = "<tag attr=\"value\" attr2>text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 6, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == ATTR, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[5] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_6_1)
{
    char *test_data = "<tag attr attr2=\"value\">text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 6, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[5] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_7)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\">text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 7, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[5] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[6] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_8)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\"><subtag>text</subtag></tag>";
    XmlToken token = ER;
    XmlToken tokens[15] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 11, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[5] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[6] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[7] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[8] == CLOSE_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[9] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[10] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_9)
{
    char *test_data = "<tag>text<subtag>subtext</subtag>text</tag>";
    XmlToken token = ER;
    XmlToken tokens[10] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 7, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == CLOSE_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[5] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[6] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

START_TEST(test_10)
{
    char *test_data = "<tag><subtag attr=\"value\" attr2=\"value2\">subtext</subtag></tag>";
    XmlToken token = ER;
    XmlToken tokens[15] = { ER };
    int i = 0, n = 0;
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token == ER)
                break;
            tokens[n++] = token;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 11, "Wrong number of tokens found.\n");
    ck_assert_msg(tokens[0] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[1] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[2] == OPEN_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[3] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[4] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[5] == ATTR_NAME, "Wrong token found.\n");
    ck_assert_msg(tokens[6] == ATTR_VAL, "Wrong token found.\n");
    ck_assert_msg(tokens[7] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[8] == CLOSE_TAG, "Wrong token found.\n");
    ck_assert_msg(tokens[9] == TEXT, "Wrong token found.\n");
    ck_assert_msg(tokens[10] == CLOSE_TAG, "Wrong token found.\n");
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("xml");
    TCase *tc_xml = tcase_create("xml");
    tcase_add_test(tc_xml, test_1);
    tcase_add_test(tc_xml, test_1_1);
    tcase_add_test(tc_xml, test_2);
    tcase_add_test(tc_xml, test_3);
    tcase_add_test(tc_xml, test_4);
    tcase_add_test(tc_xml, test_4_1);
    tcase_add_test(tc_xml, test_5);
    tcase_add_test(tc_xml, test_6);
    tcase_add_test(tc_xml, test_6_1);
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
    //check_table();
    return n == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
