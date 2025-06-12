#ifndef DEFER_H
#define DEFER_H

/*
 * This header provides 'defer' and 'errdefer' functionality similar to Java's try-with-resources
 * but using C compiler features. It allows you to specify cleanup code right after resource
 * acquisition, which will automatically run when the current scope ends.
 */

#if defined(__GNUC__) && !defined(__clang__)
/* GCC implementation: Uses __cleanup__ attribute to register a function that runs when a variable
 * goes out of scope, similar to a destructor in Java but at a lower level */
#define DEFER_(F, V)                                                                               \
auto inline __attribute__((__always_inline__)) void F(int *);                                  \
int V __attribute__((__cleanup__(F)));                                                         \
void F(__attribute__((__unused__)) int *_)
#define DEFERER_(L, C) DEFER_(DEFER_FUNC_##L##C, DEFER_VAR_##L##C)

#elif defined(__clang__) && defined(__BLOCKS__)
/* Clang implementation: Uses Apple's Blocks extension (similar to Java lambdas) 
 * to create a cleanup function that executes the deferred code */
#include <Block.h>
static inline __attribute__((__always_inline__)) void defer_cleanup_(void (^*block)(void)) { (*block)(); }
#define DEFER_(B) __attribute__((__cleanup__(defer_cleanup_))) void (^B)(void) = ^
#define DEFERER_(L, C) DEFER_(DEFER_BLOCK_##L##C)

#else
#error "defer is not supported on this compiler"
#endif

#define DEFERER(L, C) DEFERER_(L, C)

/* Usage example for defer:
 * FILE* file = fopen("example.txt", "r");
 * defer { 
 *     fclose(file); 
 * }
 * // do work with file
 * // fclose(file) will automatically be called when scope ends
 */
#define defer DEFERER(__LINE__, __COUNTER__)

/* errdefer executes cleanup code only if an error occurred.
 * Example usage:
 * bool error = false;
 * void* memory = malloc(1000);
 * errdefer(error, {
 *     free(memory);
 * })
 * // if error becomes true, memory will be freed when scope ends
 * // if no error occurs, memory won't be freed
 */
#define errdefer(err, ...)                                                                         \
defer {                                                                                        \
if (unlikely(err))                                                                         \
__VA_ARGS__;                                                                           \
}

#endif //DEFER_H