#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "..\xml\xml.h"

#define CK_ASSERT_INT_EQ(E,A) ck_assert_msg(E == A, "Expected %d, but actual %d.\n", E, A);

typedef struct _ActualResult
{
    XmlToken token;
    int start;
    int length;
} ActualResult;

START_TEST(test_1)
{
    char *test_data = "<tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 1, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
}
END_TEST

START_TEST(test_1_1)
{
    char *test_data = "<tag />";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 2, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[1].start);
    CK_ASSERT_INT_EQ(3, results[1].length);
}
END_TEST

START_TEST(test_1_2)
{
    char *test_data = "<tag attr />";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 3, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[2].start);
    CK_ASSERT_INT_EQ(3, results[2].length);
}
END_TEST

START_TEST(test_1_3)
{
    char *test_data = "<tag attr=\"val\" />";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 4, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(11, results[2].start);
    CK_ASSERT_INT_EQ(3, results[2].length);
    ck_assert_msg(results[3].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[3].start);
    CK_ASSERT_INT_EQ(3, results[3].length);
}
END_TEST

START_TEST(test_2)
{
    char *test_data = "<tag></tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 2, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(7, results[1].start);
    CK_ASSERT_INT_EQ(3, results[1].length);
}
END_TEST

START_TEST(test_3)
{
    char *test_data = "<tag>text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 3, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(11, results[2].start);
    CK_ASSERT_INT_EQ(3, results[2].length);
}
END_TEST

START_TEST(test_4)
{
    char *test_data = "<tag attr>text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 4, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(10, results[2].start);
    CK_ASSERT_INT_EQ(4, results[2].length);
    ck_assert_msg(results[3].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(16, results[3].start);
    CK_ASSERT_INT_EQ(3, results[3].length);
}
END_TEST

START_TEST(test_4_1)
{
    char *test_data = "<tag attr attr2>text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 5, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(10, results[2].start);
    CK_ASSERT_INT_EQ(5, results[2].length);
    ck_assert_msg(results[3].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(16, results[3].start);
    CK_ASSERT_INT_EQ(4, results[3].length);
    ck_assert_msg(results[4].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(22, results[4].start);
    CK_ASSERT_INT_EQ(3, results[4].length);
}
END_TEST

START_TEST(test_5)
{
    char *test_data = "<tag attr=\"value\">text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 5, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(11, results[2].start);
    CK_ASSERT_INT_EQ(5, results[2].length);
    ck_assert_msg(results[3].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(18, results[3].start);
    CK_ASSERT_INT_EQ(4, results[3].length);
    ck_assert_msg(results[4].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(24, results[4].start);
    CK_ASSERT_INT_EQ(3, results[4].length);
}
END_TEST

START_TEST(test_6)
{
    char *test_data = "<tag attr=\"value\" attr2>text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 6, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(11, results[2].start);
    CK_ASSERT_INT_EQ(5, results[2].length);
    ck_assert_msg(results[3].token == ATTR, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(18, results[3].start);
    CK_ASSERT_INT_EQ(5, results[3].length);
    ck_assert_msg(results[4].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(24, results[4].start);
    CK_ASSERT_INT_EQ(4, results[4].length);
    ck_assert_msg(results[5].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(30, results[5].start);
    CK_ASSERT_INT_EQ(3, results[5].length);
}
END_TEST

START_TEST(test_6_1)
{
    char *test_data = "<tag attr attr2=\"value\">text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 6, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(10, results[2].start);
    CK_ASSERT_INT_EQ(5, results[2].length);
    ck_assert_msg(results[3].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(17, results[3].start);
    CK_ASSERT_INT_EQ(5, results[3].length);
    ck_assert_msg(results[4].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(24, results[4].start);
    CK_ASSERT_INT_EQ(4, results[4].length);
    ck_assert_msg(results[5].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(30, results[5].start);
    CK_ASSERT_INT_EQ(3, results[5].length);
}
END_TEST

START_TEST(test_7)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\">text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 7, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(11, results[2].start);
    CK_ASSERT_INT_EQ(5, results[2].length);
    ck_assert_msg(results[3].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(18, results[3].start);
    CK_ASSERT_INT_EQ(5, results[3].length);
    ck_assert_msg(results[4].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(25, results[4].start);
    CK_ASSERT_INT_EQ(6, results[4].length);
    ck_assert_msg(results[5].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(33, results[5].start);
    CK_ASSERT_INT_EQ(4, results[5].length);
    ck_assert_msg(results[6].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(39, results[6].start);
    CK_ASSERT_INT_EQ(3, results[6].length);
}
END_TEST

START_TEST(test_8)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\"><subtag>text</subtag></tag>";
    ActualResult results[15] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 9, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(11, results[2].start);
    CK_ASSERT_INT_EQ(5, results[2].length);
    ck_assert_msg(results[3].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(18, results[3].start);
    CK_ASSERT_INT_EQ(5, results[3].length);
    ck_assert_msg(results[4].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(25, results[4].start);
    CK_ASSERT_INT_EQ(6, results[4].length);
    ck_assert_msg(results[5].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(34, results[5].start);
    CK_ASSERT_INT_EQ(6, results[5].length);
    ck_assert_msg(results[6].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(41, results[6].start);
    CK_ASSERT_INT_EQ(4, results[6].length);
    ck_assert_msg(results[7].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(47, results[7].start);
    CK_ASSERT_INT_EQ(6, results[7].length);
    ck_assert_msg(results[8].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(56, results[8].start);
    CK_ASSERT_INT_EQ(3, results[8].length);
}
END_TEST

START_TEST(test_9)
{
    char *test_data = "<tag>text<subtag>subtext</subtag>text</tag>";
    ActualResult results[10] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 7, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(5, results[1].start);
    CK_ASSERT_INT_EQ(4, results[1].length);
    ck_assert_msg(results[2].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(10, results[2].start);
    CK_ASSERT_INT_EQ(6, results[2].length);
    ck_assert_msg(results[3].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(17, results[3].start);
    CK_ASSERT_INT_EQ(7, results[3].length);
    ck_assert_msg(results[4].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(26, results[4].start);
    CK_ASSERT_INT_EQ(6, results[4].length);
    ck_assert_msg(results[5].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(33, results[5].start);
    CK_ASSERT_INT_EQ(4, results[5].length);
    ck_assert_msg(results[6].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(39, results[6].start);
    CK_ASSERT_INT_EQ(3, results[6].length);
}
END_TEST

START_TEST(test_10)
{
    char *test_data = "<tag><subtag attr=\"value\" attr2=\"value2\">subtext</subtag></tag>";
    ActualResult results[15] = { 0 };
    int i = 0, n = 0;
    XmlParser parser;
    xmlparser_init(&parser);
    for (; i < strlen(test_data); ++i)
    {
        if (xmlparser_feed(&parser, test_data[i]))
        {
            if (parser.token == ER)
                break;
            results[n].token = parser.token;
            results[n].start = parser.start;
            results[n].length = parser.length;
            n++;
        }
    }
    ck_assert_msg(i == strlen(test_data), "Invalid char at index: %d\n", i);
    ck_assert_msg(n == 9, "Wrong number of tokens found.\n");
    ck_assert_msg(results[0].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(1, results[0].start);
    CK_ASSERT_INT_EQ(3, results[0].length);
    ck_assert_msg(results[1].token == OPEN_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(6, results[1].start);
    CK_ASSERT_INT_EQ(6, results[1].length);
    ck_assert_msg(results[2].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(13, results[2].start);
    CK_ASSERT_INT_EQ(4, results[2].length);
    ck_assert_msg(results[3].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(19, results[3].start);
    CK_ASSERT_INT_EQ(5, results[3].length);
    ck_assert_msg(results[4].token == ATTR_NAME, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(26, results[4].start);
    CK_ASSERT_INT_EQ(5, results[4].length);
    ck_assert_msg(results[5].token == ATTR_VAL, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(33, results[5].start);
    CK_ASSERT_INT_EQ(6, results[5].length);
    ck_assert_msg(results[6].token == TEXT, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(41, results[6].start);
    CK_ASSERT_INT_EQ(7, results[6].length);
    ck_assert_msg(results[7].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(50, results[7].start);
    CK_ASSERT_INT_EQ(6, results[7].length);
    ck_assert_msg(results[8].token == CLOSE_TAG, "Wrong token found.\n");
    CK_ASSERT_INT_EQ(59, results[8].start);
    CK_ASSERT_INT_EQ(3, results[8].length);
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
