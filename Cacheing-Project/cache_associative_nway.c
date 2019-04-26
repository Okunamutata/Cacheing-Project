#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cache_associative_nway.h"

#define SETS (CACHE_ENTRIES / 2)

struct NWAY_CACHE{
    bool valid;
    bool dirty;
    int tag;
    unsigned char block[CACHE_BLOCK_SIZE];
    double timestamp;
}na_cache[SETS][2];

static int hits,
           misses;

static double time;

void cache_associative_nway_init(){
    int i, j;
    for(i = 0; i < SETS; i++){
        for(j = 0; j < 2; j++){
            na_cache[i][j].valid = false;
            na_cache[i][j].dirty = false;
            na_cache[i][j].timestamp = 0;
        }
    }

    hits = 0;
    misses = 0;
    time = 0;
}

int cache_associative_nway_load(memory_address addr){
    int off =  addr       & 0xF;
    int set = (addr >> 4) & 0xF;
    int tag = (addr >> 8) & 0xFFFFFF;

    unsigned int value;

    int i;
    for(i = 0; i < 2; i++){
        if(na_cache[set][i].valid && na_cache[set][i].tag == tag){
            hits++;
            memcpy(&value, &na_cache[set][i].block[off], OFFSET_BITS);
            na_cache[set][i].timestamp = time++;
            na_cache[set][i].valid = true;
            na_cache[set][i].dirty = true;
            return value;
        }
    }

    misses++;
    for(i = 0; i < 2; i++){
        if(!na_cache[set][i].valid){
            storage_load_line((addr & ~0xF),na_cache[set][i].block);
            memcpy(&value, &na_cache[set][i].block[off], OFFSET_BITS);
            na_cache[set][i].timestamp = time++;
            na_cache[set][i].valid = true;
            na_cache[set][i].dirty = true;
            na_cache[set][i].tag = tag;
            return value;
        }
    }

    int least_used;
    if(na_cache[set][0].timestamp < na_cache[set][1].timestamp){
        least_used = 0;
    }else{
        least_used = 1;
    }
    if(na_cache[set][least_used].dirty){
        storage_store_line((addr  & ~0xF), na_cache[set][least_used].block);
        na_cache[set][least_used].dirty = false;
    }
    storage_load_line((addr & ~0xF), na_cache[set][least_used].block);
    memcpy(&value, &na_cache[set][least_used].block[off], OFFSET_BITS);
    na_cache[set][i].timestamp = time++;
    na_cache[set][i].valid = true;
    na_cache[set][i].dirty = true;
    na_cache[set][i].tag = tag;
    return value;
}

void cache_associative_nway_store(memory_address addr, int value){
    int off =  addr       & 0xF;
    int set = (addr >> 4) & 0xF;
    int tag = (addr >> 8) & 0xFFFFFF;

    int i;
    for(i = 0; i < 2; i++){
        if(na_cache[set][i].valid && na_cache[set][i].tag == tag){
            hits++;
            memcpy(&na_cache[set][i].block[off], &value, OFFSET_BITS);
            na_cache[set][i].timestamp = time++;
            na_cache[set][i].valid = true;
            na_cache[set][i].dirty = true;
            return;
        }
    }

    misses++;
    for(i = 0; i < 2; i++){
        if(!na_cache[set][i].valid){
            storage_load_line((addr & ~0xF),na_cache[set][i].block);
            memcpy(&na_cache[set][i].block[off], &value, OFFSET_BITS);
            na_cache[set][i].timestamp = time++;
            na_cache[set][i].valid = true;
            na_cache[set][i].dirty = true;
            na_cache[set][i].tag = tag;
            return;
        }
    }

    int least_used;
    if(na_cache[set][0].timestamp < na_cache[set][1].timestamp){
        least_used = 0;
    }else{
        least_used = 1;
    }
    if(na_cache[set][least_used].dirty){
        storage_store_line((addr  & ~0xF), na_cache[set][least_used].block);
        na_cache[set][least_used].dirty = false;
    }
    storage_load_line((addr & ~0xF), na_cache[set][least_used].block);
    memcpy(&na_cache[set][least_used].block[off], &value, OFFSET_BITS);
    na_cache[set][i].timestamp = time++;
    na_cache[set][i].valid = true;
    na_cache[set][i].dirty = true;
    na_cache[set][i].tag = tag;
    return;
}

void cache_associative_nway_flush(){
    memory_address addr;
    int set, line;
    for(set = 0; set < SETS; set++){
        for(line = 0; line < 2; line++){
            addr = na_cache[set][line].tag << 4;
            addr |= (set & 0xF);
            addr = addr << 4;
            storage_store_line(addr, na_cache[set][line].block);
            na_cache[set][line].dirty = false;
        }
    }
}

void cache_associative_nway_stats(){
    printf("cache direct stats:  hits: %d  misses: %d", hits, misses);
}