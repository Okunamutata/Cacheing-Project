#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cache_associative_full.h"

struct NWAY_CACHE{
    bool valid;
    bool dirty;
    int tag;
    unsigned char block[CACHE_BLOCK_SIZE];
    double timestamp;
}fa_cache[CACHE_ENTRIES];

static int hits,
           misses;

static double time;

void cache_associative_full_init(){
    int i;
    for(i = 0; i < CACHE_ENTRIES; i++){
        fa_cache[i].valid = false;
        fa_cache[i].dirty = false;
        fa_cache[i].timestamp = 0;
    }

    hits = 0;
    misses = 0;
    time = 0;
}

int cache_associative_full_load(memory_address addr){
    int off =  addr       & 0xF;
    int tag = (addr >> 4) & 0xFFFFFFF;

    unsigned int value;

    int i;
    for(i = 0; i < CACHE_ENTRIES; i++){
        if(fa_cache[i].valid && fa_cache[i].tag == tag){
            hits++;
            memcpy(&value, &fa_cache[i].block[off], OFFSET_BITS);
            fa_cache[i].timestamp = time++;
            fa_cache[i].valid = true;
            fa_cache[i].dirty = true;
            return value;
        }
    }

    misses++;
    for(i = 0; i < CACHE_ENTRIES; i++){
        if(!fa_cache[i].valid){
            storage_load_line((addr & ~0xF), fa_cache[i].block);
            memcpy(&value, &fa_cache[i].block[off], OFFSET_BITS);	
            fa_cache[i].timestamp = time++;
            fa_cache[i].valid = true;
            fa_cache[i].dirty = true;
            fa_cache[i].tag = tag;
            return value;
        }
    }

    double least_used = fa_cache[0].timestamp;
    int location = 0;
    for(i = 0; i < CACHE_ENTRIES; i++){
        if(least_used > fa_cache[i].timestamp){
            least_used = fa_cache[i].timestamp;
            location = i;
        }
    }

    if(fa_cache[location].dirty){
        storage_store_line((fa_cache[location].tag << 4), fa_cache[location].block);
        fa_cache[location].dirty = false;
    }
    storage_load_line((addr & ~0xF), fa_cache[location].block);
    memcpy(&value, &fa_cache[location].block[off], OFFSET_BITS);
    fa_cache[location].timestamp = time++;
    fa_cache[location].valid = true;
    fa_cache[location].dirty = true;
    fa_cache[location].tag = tag;
    return value;
}

void cache_associative_full_store(memory_address addr, int value){
    int off =  addr       & 0xF;
    int tag = (addr >> 4) & 0xFFFFFFF;

    int i;
    for(i = 0; i < CACHE_ENTRIES; i++){
        if(fa_cache[i].valid && fa_cache[i].tag == tag){
            hits++;
            memcpy(&fa_cache[i].block[off], &value, OFFSET_BITS);
            fa_cache[i].timestamp = time++;
            fa_cache[i].valid = true;
            fa_cache[i].dirty = true;
            return;
        }
    }

    misses++;
    for(i = 0; i < CACHE_ENTRIES; i++){
        if(!fa_cache[i].valid){
            storage_load_line((addr & ~0xF), fa_cache[i].block);
            memcpy(&fa_cache[i].block[off], &value, OFFSET_BITS);            fa_cache[i].timestamp = time++;
            fa_cache[i].valid = true;
            fa_cache[i].dirty = true;
            fa_cache[i].tag = tag;
            return;
        }
    }

    double least_used = fa_cache[0].timestamp;
    int location = 0;
    for(i = 0; i < CACHE_ENTRIES; i++){
        if(least_used > fa_cache[i].timestamp){
            least_used = fa_cache[i].timestamp;
            location = i;
        }
    }

    if(fa_cache[location].dirty){
        storage_store_line((fa_cache[location].tag << 4), fa_cache[location].block);
        fa_cache[location].dirty = false;
    }
    storage_load_line((addr & ~0xF), fa_cache[location].block);
    memcpy(&fa_cache[i].block[off], &value, OFFSET_BITS);
    fa_cache[location].timestamp = time++;
    fa_cache[location].valid = true;
    fa_cache[location].dirty = true;
    fa_cache[location].tag = tag;
    return;
}

void cache_associative_full_flush(){
    memory_address addr;
    int i;
    for(i = 0; i < CACHE_ENTRIES; i++){
        addr = fa_cache[i].tag << 4;
        storage_store_line(addr, fa_cache[i].block);
        fa_cache[i].dirty = false;
    }
}

void cache_associative_nway_stats(){
    printf("cache direct stats:  hits: %d  misses: %d", hits, misses);
}