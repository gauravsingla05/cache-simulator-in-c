/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

// Author - Gourav Singla
// # R11800214
// gsingla@ttu.edu


#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "cachesim.h"

int main(int argc, char *argv[])
{

    if (argc != 4) {
        printf("Usage: %s <single-level or two-level cache> <direct> <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    char* trace_file_name = argv[3];
    char* level_of_cache = argv[1];
    struct direct_mapped_cache d_cache;
    struct l1_mapped_cache l1MappedCache;
    struct l2_mapped_cache l2MappedCache;

    char mem_request[20];
    uint64_t address;
    FILE *fp;


    /* Initialization */
    for (int i=0; i<NUM_BLOCKS; i++) {
        d_cache.valid_field[i] = 0;
        d_cache.dirty_field[i] = 0;
        d_cache.tag_field[i] = 0;
    }

    for (int i=0; i<L1_NUM_BLOCKS; i++) {
        l1MappedCache.valid_field[i] = 0;
        l1MappedCache.dirty_field[i] = 0;
        l1MappedCache.tag_field[i] = 0;
    }

    for (int i=0; i<L2_NUM_BLOCKS; i++) {
        l2MappedCache.valid_field[i] = 0;
        l2MappedCache.dirty_field[i] = 0;
        l2MappedCache.tag_field[i] = 0;
    }

    for (int i = 0; i < NUM_SETS; ++i) {
        for (int j = 0; j < WAY_SIZE; ++j) {
            d_cache.set[i][j]=0;
        }
    }


    for (int i = 0; i < L1_NUM_SETS; ++i) {
        for (int j = 0; j < L1_WAY_SIZE; ++j) {
            l1MappedCache.set[i][j]=0;
        }
    }

    for (int i = 0; i < L2_NUM_SETS; ++i) {
        for (int j = 0; j < L2_WAY_SIZE; ++j) {
            l2MappedCache.set[i][j]=0;
        }
    }

    d_cache.hits = 0;
    d_cache.misses = 0;
    l1MappedCache.hits=0;
    l2MappedCache.misses=0;

    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");

    if (strncmp(argv[2], "direct", 6)==0) { /* Simulating direct-mapped cache */
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            direct_mapped_cache_access(&d_cache, address,level_of_cache, &l1MappedCache, &l2MappedCache);
        }

        /* Print out the results for single-level cache */
        if (strncmp(level_of_cache, "single-level", 6)==0) {
            double totalMemAcesss = d_cache.hits + d_cache.misses;
            double missRatio = (d_cache.misses )/ totalMemAcesss;
            double hitRatio = 1-missRatio;
            printf("\n==================================\n");
            printf("Cache type:  SINGLE LEVEL Direct-Mapped Cache\n");
            printf("==================================\n");
            printf("Cache Hits:    %d\n", d_cache.hits);
            printf("Cache Misses:  %d\n", d_cache.misses);
            printf("Total Access: %f\n", totalMemAcesss);
            printf("Cache Hit rate: %f\n", hitRatio);
            printf("Cache miss rate: %f\n", missRatio);
            printf("\n");

        }
            /* Print out the results for two-level cache */

        else{
            double l1totalMemAcesss = l1MappedCache.hits + l1MappedCache.misses;
            double l2totalMemAcesss = l2MappedCache.hits + l2MappedCache.misses;
            double totalAccess = l1totalMemAcesss+l2totalMemAcesss;
            double l2Access = l1MappedCache.misses;
            double l2Misses =  l2MappedCache.misses;
            double l2GlobalMissRate = l2Misses/totalAccess;
            double l2LocalMissRate = l2Access/totalAccess;
            double l2GlobalHitRate = 1-l2GlobalMissRate;
            double l2LocalHitRate = 1-l2LocalMissRate;

            printf("\n==================================\n");
            printf("Cache type:    TWO-LEVEL Direct-Mapped Cache\n");
            printf("==================================\n");
            printf("L2 global miss rate:    %f\n", l2GlobalMissRate);
            printf("L2 local miss rate:    %f\n", l2LocalMissRate);
            printf("L2 global hit rate:    %f\n", l2GlobalHitRate);
            printf("L2 local hit rate:    %f\n", l2LocalHitRate);
            printf("\n");

        }

    }
   else if (strncmp(argv[2], "n-way", 6)==0) { /* Simulating n-way cache */
        /* Read the memory request address and access the cache */

        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            nway_mapped_cache_access(&d_cache, address,level_of_cache,&l1MappedCache, &l2MappedCache);
        }
        /* Print out the results for single-level cache */

        if (strncmp(level_of_cache, "single-level", 6)==0) {
            double totalMemAcesss = d_cache.hits + d_cache.misses;
            double missRatio = d_cache.misses / totalMemAcesss;
            double hitRatio = d_cache.hits / totalMemAcesss;
            /*Print out the results*/
            printf("\n==================================\n");
            printf("Cache type:   single level n-way Mapped Cache\n");
            printf("==================================\n");
            printf("Cache Hits:    %d\n", d_cache.hits);
            printf("Cache Misses:  %d\n", d_cache.misses);
            printf("Total Access: %f\n", totalMemAcesss);
            printf("Cache Hit rate: %f\n", hitRatio);
            printf("Cache miss rate: %f\n", missRatio);
            printf("\n");
        }
            /* Print out the results for two-level cache */

        else{
            double l1totalMemAcesss = l1MappedCache.hits + l1MappedCache.misses;
            double l2totalMemAcesss = l2MappedCache.hits + l2MappedCache.misses;
            double totalAccess = l1totalMemAcesss+l2totalMemAcesss;
            double l2Access = l1MappedCache.misses;
            double l2Misses =  l2MappedCache.misses;
            double l2GlobalMissRate = l2Misses/totalAccess;
            double l2LocalMissRate = l2Misses/totalAccess;
            double l2GlobalHitRate = 1-l2GlobalMissRate;
            double l2LocalHitRate = 1-l2LocalMissRate;

            printf("\n==================================\n");
            printf("Cache type:    TWO-LEVEL N-WAY Mapped Cache\n");
            printf("==================================\n");
            printf("L2 global miss rate:    %f\n", l2GlobalMissRate);
            printf("L2 local miss rate:    %f\n", l2LocalMissRate);
            printf("L2 global hit rate:    %f\n", l2GlobalHitRate);
            printf("L2 local hit rate:    %f\n", l2LocalHitRate);
            printf("\n");
        }

    }

    else if (strncmp(argv[2], "fully-associative", 20)==0) { /* Simulating n-way cache */
        /* Read the memory request address and access the cache */
        int cnt=0;
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            fully_associative_cache_access(&d_cache, address,cnt);
            cnt++;
        }
        double totalMemAcesss = d_cache.hits + d_cache.misses;
        double missRatio = (d_cache.misses )/ totalMemAcesss;
        double hitRatio = (d_cache.hits ) / totalMemAcesss;
        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    fully-associative Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", d_cache.hits);
        printf("Cache Misses:  %d\n", d_cache.misses);
        printf("Total Access: %f\n", totalMemAcesss);
        printf("Cache Hit rate: %f\n", hitRatio);
        printf("Cache miss rate: %f\n", missRatio);
        printf("\n");
    }

    fclose(fp);

    return 0;
}

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}

void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address, char* level,struct l1_mapped_cache *l1_cache,struct l2_mapped_cache *l2_cache)
{

    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);

    // L1 LEVEL CACHE BLOG ADDRESS, INDEX, TAG
    uint64_t l1_block_addr = address >> (unsigned)log2(L1_BLOCK_SIZE);
    uint64_t l1_index = l1_block_addr % L1_NUM_BLOCKS;
    uint64_t l1_tag = l1_block_addr >> (unsigned)log2(L1_NUM_BLOCKS);

    // L2 LEVEL CACHE BLOG ADDRESS, INDEX, TAG
    uint64_t l2_block_addr = address >> (unsigned)log2(L2_BLOCK_SIZE);
    uint64_t l2_index = l2_block_addr % L2_NUM_BLOCKS;
    uint64_t l2_tag = l2_block_addr >> (unsigned)log2(L2_NUM_BLOCKS);



#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif
  // SINGLE LEVEL CACHE SIMULATION FOR DIRECT MAPPING
    if(strncmp(level, "single-level",6)==0){
        if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
            cache->hits += 1;
#ifdef DBG
            printf("Hit!\n");
#endif
        } else {
            /* Cache miss */
            cache->misses += 1;
#ifdef DBG
            printf("Miss!\n");
#endif
            if (cache->valid_field[index] && cache->dirty_field[index]) {
                /* Write the cache block back to memory */
            }
            cache->tag_field[index] = tag;
            cache->valid_field[index] = 1;
            cache->dirty_field[index] = 0;
        }
    }
    // TWO LEVEL CACHE SIMULATION FOR DIRECT MAPPING
    else{
         /*
          TRYING TO FIND IN L1 CACHE,
          If not found in L1, It is miss of L1 and then will try to find in L2
        */
         if (l1_cache->valid_field[l1_index] && l1_cache->tag_field[l1_index] == l1_tag) {
            l1_cache->hits += 1;
        }
        // MISSED IN L1 CACHE, SO WILL TRY TO FIND IN L2 NOW
        else{
            l1_cache->misses+=1;
        // FOUND IN L2 CACHE, It is L2 hit
            if (l2_cache->valid_field[l2_index] && l2_cache->tag_field[l2_index] == l1_tag) {
                // ITS A HIT IN L2, SO NOW WILL WRITE SAME DATA IN L1 ALSO
                l2_cache->hits+=1;
                l1_cache->tag_field[l1_index] = l1_tag;
                l1_cache->valid_field[l1_index] = 1;
                l1_cache->dirty_field[l1_index] = 0;
            }

            /*
           MISSED IN L2 CACHE ALSO, THEREFORE IT IS ACTUAL MISS OR GLOBAL MISS
           SO NOW WILL WRITE MISSED DATA IN L1 AND L2 BOTH
           */
            else{
                l2_cache->misses+=1;
                l1_cache->tag_field[l1_index] = l1_tag;
                l1_cache->valid_field[l1_index] = 1;
                l1_cache->dirty_field[l1_index] = 0;

                l2_cache->tag_field[l2_index] = l1_tag;
                l2_cache->valid_field[l2_index] = 1;
                l2_cache->dirty_field[l2_index] = 0;
            }
        }
    }


}

// Function for n-way mapping
// If way_size is 1, It will give same result as direct mapping
void nway_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address, char* type,struct l1_mapped_cache *l1_cache,struct l2_mapped_cache *l2_cache)
{
    if(strncmp(type,"single-level",6)==0){
        uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);

        // Finding how many bits are there in index
        int nindex_bits = log2(NUM_SETS);

        // getting last index bits
        uint64_t mask = (1 << nindex_bits) - 1;

        // Extracting index from block address
        uint64_t n_way_index = block_addr & mask;

        // Extracting tag from block address
        uint64_t n_way_tag = block_addr >> nindex_bits;

        // finding set number
        int set_no = block_addr % NUM_SETS;


#ifdef DBG
        printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

        if (cache->valid_field[n_way_index] && cache->tag_field[n_way_index] == n_way_tag) { /* Cache hit */

            // Running loop on all sub-sets of way of sets
            // If any of sub-set found same n_way_index
            // It is hit
            for (int i = 0; i < WAY_SIZE; ++i) {
                if(cache->set[set_no][i]==n_way_index){
                    cache->hits += 1;

                }
            }


#ifdef DBG
            printf("Hit!\n");
#endif
        } else {
            /* Cache miss */
            cache->misses += 1;
#ifdef DBG
            printf("Miss!\n");
#endif

            cache->tag_field[n_way_index] = n_way_tag;
            cache->valid_field[n_way_index] = 1;
            cache->dirty_field[n_way_index] = 0;

            // Because it is a miss,
            // Have to write data in sub-set of a set
            for (int i = 0; i < WAY_SIZE; i++) {
                // Condition to check if any sub-set is empty
                if(cache->set[set_no][i]==0){
                    // sub-set is not full
                    cache->set[set_no][i]=n_way_index;
                    break;
                }
                // No sub-set of n-set is empty. It is full
                if(i==WAY_SIZE-1){
                    // Generating random number upto WAY_SIZE
                    int random = rand()%WAY_SIZE;
                    cache->set[set_no][random]=n_way_index;
                    break;
                }
            }





        }
    }
    else{
        uint64_t l1_block_addr = address >> (unsigned)log2(L1_BLOCK_SIZE);
        int l1_nindex_bits = log2(L1_NUM_SETS);

        // getting last nindex_bits bits
        uint64_t mask = (1 << l1_nindex_bits) - 1;

        // Extracting index from block address
        uint64_t l1_n_way_index = l1_block_addr & mask;

        // Extracting tag from block address
        uint64_t l1_n_way_tag = l1_block_addr >> l1_nindex_bits;

        // finding number of set
        int l1_set_no = l1_block_addr % NUM_SETS;

        // L2 INITIALIZATION
        uint64_t l2_block_addr = address >> (unsigned)log2(L2_BLOCK_SIZE);

        int l2_nindex_bits = log2(L2_NUM_SETS);

        // getting last nindex_bits bits
        uint64_t l2_mask = (1 << l2_nindex_bits) - 1;

        // Extracting index from block address
        uint64_t l2_n_way_index = l2_block_addr & l2_mask;

        // Extracting tag from block address
        uint64_t l2_n_way_tag = l2_block_addr >> l2_nindex_bits;

        // finding number of set
        int l2_set_no = l2_block_addr % L2_NUM_SETS;



#ifdef DBG
        printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif
        // TRYING TO FIND IN L1 CACHE, IF NOT FOUND, WILL TRY IN L2 CACHE

        if (l1_cache->valid_field[l1_n_way_index] && l1_cache->tag_field[l1_n_way_index] == l1_n_way_tag) { /* Cache hit */

            // Running loop on all sub-sets or way of sets
            // If any of sub-set found same l1_n_way_index
            // It is hit
            for (int i = 0; i < L1_WAY_SIZE; ++i) {
                if(l1_cache->set[l1_set_no][i]==l1_n_way_index){
                    l1_cache->hits += 1;
                }
            }

        }
        // TRYING TO FIND IN L2 CACHE
        else {
            /* Cache miss */
            l1_cache->misses += 1;

            if (l2_cache->valid_field[l2_n_way_index] && l2_cache->tag_field[l2_n_way_index] == l1_n_way_tag) { /* Cache hit */
                for (int i = 0; i < L2_WAY_SIZE; ++i) {
                    if (l2_cache->set[l2_set_no][i] == l1_n_way_index) {
                        l2_cache->hits += 1;
                    }
                }
               // BECAUSE IT IS MISS IN L1
              // WRITE SAME MISSED DATA IN L1
                l1_cache->tag_field[l1_n_way_index] = l1_n_way_tag;
                l1_cache->valid_field[l1_n_way_index] = 1;
                l1_cache->dirty_field[l1_n_way_index] = 0;

                for (int i = 0; i < L1_WAY_SIZE; i++) {
                    if(l1_cache->set[l1_set_no][i]==0){
                        // sub-set is not full
                        l1_cache->set[l1_set_no][i]=l1_n_way_index;
                        break;
                    }
                    // No sub-set of n-set is empty. It is full
                    if(i==L1_WAY_SIZE-1){
                        // Generating random number upto WAY_SIZE
                        int random = rand()%WAY_SIZE;
                        l1_cache->set[l1_set_no][random]=l1_n_way_index;
                        break;
                    }
                }

            }

              // MISSED IN L2 ALSO
            else{
                l2_cache->misses+=1;

                // WRITE IN L1
                l1_cache->tag_field[l1_n_way_index] = l1_n_way_tag;
                l1_cache->valid_field[l1_n_way_index] = 1;
                l1_cache->dirty_field[l1_n_way_index] = 0;

                for (int i = 0; i < L1_WAY_SIZE; i++) {
                    if(l1_cache->set[l1_set_no][i]==0){
                        // sub-set is not full
                        l1_cache->set[l1_set_no][i]=l1_n_way_index;
                        break;
                    }
                    // No sub-set of n-set is empty. It is full
                    if(i==L1_WAY_SIZE-1){
                        // Generating random number upto WAY_SIZE
                        int random = rand()%WAY_SIZE;
                        l1_cache->set[l1_set_no][random]=l1_n_way_index;
                        break;
                    }
                }
            }


            // WRITE IN L2
            l2_cache->tag_field[l2_n_way_index] = l1_n_way_tag;
            l2_cache->valid_field[l2_n_way_index] = 1;
            l2_cache->dirty_field[l2_n_way_index] = 0;

            for (int i = 0; i < L2_WAY_SIZE; i++) {
                if(l2_cache->set[l2_set_no][i]==0){
                    // sub-set is not full
                    l2_cache->set[l2_set_no][i]=l1_n_way_index;
                    break;
                }
                // No sub-set of n-set is empty. It is full
                // Therefore, will implement random replacement policy
                if(i==L2_WAY_SIZE-1){
                    // Generating random number upto WAY_SIZE
                    // Because, all the subsets are full, will write at random sub-set
                    int random = rand()%WAY_SIZE;
                    l2_cache->set[l2_set_no][random]=l1_n_way_index;
                    break;
                }
            }


        }
    }
}


void fully_associative_cache_access(struct direct_mapped_cache *cache, uint64_t address, int idx)
{
    // fully associative does not have index. The block address is same as Tag
    uint64_t full_assoc_tag = address >> (unsigned)log2(BLOCK_SIZE);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif
    int found=0;

    // LOOPING THROUGH ALL THE BLOCKS TO FIND THE DATA
    // IF FOUND THE DATA, SETTING FOUND FLAG TO 1
    for (int i = 0; i < NUM_BLOCKS; ++i) {

        if(cache->tag_field[i]==full_assoc_tag){

            cache->hits += 1;
            found=1;
            break;
        }
    }

    // COULD NOT FIND DATA IN ANY OF BLOCKS
    // THEREFORE, HAVE TO WRITE IN SOME BLOCK
    if(found==0){
        cache->misses += 1;
        // CONDITION TO CHECK IF INDEX IS REACHED TO END OF BLOCKS
        // IF IT IS FULL, WILL WRITE DATA RANDOMLY
        if(idx>=NUM_BLOCKS-1){
            int random = rand()%NUM_BLOCKS;
            cache->tag_field[random]=full_assoc_tag;
        }
        // THERE IS STILL PLACE IN TAG_FIELD ARRAY
        // WE CAN WRITE DATA AT "INDEX % NUM_BLOCKS" INDEX
        else{
            cache->tag_field[idx%NUM_BLOCKS]=full_assoc_tag;
        }
    }
}
