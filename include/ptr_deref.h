#ifndef libfaafo_PTR_DEREF_H
#define libfaafo_PTR_DEREF_H

#define deref_void_ptr(ptr, type) *(type*) (ptr)

#define deref_char(ptr) deref_void_ptr(ptr, char)

#define deref_int(ptr) (*(int*)(ptr))


#define deref_string(ptr) ((char*)(ptr))
#endif //libfaafo_PTR_DEREF_H
