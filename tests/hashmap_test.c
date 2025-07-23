//
// Created by johan on 2025-07-23.
//
#include <unity.h>
#include <hashmap.h>
#include "testutil.h"

static HashMap *static_map;

void setUp(void) {
}

void tearDown(void) {
    HashMap_destroy(static_map);
}

int main(void) {
    UNITY_BEGIN();
    return UNITY_END();
}