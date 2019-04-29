#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cache_direct.h"

/*
*   structure for the cache entries
*   held in an array 
*/
struct DIRECT_CACHE{
    bool valid;
    bool dirty;
    int tag;
    unsigned char block[CACHE_BLOCK_SIZE]; 
} d_cache[CACHE_ENTRIES];

static int hits,
           misses;


//initialize the cache
void cache_direct_init(){
    int i;
    for(i = 0; i < CACHE_ENTRIES; i++){
        d_cache[i].valid = false;
        d_cache[i].dirty = false;
    }

    hits = 0;
    misses = 0;
}

// save data from given memory address
int cache_direct_load(memory_address addr){
    int off =  addr       & 0xF;
    int set = (addr >> 4) & 0xF;
    int tag = (addr >> 8) & 0xFFFFFF;

    unsigned int value;

    if(d_cache[set].valid && d_cache[set].tag == tag){
        hits++;
    }else{
        misses++;
        if(d_cache[set].dirty){
            storage_store_line((addr & ~0xF), d_cache[set].block);
            d_cache[set].dirty = false;
        }
        storage_load_line((addr & ~0xF), d_cache[set].block);
        d_cache[set].valid = true;
        d_cache[set].tag = tag;
    }
    memcpy(&value, &d_cache[set].block[off], OFFSET_BITS); 
    return value;
}

void cache_direct_store(memory_address addr, int value){
    int off =  addr       & 0xF;
    int set = (addr >> 4) & 0xF;
    int tag = (addr >> 8) & 0xFFFFFF;

    if(d_cache[set].valid && d_cache[set].tag == tag){
        hits++;
        memcpy(&d_cache[set].block[off], &value, OFFSET_BITS);
        d_cache[set].dirty = true;
    }else{
        misses++;
        if(d_cache[set].dirty){
            storage_store_line((addr & ~0xF), d_cache[set].block);
            d_cache[set].dirty = false;
        }
        storage_load_line((addr & ~0xF), d_cache[set].block);
        stroage_store_line((addr & ~0xF ), d_cache[set].block);
        memcpy(&d_cache[set].block[off], &value, sizeof(value));
        d_cache[set].valid = true;
		d_cache[set].dirty = true;
		d_cache[set].tag = tag;
    }
}

void cache_direct_flush(){
    memory_address addr;
    int set;
    for(set = 0; set < CACHE_ENTRIES; set++){
        addr = (d_cache[set].tag << 4);
        storage_store_line(addr, d_cache[set].block);
        d_cache[set].dirty = false;
    }
}

void cache_direct_stats(){
    printf("cache direct stats:  hits: %d  misses: %d", hits, misses);
}