// set_associative.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_SIZE 131072
#define CACHE_LINES 1024
#define WAYS 4
#define BLOCK_SIZE 128

typedef struct {
    int valid;
    uint32_t tag;
    int last_used;
    uint8_t data[BLOCK_SIZE];  // データを保持するための配列を追加
} CacheLine;

typedef struct {
    CacheLine lines[WAYS];
} CacheSet;

CacheSet cache[CACHE_LINES];
uint8_t memory[MEMORY_SIZE];

int hit_count = 0, miss_count = 0;
int access_counter = 0;

void init_set_associative_cache() {  // 関数名にプレフィックスを追加
    for (int i = 0; i < CACHE_LINES; i++) {
        for (int j = 0; j < WAYS; j++) {
            cache[i].lines[j].valid = 0;
            cache[i].lines[j].tag = 0;
            cache[i].lines[j].last_used = 0;
        }
    }
}

void init_memory() {  // 関数名にプレフィックスを追加
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = rand() % 256;
    }
}

void cache_read(uint32_t address, uint8_t* data, int size) {
    printf("Cache Read: Reading %d bytes from address 0x%08x\n", size, address);

    uint32_t block_address = address / BLOCK_SIZE;
    uint32_t index = block_address % CACHE_LINES;
    uint32_t tag = block_address / CACHE_LINES;

    CacheSet* set = &cache[index];

    // キャッシュ内のデータを探す
    int way;
    for (way = 0; way < WAYS; way++) {
        if (set->lines[way].valid && set->lines[way].tag == tag) {
            // キャッシュヒット
            hit_count++;
            break;
        }
    }

    if (way == WAYS) {
        // キャッシュミス
        miss_count++;
        // 新しいラインを割り当て（ここではLRUを使用）
        way = find_lru_way(set);
        set->lines[way].valid = 1;
        set->lines[way].tag = tag;
        
        // メモリからデータをロード
        uint32_t block_start = block_address * BLOCK_SIZE;
        for (int i = 0; i < BLOCK_SIZE; i++) {
            set->lines[way].data[i] = memory[block_start + i];
        }
    }

    // データをキャッシュから読み込む
    uint32_t block_offset = address % BLOCK_SIZE;
    for (int i = 0; i < size; i++) {
        data[i] = set->lines[way].data[block_offset + i];
    }

    // LRU情報を更新
    set->lines[way].last_used = ++access_counter;

    printf("Cache Read: Value 0x%08x read from cache\n", *(uint32_t*)data);
}

int find_lru_way(CacheSet* set) {
    int lru_way = 0;
    for (int i = 1; i < WAYS; i++) {
        if (set->lines[i].last_used < set->lines[lru_way].last_used) {
            lru_way = i;
        }
    }
    return lru_way;
}

void cache_write(uint32_t address, uint8_t* data, int size) {
    printf("Cache Write: Writing %d bytes to address 0x%08x\n", size, address);

    uint32_t block_address = address / BLOCK_SIZE;
    uint32_t index = block_address % CACHE_LINES;
    uint32_t tag = block_address / CACHE_LINES;

    CacheSet* set = &cache[index];

    // キャッシュ内のデータを探す
    int way;
    for (way = 0; way < WAYS; way++) {
        if (set->lines[way].valid && set->lines[way].tag == tag) {
            // キャッシュヒット
            hit_count++;
            break;
        }
    }

    if (way == WAYS) {
        // キャッシュミス
        miss_count++;
        // 新しいラインを割り当て（ここではLRUを使用）
        way = find_lru_way(set);
        set->lines[way].valid = 1;
        set->lines[way].tag = tag;
    }

    // キャッシュとメモリの両方を更新
    uint32_t block_offset = address % BLOCK_SIZE;
    for (int i = 0; i < size; i++) {
        set->lines[way].data[block_offset + i] = data[i];
        memory[address + i] = data[i];
    }

    // LRU情報を更新
    set->lines[way].last_used = ++access_counter;

    printf("Cache Write: Value 0x%08x written to cache and memory\n", *(uint32_t*)data);
}

void print_stats() {  // 関数名にプレフィックスを追加
    printf("Hits: %d, Misses: %d\n", hit_count, miss_count);
    printf("Hit Rate: %.2f%%\n", (double)hit_count / (hit_count + miss_count) * 100);
}

void reset_stats() {  // 関数名にプレフィックスを追加
    hit_count = 0;
    miss_count = 0;
}

void sequential_access_test(int num_accesses) {
    reset_stats();
    printf("\nSequential Access Test (%d accesses):\n", num_accesses);
    uint8_t data[BLOCK_SIZE];
    for (int i = 0; i < num_accesses && i * BLOCK_SIZE < MEMORY_SIZE; i++) {  // 境界チェックを追加
        cache_read(i * BLOCK_SIZE, data, BLOCK_SIZE);
    }
    print_stats();
}

void random_access_test(int num_accesses) {  // 関数名にプレフィックスを追加
    reset_stats();
    printf("\nRandom Access Test (%d accesses):\n", num_accesses);
    uint8_t data[BLOCK_SIZE];
    for (int i = 0; i < num_accesses; i++) {
        uint32_t address = (rand() % (MEMORY_SIZE / BLOCK_SIZE)) * BLOCK_SIZE;
        cache_read(address, data, BLOCK_SIZE);
    }
    print_stats();
}

// int main() {
//     srand(time(NULL));  // 乱数生成器の初期化

//     // キャッシュとメモリの初期化
//     init_set_associative_cache();
//     init_memory();

//     printf("Set Associative Cache Simulation\n");
//     printf("Cache Lines: %d, Ways: %d, Block Size: %d bytes\n", CACHE_LINES, WAYS, BLOCK_SIZE);
//     printf("Total Memory Size: %d bytes\n\n", MEMORY_SIZE);

//     // シーケンシャルアクセステスト
//     sequential_access_test(1000);

//     // ランダムアクセステスト
//     random_access_test(1000);

//     // カスタムテスト
//     printf("\nCustom Test:\n");
//     reset_stats();

//     uint8_t data[4];
//     for (int i = 0; i < 1000; i++) {
//         uint32_t address = rand() % MEMORY_SIZE;
//         if (rand() % 2 == 0) {
//             // 読み込み操作
//             cache_read(address, data, 4);
//         } else {
//             // 書き込み操作
//             for (int j = 0; j < 4; j++) {
//                 data[j] = rand() % 256;
//             }
//             cache_write(address, data, 4);
//         }
//     }

//     print_stats();

//     return 0;
// }