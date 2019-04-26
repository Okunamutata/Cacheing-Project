#!/bin/bash
export compile="gcc -std=c99 -Wall -c -g "

$compile -o -fPIC objs/storage.o           storage.c
$compile -o -fPIC objs/trace.o             trace.c
$compile -o -fPIC objs/memory.o            memory.c
$compile -o -fPic objs/cache_direct.o      cache_direct.c
$compile -o -fPic objs/cache_associative_full.o cache_associative_full.c
$compile -o -fPic objs/cache_associative_nway.o cache_associative_nway.c


gcc -shared -o libfoo.sc obj/*.o



gcc test1.c –o test1 -L. –lfoo

gcc test1a.c –o test1a -L. –lfoo

gcc test1a.c –o test1a -L. –lfoo

gcc test3.c -o test3 -L. -libfoo

gcc test_matrix_sum_16x16.c -o test_matrix_sum_16x16 -L. -libfoo

gcc test_matrix_sum_8x4.c -o test_matrix_sum_8x4 -L. -libfoo

gcc test_transpose_8x8.c -o test_transpose_8x8 -L. -libfoo

gcc test_transpose_8x8_improved.c -o test_transpose_8x8_improved -L. -libfoo

export LD_LIBRARY_PATH=/Users/tokunna/GitHub/Cacheing-Project/Cacheing-Project/libs

