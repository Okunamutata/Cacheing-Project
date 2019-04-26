#!/bin/bash
export compile="gcc -std=c99 -Wall -c -g -fPIC"

$compile -o objs/storage.o           storage.c
$compile -o objs/trace.o             trace.c
$compile -o objs/memory.o            memory.c
$compile -o objs/cache_direct.o      cache_direct.c
$compile -o objs/cache_associative_full.o cache_associative_full.c
$compile -o objs/cache_associative_nway.o cache_associative_nway.c

gcc -shared -o libtest.so objs/*.o

gcc -std=c99 -Wall -g  \
    -o test1 test1.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest

gcc -std=c99 -Wall -g  \
    -o test1a test1a.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest
gcc -std=c99 -Wall -g \
    -o test2 test2.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest

gcc -std=c99 -Wall -g \
    -o test3 test3.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest

gcc -std=c99 -Wall -g \
    -o test_matrix_sum_16x16 test_matrix_sum_16x16.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest

gcc -std=c99 -Wall -g \
    -o test_matrix_sum_8x4 test_matrix_sum_8x4.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest

gcc -std=c99 -Wall -g \
    -o test_transpose_8x8 test_transpose_8x8.c \
    -L./Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test -ltest

gcc -std=c99 -Wall -g \
    -o test_transpose_8x8_improved test_transpose_8x8_improved.c \
    -L/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/test 
    -ltest



export LD_LIBRARY_PATH=/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project