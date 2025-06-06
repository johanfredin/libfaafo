//
// Created by johan on 2025-06-05.
//

#ifndef PTR_DEREF_H
#define PTR_DEREF_H

#define deref_void_ptr(ptr, type) *(type*) (ptr)

#define deref_int(ptr) deref_void_ptr(ptr, int)
#define deref_string(ptr) deref_void_ptr(ptr, char)

#endif //PTR_DEREF_H
