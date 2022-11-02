/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#define BLOCK_SIZE  64          /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define WAY_SIZE    2         /* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  65536     /* Cache capacity in bytes (must be power of 2)*/

#define NUM_BLOCKS  (CACHE_SIZE / BLOCK_SIZE)
#define NUM_SETS    (NUM_BLOCKS / WAY_SIZE)


#define L1_BLOCK_SIZE  64        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define L1_WAY_SIZE    2         /* Associativity; 1-way = direct-mapped */
#define L1_CACHE_SIZE  65536     /* Cache capacity in bytes (must be power of 2)*/

#define L1_NUM_BLOCKS  (L1_CACHE_SIZE / L1_BLOCK_SIZE)
#define L1_NUM_SETS    (L1_NUM_BLOCKS / L1_WAY_SIZE)

#define L2_BLOCK_SIZE  64        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define L2_WAY_SIZE    8         /* Associativity; 1-way = direct-mapped */
#define L2_CACHE_SIZE  1048576   /* Cache capacity in bytes (must be power of 2)*/

#define L2_NUM_BLOCKS  (L2_CACHE_SIZE / L2_BLOCK_SIZE)
#define L2_NUM_SETS    (L2_NUM_BLOCKS / L2_WAY_SIZE)


//#define DBG

/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    unsigned set[NUM_SETS][WAY_SIZE];
};

/*The data structure of L1-mapped cache*/
struct l1_mapped_cache {
    unsigned valid_field[L1_NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[L1_NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[L1_NUM_BLOCKS];     /* Tag field */
    char data_field[L1_NUM_BLOCKS][L1_BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    unsigned set[L1_NUM_SETS][L1_WAY_SIZE];
};

/*The data structure of L2-mapped cache*/
struct l2_mapped_cache {
    unsigned valid_field[L2_NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[L2_NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[L2_NUM_BLOCKS];     /* Tag field */
    char data_field[L2_NUM_BLOCKS][L2_BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    unsigned set[L2_NUM_SETS][L2_WAY_SIZE];
};


/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/*Simulate the direct-mapped cache*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address, char* level,struct l1_mapped_cache *l1_cache,struct l2_mapped_cache *l2_cache);

/*Simulate the n-way-mapped cache*/
// Function for n-way mapping
// If way_size is 1, It will give same result as direct mapping
void nway_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address,char* type,struct l1_mapped_cache *l1_cache,struct l2_mapped_cache *l2_cache);

/*Simulate the fully-associative cache*/
void fully_associative_cache_access(struct direct_mapped_cache *cache, uint64_t address, int idx);
