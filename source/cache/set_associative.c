#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_SIZE 131072  // メモリサイズ
#define CACHE_LINES 128     // L1キャッシュのライン数(index)
#define WAYS 8    // セットあたりのウェイ数
#define BLOCK_SIZE 128  // ブロックサイズ（バイト）

typedef struct {
    int valid;
    uint32_t tag;
    int last_used;  // LRU用
} CacheLine;

typedef struct {
    CacheLine lines[WAYS];
} CacheSet;

CacheSet cache[CACHE_LINES];
uint8_t memory[MEMORY_SIZE];

int hit_count = 0, miss_count = 0;
int access_counter = 0;  // 高精度LRU用

// 初期化関数
void init_cache() {
    for (int i = 0; i < CACHE_LINES; i++) {
        for (int j = 0; j < WAYS; j++) {
            cache[i].lines[j].valid = 0;
            cache[i].lines[j].tag = 0;
            cache[i].lines[j].last_used = 0;
        }
    }
}

// main memoryをランダムデータで初期化(0 ~ 255)
void init_memory() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = rand() % 256;
    }
}

// キャッシュアクセスシミュレーション
// 指定されたアドレスからデータを読み取る
void cache_read(uint32_t address, uint8_t* data, int size) {
    uint32_t block_address = address / BLOCK_SIZE;
    uint32_t index = block_address % ( CACHE_LINES / WAYS ); // 何行目か
    uint32_t tag = block_address / ( CACHE_LINES / WAYS ); // どのwayか
    // printf("address:%x, block_address:%d, index:%d, tag:%d\n",address,block_address,index,tag);

    CacheSet* set = &cache[index];

    // キャッシュヒット確認
    for (int i = 0; i < WAYS; i++) {
        if (set->lines[i].valid && set->lines[i].tag == tag) {
            hit_count++;
            set->lines[i].last_used = ++access_counter;  // LRU更新
            for (int j = 0; j < size; j++) {
                data[j] = memory[address + j];
            }
            return;
        }
    }

    // キャッシュミス処理
    miss_count++;

    // LRUポリシーによる置換
    int lru_index = 0;
    for (int i = 1; i < WAYS; i++) {
        if (!set->lines[i].valid || set->lines[i].last_used < set->lines[lru_index].last_used) {
            lru_index = i;
        }
    }

    // キャッシュラインを更新
    set->lines[lru_index].valid = 1;
    set->lines[lru_index].tag = tag;
    set->lines[lru_index].last_used = ++access_counter;

    // メモリからデータを取得
    for (int j = 0; j < size; j++) {
        data[j] = memory[address + j];
    }
}

// 統計表示
void print_stats(const char* test_name) {
    printf("\n%s Results:\n", test_name);
    printf("Hits: %d, Misses: %d\n", hit_count, miss_count);
    printf("Hit Rate: %.2f%%\n", (double)hit_count / (hit_count + miss_count) * 100);
}

// 統計リセット
void reset_stats() {
    hit_count = 0;
    miss_count = 0;
}

// テスト関数
void sequential_access_test(int num_accesses) {
    reset_stats();
    printf("\nSequential Access Test (%d accesses):\n", num_accesses);
    uint8_t data[BLOCK_SIZE];
    for (int i = 0; i < num_accesses; i++) {
        cache_read(i * BLOCK_SIZE, data, BLOCK_SIZE);
    }
    print_stats("Sequential Access Test");
}

void random_access_test(int num_accesses) {
    reset_stats();
    printf("\nRandom Access Test (%d accesses):\n", num_accesses);
    uint8_t data[BLOCK_SIZE];
    for (int i = 0; i < num_accesses; i++) {
        uint32_t address = (rand() % (MEMORY_SIZE / BLOCK_SIZE)) * BLOCK_SIZE;
        cache_read(address, data, BLOCK_SIZE);
    }
    print_stats("Random Access Test");
}

// メイン関数
int main() {
    srand(time(NULL));

    // 初期化
    init_cache();
    init_memory();

    // テスト実行
    sequential_access_test(1000);
    random_access_test(1000);

    return 0;
}
