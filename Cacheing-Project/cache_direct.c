#include "storage.h"
#include "cache_direct.h"
#include <stdio.h>

struct Block{
    int valid;
    int tag;
    int dirty;
    int data[4];
};

static struct Block block[16];
static int hits;
static int misses;

void cache_direct_init(){
    int i, j;
    for(i = 0; i < CACHE_ENTRIES; i++){
        block[i].valid = 0;
        block[i].tag = 0;
        block[i].dirty = 0;
        for(j = 0; j < CACHE_BLOCK_SIZE; j++){
            block[i].data[j] = 0;
        }
    }

    hits = 0;
    misses = 0;
}

int cache_direct_load(memory_address addr){
    int tag = addr & 0xFFFFFF00;
    int set = addr & 0x000000F0;
    int off = addr & 0x0000000F;

    if(block[set].valid == 1 && block[set].tag == tag){
        hits++;
        return block[set].data[off];
    }else{
        misses++;
        return storage_load(addr);
    }
}

void cache_direct_store(memory_address addr, int value){
    int tag = (addr & 0xFFFFFF00) >> 8;
    int set = (addr & 0x000000F0) >> 4;
    int off = addr & 0x0000000F;

    if(block[set].valid == 1 && block[set].tag == tag){
        hits++;
        block[set].data[off] = value;
    }else{
        misses++;
        block[set].tag = tag;
        block[set].valid = 1;
        block[set].data[off] = value;
    }

    block[set].dirty = 1;
}

void cache_direct_flush(){
    memory_address addr;
    int set, off;
    for(set = 0; set < CACHE_ENTRIES; set++){
        if(block[set].dirty == 1){
            int tag = block[set].tag;
            for(off = 0; off < OFFSET_BITS; off++){
                addr = (tag << 8) & (set << 4) & off;
                storage_store(addr, block[set].data[off]);
            }
            block[set].dirty = 0;
        }
    }
}

void cache_direct_stats(){
    printf("cache direct stats:  hits: %d  misses: %d", hits, misses);
}