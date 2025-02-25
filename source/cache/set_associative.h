#ifndef SET_ASSOCIATIVE_H
#define SET_ASSOCIATIVE_H

#include <stdint.h>

// 定数定義
#define MEMORY_SIZE 131072  // メモリサイズ
#define CACHE_LINES 1024    // L1キャッシュのライン数 (index)
#define WAYS 4              // セットあたりのウェイ数
#define BLOCK_SIZE 128      // ブロックサイズ（バイト）

// キャッシュラインの構造体
typedef struct {
    int valid;
    uint32_t tag;
    int last_used;  // LRU用
} CacheLine;

// キャッシュセットの構造体
typedef struct {
    CacheLine lines[WAYS];
} CacheSet;

// グローバル変数の宣言
extern CacheSet cache[CACHE_LINES];
extern uint8_t memory[MEMORY_SIZE];

// 関数プロトタイプ
void init_set_associative_cache();
void init_memory();
void cache_read(uint32_t address, uint8_t* data, int size);
void cache_write(uint32_t address, uint8_t* data, int size);
void print_stats();
void reset_stats();
void sequential_access_test(int num_accesses);
void random_access_test(int num_accesses);

#endif // SET_ASSOCIATIVE_H