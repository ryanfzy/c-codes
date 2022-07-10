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
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
        if (token. type != NONE)
        {
            results[n].token = token;
            s_memcpy(results[n].text, test_data+token.start, token.length);
            n++;
        }
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
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_1_2)
{
    char *test_data = "<tag attr /> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_1_3)
{
    char *test_data = "<tag attr=\"val\" /> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_2)
{
    char *test_data = "<tag></tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_3)
{
    char *test_data = "<tag>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_4)
{
    char *test_data = "<tag attr>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_4_1)
{
    char *test_data = "<tag attr attr2>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_5)
{
    char *test_data = "<tag attr=\"value\">text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_6)
{
    char *test_data = "<tag attr=\"value\" attr2>text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_6_1)
{
    char *test_data = "<tag attr attr2=\"value\">text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_7)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\">text</tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_8)
{
    char *test_data = "<tag attr=\"value\" attr2=\"value2\"><subtag>text</subtag></tag> ";
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
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
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
}
END_TEST

START_TEST(test_11)
{
    char *test_data = "<tag attr1><subtag attr2=\"val2\">subtext</subtag></tag> ";
    int start_tag_found = 0;
    Result start_tags[2];
    int close_tag_found = 0;
    Result close_tags[2];
    int attr_found = 0;
    Result attrs[4] = {0};
    int text_found = 0;
    Result texts[1];
    void on_start_found(XmlToken *t)
    {
        start_tags[start_tag_found].token = *t;
        s_memcpy(start_tags[start_tag_found].text, test_data+t->start, t->length);
        start_tag_found++;
    }
    void on_close_found(XmlToken *t)
    {
        close_tags[close_tag_found].token = *t;
        s_memcpy(close_tags[close_tag_found].text, test_data+t->start, t->length);
        close_tag_found++;
    }
    void on_attr_found(XmlToken *k, XmlToken *v)
    {
        int i = attr_found*2;
        attrs[i].token = *k;
        s_memcpy(attrs[i].text, test_data+k->start, k->length);
        if (v != NULL)
        {
            attrs[i+1].token = *v;
            s_memcpy(attrs[i+1].text, test_data+v->start, v->length);
        }
        attr_found++;
    }
    void on_text_found(XmlToken *t)
    {
        texts[text_found].token = *t;
        s_memcpy(texts[text_found].text, test_data+t->start, t->length);
        text_found++;
    }
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    xmlparser_set_listners(parser, on_start_found, on_attr_found, on_text_found, on_close_found);
    int i = 0;
    while (i < strlen(test_data))
    {
        CK_ASSERT_TRUE(xmlparser_feed(parser, test_data[i], &i, &token));
    }
    xmlparser_free(parser);
    CK_ASSERT_INT_EQ(2, start_tag_found);
    CK_ASSERT_STR_EQ("tag", start_tags[0].text);
    CK_ASSERT_STR_EQ("subtag", start_tags[1].text);
    CK_ASSERT_INT_EQ(2, close_tag_found);
    CK_ASSERT_STR_EQ("subtag", close_tags[0].text);
    CK_ASSERT_STR_EQ("tag", close_tags[1].text);
    CK_ASSERT_INT_EQ(2, attr_found);
    CK_ASSERT_STR_EQ("attr1", attrs[0].text);
    CK_ASSERT_INT_EQ(0, attrs[1].token.type);
    CK_ASSERT_INT_EQ(0, attrs[1].token.start);
    CK_ASSERT_INT_EQ(0, attrs[1].token.length);
    CK_ASSERT_STR_EQ("attr2", attrs[2].text);
    CK_ASSERT_STR_EQ("\"val2\"", attrs[3].text);
    CK_ASSERT_INT_EQ(1, text_found);
    CK_ASSERT_STR_EQ("subtext", texts[0].text);
}
END_TEST

START_TEST(test_12)
{
    char *test_file_path = "..\\test files\\xml_test.txt";
    FILE *fp = fopen(test_file_path, "rb");
    int start_tag_found = 0;
    Result start_tags[2];
    int close_tag_found = 0;
    Result close_tags[2];
    int attr_found = 0;
    Result attrs[4] = {0};
    int text_found = 0;
    Result texts[1];
    void on_start_found(XmlToken *t)
    {
        start_tags[start_tag_found].token = *t;
        int cur = ftell(fp);
        fseek(fp, t->start, SEEK_SET);
        fgets(start_tags[start_tag_found].text, t->length+1, fp);
        fseek(fp, cur, SEEK_SET);
        start_tag_found++;
    }
    void on_close_found(XmlToken *t)
    {
        close_tags[close_tag_found].token = *t;
        int cur = ftell(fp);
        fseek(fp, t->start, SEEK_SET);
        fgets(close_tags[close_tag_found].text, t->length+1, fp);
        fseek(fp, cur, SEEK_SET);
        close_tag_found++;
    }
    void on_attr_found(XmlToken *k, XmlToken *v)
    {
        int i = attr_found*2;
        attrs[i].token = *k;
        int cur = ftell(fp);
        fseek(fp, k->start, SEEK_SET);
        fgets(attrs[i].text, k->length+1, fp);
        if (v != NULL)
        {
            attrs[i+1].token = *v;
            fseek(fp, v->start, SEEK_SET);
            fgets(attrs[i+1].text, v->length+1, fp);
        }
        fseek(fp, cur, SEEK_SET);
        attr_found++;
    }
    void on_text_found(XmlToken *t)
    {
        texts[text_found].token = *t;
        int cur = ftell(fp);
        fseek(fp, t->start, SEEK_SET);
        fgets(texts[text_found].text, t->length+1, fp);
        fseek(fp, cur, SEEK_SET);
        text_found++;
    }
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    xmlparser_set_listners(parser, on_start_found, on_attr_found, on_text_found, on_close_found);
    int ch = 0;
    int i = 0;
    int j = 0;
    while (EOF != (ch = fgetc(fp)))
    {
        j = ftell(fp);
        CK_ASSERT_TRUE(xmlparser_feed(parser, ch, &i, &token));
        if (i < j)
        {
            fseek(fp, i, SEEK_SET);
        }
    }
    CK_ASSERT_TRUE(xmlparser_feed(parser, ' ', &i, &token));
    fclose(fp);
    xmlparser_free(parser);

    CK_ASSERT_INT_EQ(2, start_tag_found);
    CK_ASSERT_STR_EQ("tag", start_tags[0].text);
    CK_ASSERT_STR_EQ("subtag", start_tags[1].text);
    CK_ASSERT_INT_EQ(2, close_tag_found);
    CK_ASSERT_STR_EQ("subtag", close_tags[0].text);
    CK_ASSERT_STR_EQ("tag", close_tags[1].text);
    CK_ASSERT_INT_EQ(2, attr_found);
    CK_ASSERT_STR_EQ("attr1", attrs[0].text);
    CK_ASSERT_INT_EQ(0, attrs[1].token.type);
    CK_ASSERT_INT_EQ(0, attrs[1].token.start);
    CK_ASSERT_INT_EQ(0, attrs[1].token.length);
    CK_ASSERT_STR_EQ("attr2", attrs[2].text);
    CK_ASSERT_STR_EQ("\"val2\"", attrs[3].text);
    CK_ASSERT_INT_EQ(1, text_found);
    CK_ASSERT_STR_EQ("subtext", texts[0].text);
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
    tcase_add_test(tc_xml, test_11);
    tcase_add_test(tc_xml, test_12);
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
