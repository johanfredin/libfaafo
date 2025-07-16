//
// Created by johan on 2025-07-14.
// Just testing bstring functions for practise
//

#include <stdlib.h>
#include <bstrlib.h>
#include <unity.h>

static bstring str;

void setUp(void) {
    str = bfromcstr("test");
}

void tearDown(void) {
    bdestroy(str);
}

void test_bfromcstr(void) {
    TEST_ASSERT_EQUAL_STRING("test", str->data);
}

void test_blk2bstr(void) {
    const const_bstring str2 = blk2bstr("test", 10);
    TEST_ASSERT_EQUAL_STRING("test", str2->data);
    TEST_ASSERT_EQUAL_INT(10, str2->slen);
    bdestroy(str2);
}

void test_bstrcpy(void) {
    const const_bstring copy = bstrcpy(str);
    TEST_ASSERT_EQUAL_STRING("test", copy->data);
    TEST_ASSERT_NOT_EQUAL(copy, str);
    bdestroy(copy);
}

void test_bassign(void) {
    const const_bstring s2 = bfromcstr("test2");
    bassign(str, s2);
    TEST_ASSERT_EQUAL_STRING("test2", str->data);
    bdestroy(s2);
}

void test_bassigncstr(void) {
    bassigncstr(str, "test3");
    TEST_ASSERT_EQUAL_STRING("test3", str->data);
}

void test_bassignblk(void) {
    bassignblk(str, "Hej hopp", 10);
    TEST_ASSERT_EQUAL_STRING("Hej hopp", str->data);
    TEST_ASSERT_EQUAL_INT(10, str->slen);
}

void test_bconcat(void) {
    const const_bstring append = bfromcstr("more");
    bconcat(str, append);
    TEST_ASSERT_EQUAL_STRING("testmore", str->data);
    bdestroy(append);
}

void test_bstricmp(void) {
    bstring s1 = bfromcstr("test");
    TEST_ASSERT_EQUAL_INT(0, bstricmp(str, s1));
    bdestroy(s1);
}

void test_biseq(void) {
    bstring s1 = bfromcstr("test");
    bstring s2 = bfromcstr("another");
    TEST_ASSERT_EQUAL(biseq(str, s1), 1);
    TEST_ASSERT_EQUAL(biseq(str, s2), 0);
    bdestroy(s1);
    bdestroy(s2);
}

void test_binstr(void) {
    bstring s1 = bfromcstr("st");
    bstring s2 = bfromcstr("anothther");
    bstring s3 = bfromcstr("ST");

    TEST_ASSERT_EQUAL(binstr(str, 0, s1), 2);
    TEST_ASSERT_EQUAL(binstr(str, 0, s2), BSTR_ERR);
    TEST_ASSERT_EQUAL(binstrcaseless(str, 0, s3), 2);
    TEST_ASSERT_EQUAL(binstr(str, 0, s3), BSTR_ERR);

    bdestroy(s1);
    bdestroy(s2);
    bdestroy(s3);
}

void test_bfindreplace(void) {
    // Set up
    const const_bstring find = bfromcstr("st");
    const const_bstring replace = bfromcstr("yomama");
    const const_bstring nodice = bfromcstr("meh?");

    const int replaced = bfindreplace(str, find, replace, 2);
    TEST_ASSERT_EQUAL(BSTR_OK, replaced);
    TEST_ASSERT_EQUAL_STRING("teyomama", str->data);
    bfindreplace(str, nodice, nodice, 5);
    TEST_ASSERT_EQUAL_STRING("teyomama", str->data);

    // What happens if out of bounds?
    const int wayoff = blength(str) + 10;
    bfindreplace(str, replace, replace, wayoff);
    TEST_ASSERT_EQUAL_STRING("teyomama", str->data);

    bdestroy(find);
    bdestroy(replace);
    bdestroy(nodice);
}

void test_bsplit(void) {
    // Set up
    bassigncstr(str, "test1,test2,test3");

    // Act
    struct bstrList *list = bsplit(str, ',');

    // Verify
    TEST_ASSERT_EQUAL(list->qty, 3);
    TEST_ASSERT_EQUAL_STRING("test1", list->entry[0]->data);
    TEST_ASSERT_EQUAL_STRING("test2", list->entry[1]->data);
    TEST_ASSERT_EQUAL_STRING("test3", list->entry[2]->data);

    bstrListDestroy(list);
}

void test_bformat(void) {
    const const_bstring str = bformat("Hello %s you are %d years old", "Johan", 400);
    TEST_ASSERT_EQUAL_STRING("Hello Johan you are 400 years old", str->data);
    bdestroy(str);
}

void test_bchar(void) {
    const char c = bchar(str, 0);
    TEST_ASSERT_EQUAL(c, 't');
    const char hmm = bchar(str, 16);
    TEST_ASSERT_EQUAL(hmm, '\0');
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_bfromcstr);
    RUN_TEST(test_blk2bstr);
    RUN_TEST(test_bstrcpy);
    RUN_TEST(test_bassign);
    RUN_TEST(test_bassigncstr);
    RUN_TEST(test_bassignblk);
    RUN_TEST(test_bconcat);
    RUN_TEST(test_bstricmp);
    RUN_TEST(test_biseq);
    RUN_TEST(test_binstr);
    RUN_TEST(test_bfindreplace);
    RUN_TEST(test_bsplit);
    RUN_TEST(test_bformat);
    RUN_TEST(test_bchar);
    return UNITY_END();
}
