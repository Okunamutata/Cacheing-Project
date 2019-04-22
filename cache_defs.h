/* 
 * File:   cache_defs.h
 * Author: Tobenna Okunna
           Nolan Bigsono 
 *
 * Created on April 22, 2019, 7:33 PM
 */

 /*  Type definistions */
typedef struct Cache_* Cache;
typedef struct Block_* Block;

/* Cache Sizes (in bytes) */
#define CACHE_SIZE 256
#define BLOCK_SIZE 16
#define num_lines 16
/* Block Sizes (in bytes) */
#define TAG 8
#define INDEX 4 
#define OFFSET 4 