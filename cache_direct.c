/* 
 * File:   cache_direct.c
 * Author: Tobenna Okunna
           Nolan Bigsono 
 *
 * Created on April 22, 2019, 7:33 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <cache_direct.h>
#include <memory.c>
#include <storage.c>
#include <cache_defs.h> 
#include <assert.h>
#include <string.h>
#include <ctype.h>

/********************************
 *         Structs            *
 ********************************/



/* Block
 *
 * Holds an integer that states the validity of the bit (0 = invalid,
 * 1 = valid), the tag being held, and another integer that states if
 * the bit is dirty or not (0 = clean, 1 = dirty).
 */

struct Block_{
    int valid;
    char* tag;
    int dirty;
};

/* Cache
 *
 * Cache object that holds all the data about cache access as well as 
 * the write policy, sizes, and an array of blocks.
 *
 * @param   hits            # of cache accesses that hit valid data
 * @param   misses          # of cache accesses that missed valid data
 * @param   reads           # of reads from main memory
 * @param   writes          # of writes from main memory
 * @param   cache_size      Total size of the cache in bytes
 * @param   block_size      How big each block of data should be
 * @param   numLines        Total number of blocks
 * @param   blocks          The actual array of blocks  
 */

struct Cache_{
    int hits;
    int misses;
    int reads;
    int writes;
    int cache_size;
    int block_size;
    int numLines;
    struct Block_ blocks;
};

/********************************
 *     Cache Functions       *
 ********************************/

/* Function List:
 *
 * 1) createCache
 * 2) destroyCache
 * 3) readFromCache
 * 4) writeToCache
 * 5) printCache
 */


/* getTag
*
*  Function to retrieve the tag bits
* @parama memory Address
* @return tag in int
*/
int getTag(int mem){
    return mem & 0x0008;
}

/* getSet
*
*  Function to retrieve the set bits
* @parama memory Address
* @return set in int
*/
int getSet(int mem){
    return mem & 0x0004;
}


/* createCache
 *
 * Function to create a new cache struct.  Returns the new struct on success
 * and NULL on failure.
 *
 * @param   cache_size      size of cache in bytes
 * @param   block_size      size of each block in bytes
 * @param   write_policy    0 = write through, 1 = write back
 *
 * @return  success         new Cache
 * @return  failure         NULL
 */


struct Cache_ createCache(int cache_size, int block_size){
        //needed local variables
        struct Cache_ cache;
        int i, write_policy = 1;

        //Validate cache_size
        if(cache_size <= 0){
            fprintf(stderr, "Cache size must be greater than 0 bytes\n" );
           //return null
        }

        //Validate block_size
        if(block_size <= 0)
        {
        fprintf(stderr, "Block size must be greater than 0 bytes...\n");
        //return NULL;
        }

          cache = (Cache) malloc( sizeof( struct Cache_ ) );
        if(cache == NULL)
        {
        fprintf(stderr, "Could not allocate memory for cache.\n");
        //return NULL;
        }

            cache.hits = 0;
            cache.misses = 0;
            cache.reads = 0;
            cache.writes = 0;
            
            //cache.write_policy = 1;
            
            cache.cache_size = CACHE_SIZE;
            cache.block_size = BLOCK_SIZE;

            /* Calculate numLines */
            cache.numLines = num_lines;

            cache.blocks = (Block*) malloc( sizeof(Block) * cache.numLines );
            assert(cache.blocks != NULL);

                /* By default insert blocks where valid = 0 */
            for(i = 0; i < cache.numLines; i++)
            {
                //cache.blocks[i] = (Block) malloc( sizeof( struct Block_ ) );
                assert(cache.blocks[i] != NULL);
                cache.blocks[i]->valid = 0;
                cache.blocks[i]->dirty = 0;
                cache->blocks[i]->tag = NULL;
            }
                    



}

