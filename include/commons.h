//
// Created by johan on 2025-07-21.
//

#ifndef COMMONS_H
#define COMMONS_H

#define NOOP_DESTRUCTOR NULL
#include <bstrlib.h>

typedef void (*destructor_fn)(void *);

void bdestroy_wrapper(void *b_string);


#endif //COMMONS_H
