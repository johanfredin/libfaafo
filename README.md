# libfaafo

A C data structures library that started as exercises from Zed Shaw's **Learn C the Hard Way** course but evolved into its own educational project.

## Origins & Tribute

This project began as implementations of the data structures and algorithms from Zed Shaw's [Learn C the Hard Way](http://c.learncodethehardway.org/) course. 
You can find the original course materials and his `liblcthw` implementation at [https://github.com/zedshaw/liblcthw](https://github.com/zedshaw/liblcthw).

## Evolution

While starting with Zed's foundation, this library has grown beyond the original scope by:

- **Borrowing concepts from Java Collections**: Implementing familiar interfaces and behaviors from Java's collection framework.  
- **Using CMake**: Switched from traditional Makefiles to CMake for easier building
- **Unity testing framework**: Using Unity instead of minunit for testing
- **Extended functionality**: Adding features and optimizations as I learn more

#### Why based on Java?
Besides Java being everyones favorite language(right?), it is the language Im most familiar with, and it gave me an opportunity to 
learn how the collections work under the hood. Is it optimal for C? Most likely not but it was easier for me to use the same terminology

## Purpose

This is an **educational project** and **work in progress (WIP)**. I'm building this to:
- Learn C programming concepts
- Understand how data structures work under the hood
- Practice memory management in C
- Have fun with coding!

## Current Status

⚠️ **Work in Progress** - This library is actively being developed as I learn. Do whatever you want with it but don't expect magic.

⚠️ This project targets C99 and above. Some code and preprocessor directives only work with GCC and Clang. Sorry/Not sorry MSVC users.


## Data Structures Implemented

- ArrayList (dynamic array)
- HashMap (hash table)
- LinkedList 
- RadixMap
- And more...

## Building

This project uses CMake. To build:

```bash
mkdir build
cd build
cmake ..
make
```
