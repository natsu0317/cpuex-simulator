#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_LINES_L1 1024
#define CACHE_LINES_L2 2048
#define CACHE_LINES_L3 4096
#define LINE_SIZE 16 // 128ビット = 16バイト
#define MEMORY_SIZE 65536

struct cache_line
{
    char valid;
    uint32_t tag;
    uint8_t data[LINE_SIZE];
};

// L1, L2, L3キャッシュ
struct cache_line cache_L1[CACHE_LINES_L1];
struct cache_line cache_L2[CACHE_LINES_L2];
struct cache_line cache_L3[CACHE_LINES_L3];
uint8_t main_memory[MEMORY_SIZE];

// 統計情報
int hit_count_L1 = 0, miss_count_L1 = 0;
int hit_count_L2 = 0, miss_count_L2 = 0;
int hit_count_L3 = 0, miss_count_L3 = 0;

void init_cache(struct cache_line* cache, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        cache[i].tag = 0;
        cache[i].valid = 0;
        memset(cache[i].data, 0, LINE_SIZE);
    }
}

void init_memory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        main_memory[i] = rand() % 256;
    }
}

int cache_access(struct cache_line* cache, int lines, uint32_t address, uint8_t* data, int size, int* hit_count, int* miss_count, int write, const uint8_t* write_data)
{
    uint32_t tag = address / (lines * LINE_SIZE);
    uint32_t index = (address / LINE_SIZE) % lines;
    uint32_t offset = address % LINE_SIZE;

    if (cache[index].valid && cache[index].tag == tag)
    {
        // キャッシュヒット
        if (write)
        {
            memcpy(&cache[index].data[offset], write_data, size);
            memcpy(&main_memory[address], write_data, size); // Write-through
        }
        else
        {
            memcpy(data, &cache[index].data[offset], size);
        }
        (*hit_count)++;
        return 1; // ヒット
    }
    else
    {
        // キャッシュミス
        uint32_t block_start = address - offset;
        memcpy(cache[index].data, &main_memory[block_start], LINE_SIZE);
        cache[index].tag = tag;
        cache[index].valid = 1;
        if (write)
        {
            memcpy(&cache[index].data[offset], write_data, size);
            memcpy(&main_memory[address], write_data, size); // Write-through
        }
        else
        {
            memcpy(data, &cache[index].data[offset], size);
        }
        (*miss_count)++;
        return 0; // ミス
    }
}

void hierarchical_cache_access(uint32_t address, uint8_t* data, int size, int write, const uint8_t* write_data)
{
    if (cache_access(cache_L1, CACHE_LINES_L1, address, data, size, &hit_count_L1, &miss_count_L1, write, write_data))
    {
        printf("L1 Cache Hit at address 0x%x\n", address);
    }
    else if (cache_access(cache_L2, CACHE_LINES_L2, address, data, size, &hit_count_L2, &miss_count_L2, write, write_data))
    {
        printf("L2 Cache Hit at address 0x%x\n", address);
    }
    else if (cache_access(cache_L3, CACHE_LINES_L3, address, data, size, &hit_count_L3, &miss_count_L3, write, write_data))
    {
        printf("L3 Cache Hit at address 0x%x\n", address);
    }
    else
    {
        printf("Main Memory Access at address 0x%x\n", address);
    }
}

void cache_read(uint32_t address, uint8_t* data, int size)
{
    hierarchical_cache_access(address, data, size, 0, NULL);
}

void cache_write(uint32_t address, const uint8_t* data, int size)
{
    hierarchical_cache_access(address, NULL, size, 1, data);
}

void print_stats()
{
    printf("\nCache Statistics:\n");
    printf("L1 - Hits: %d, Misses: %d\n", hit_count_L1, miss_count_L1);
    printf("L2 - Hits: %d, Misses: %d\n", hit_count_L2, miss_count_L2);
    printf("L3 - Hits: %d, Misses: %d\n", hit_count_L3, miss_count_L3);
    int total_accesses = hit_count_L1 + miss_count_L1;
    printf("Total Accesses: %d\n", total_accesses);
}

void sequential_access_test(int num_accesses)
{
    printf("\nSequential Access Test (%d accesses):\n", num_accesses);
    uint8_t data[8];
    for (int i = 0; i < num_accesses; i++)
    {
        cache_read(i * 8, data, 8);
    }
}

void random_access_test(int num_accesses)
{
    printf("\nRandom Access Test (%d accesses):\n", num_accesses);
    uint8_t data[8];
    for (int i = 0; i < num_accesses; i++)
    {
        uint32_t address = rand() % (MEMORY_SIZE - 8);
        cache_read(address, data, 8);
    }
}

void locality_test(int num_accesses, int locality_range)
{
    printf("\nLocality Test (%d accesses, range %d):\n", num_accesses, locality_range);
    uint8_t data[8];
    uint32_t base_address = rand() % (MEMORY_SIZE - locality_range);
    for (int i = 0; i < num_accesses; i++)
    {
        uint32_t offset = rand() % locality_range;
        cache_read(base_address + offset, data, 8);
    }
}

int main()
{
    srand(time(NULL));

    // 初期化
    init_cache(cache_L1, CACHE_LINES_L1);
    init_cache(cache_L2, CACHE_LINES_L2);
    init_cache(cache_L3, CACHE_LINES_L3);
    init_memory();

    uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    // テスト
    sequential_access_test(100);
    random_access_test(100);
    locality_test(100, 64);

    // 書き込みテスト
    cache_write(1024, data, 8);
    cache_read(1024, data, 8);

    print_stats();
    return 0;
}
