#ifndef libfaafo_PTR_DEREF_H
#define libfaafo_PTR_DEREF_H

#define deref_void_ptr(ptr, type) *(type*) (ptr)

#define deref_char(ptr) deref_void_ptr(ptr, char)

#define deref_int(ptr) (*(int*)(ptr))

#define deref_string(ptr) ((char*)(ptr))

#define deref_float(ptr) (*(float*)(ptr))

#endif //libfaafo_PTR_DEREF_H
