//
// Created by johan on 2025-07-21.
//
#include "commons.h"

#include <assert.h>

void bdestroy_wrapper(void *b_string) {
    assert(bdestroy(b_string) == BSTR_OK);
}
