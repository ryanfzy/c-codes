#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "..\xml\xmlparser.h"

#define CK_ASSERT_INT_EQ(E,A) ck_assert_msg(E == A, "Expected %d, but actual %d.\n", E, A)
#define CK_ASSERT_STR_EQ(E,A) ck_assert_msg(strcmp(E, A) == 0, "Expected %s, but actual %s.\n", E, A)
#define CK_ASSERT_TRUE(A) ck_assert_msg(A, "Expected true, but actual false.\n")

typedef struct _Result
{
    XmlToken token;
    char text[32];
} Result;

static void s_memcpy(char *dest, char *src, size_t size)
{
    memcpy(dest, src, size);
    dest[size] = '\0';
}

START_TEST(test_1)
{
    char *test_data = "<tag attr=\"val\" attr2>text</tag> ";
    Result results[15] = { 0 };
    int i = 0, n = 0;
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(parser, test_data[i], &token))
        {
            if (token. type != NONE)
            {
                results[n].token = token;
                s_memcpy(results[n].text, test_data+token.start, token.length);
                n++;
            }
        }
        else
            break;
    }
    xmlparser_free(parser);

    CK_ASSERT_INT_EQ(strlen(test_data), i);
    CK_ASSERT_INT_EQ(12, n);
    CK_ASSERT_INT_EQ(LEFT_ANGLE, results[0].token.type);
    CK_ASSERT_STR_EQ("<", results[0].text);
    CK_ASSERT_INT_EQ(IDENTIFIER, results[1].token.type);
    CK_ASSERT_STR_EQ("tag", results[1].text);
    CK_ASSERT_INT_EQ(IDENTIFIER, results[2].token.type);
    CK_ASSERT_STR_EQ("attr", results[2].text);
    CK_ASSERT_INT_EQ(EQUAL, results[3].token.type);
    CK_ASSERT_STR_EQ("=", results[3].text);
    CK_ASSERT_INT_EQ(QUOTED_STRING, results[4].token.type);
    CK_ASSERT_STR_EQ("\"val\"", results[4].text);
    CK_ASSERT_INT_EQ(IDENTIFIER, results[5].token.type);
    CK_ASSERT_STR_EQ("attr2", results[5].text);
    CK_ASSERT_INT_EQ(RIGHT_ANGLE, results[6].token.type);
    CK_ASSERT_STR_EQ(">", results[6].text);
    CK_ASSERT_INT_EQ(TEXT, results[7].token.type);
    CK_ASSERT_STR_EQ("text", results[7].text);
    CK_ASSERT_INT_EQ(LEFT_ANGLE, results[8].token.type);
    CK_ASSERT_STR_EQ("<", results[8].text);
    CK_ASSERT_INT_EQ(SLASH, results[9].token.type);
    CK_ASSERT_STR_EQ("/", results[9].text);
    CK_ASSERT_INT_EQ(IDENTIFIER, results[10].token.type);
    CK_ASSERT_STR_EQ("tag", results[10].text);
    CK_ASSERT_INT_EQ(RIGHT_ANGLE, results[11].token.type);
    CK_ASSERT_STR_EQ(">", results[11].text);
}
END_TEST

START_TEST(test_1_1)
{
    char *test_data = "<tag /> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_1_2)
{
    char *test_data = "<tag attr /> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_1_3)
{
    char *test_data = "<tag attr=\"val\" /> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_2)
{
    char *test_data = "<tag></tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_3)
{
    char *test_data = "<tag>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_4)
{
    char *test_data = "<tag attr>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_4_1)
{
    char *test_data = "<tag attr attr2>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_5)
{
    char *test_data = "<tag attr=\"value\">text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_6)
{
    char *test_data = "<tag attr=\"value\" attr2>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_6_1)
{
    char *test_data = "<tag attr attr2=\"value\">text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_7)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\">text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_8)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\"><subtag>text</subtag></tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

/*
// TODO
START_TEST(test_9)
{
    char *test_data = "<tag>text<subtag>subtext</subtag>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser;
    xmlparser_init(&parser);
    for (int i = 0; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i], &token))
        {
            if (token.type != NONE)
                CK_ASSERT_TRUE(xmlparser_feed_token(&parser, &token));
        }
    }
    xmlparser_destroy(&parser);
}
END_TEST*/

START_TEST(test_10)
{
    char *test_data = "<tag><subtag attr=\"value\" attr2=\"value2\">subtext</subtag></tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    for (int i = 0; i < strlen(test_data); ++i)
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &token));
    }
    xmlparser_free(parser);
}
END_TEST

Suite* make_add_suit(void)
{
    Suite *s = suite_create("xml");
    TCase *tc_xml = tcase_create("xml");
    tcase_add_test(tc_xml, test_1);
    tcase_add_test(tc_xml, test_1_1);
    tcase_add_test(tc_xml, test_1_2);
    tcase_add_test(tc_xml, test_1_3);
    tcase_add_test(tc_xml, test_2);
    tcase_add_test(tc_xml, test_3);
    tcase_add_test(tc_xml, test_4);
    tcase_add_test(tc_xml, test_4_1);
    tcase_add_test(tc_xml, test_5);
    tcase_add_test(tc_xml, test_6);
    tcase_add_test(tc_xml, test_6_1);
    tcase_add_test(tc_xml, test_7);
    tcase_add_test(tc_xml, test_8);
    //tcase_add_test(tc_xml, test_9);
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
