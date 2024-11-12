#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_LINES 1024
#define LINE_SIZE 16  // 128ビット = 16バイト
#define MEMORY_SIZE 65536  // より大きなメモリサイズを使用

struct cache_line {
    char valid;
    uint32_t tag;
    uint8_t data[LINE_SIZE];
};

struct cache_line cache[CACHE_LINES];
uint8_t main_memory[MEMORY_SIZE];

int hit_count = 0;
int miss_count = 0;

void init_cache() {
    for (int i = 0; i < CACHE_LINES; i++) {
        cache[i].tag = 0;
        cache[i].valid = 0;
        memset(cache[i].data, 0, LINE_SIZE);
    }
}

void init_memory() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        main_memory[i] = rand() % 256;
    }
}

int cache_read(uint32_t address, uint8_t* data, int size) {
    uint32_t tag = address / (CACHE_LINES * LINE_SIZE);
    uint32_t index = (address / LINE_SIZE) % CACHE_LINES;
    uint32_t offset = address % LINE_SIZE;

    if (cache[index].valid && cache[index].tag == tag) {
        printf("Cache Hit at address 0x%x\n", address);
        memcpy(data, &cache[index].data[offset], size);
        hit_count++;
        return 1;  // Cache hit
    } else {
        printf("Cache Miss at address 0x%x\n", address);
        uint32_t block_start = address - offset;
        memcpy(cache[index].data, &main_memory[block_start], LINE_SIZE);
        cache[index].tag = tag;
        cache[index].valid = 1;
        memcpy(data, &cache[index].data[offset], size);
        miss_count++;
        return 0;  // Cache miss
    }
}

int cache_write(uint32_t address, const uint8_t* data, int size) {
    uint32_t tag = address / (CACHE_LINES * LINE_SIZE);
    uint32_t index = (address / LINE_SIZE) % CACHE_LINES;
    uint32_t offset = address % LINE_SIZE;

    if (cache[index].valid && cache[index].tag == tag) {
        printf("Cache Hit (Write) at address 0x%x\n", address);
        hit_count++;
    } else {
        printf("Cache Miss (Write) at address 0x%x\n", address);
        uint32_t block_start = address - offset;
        memcpy(cache[index].data, &main_memory[block_start], LINE_SIZE);
        cache[index].tag = tag;
        cache[index].valid = 1;
        miss_count++;
    }

    // Write-through policy
    memcpy(&cache[index].data[offset], data, size);
    memcpy(&main_memory[address], data, size);
    return cache[index].valid && cache[index].tag == tag;
}

void print_cache_line(uint32_t index) {
    printf("Cache Line %u: Valid=%d, Tag=0x%x, Data=", index, cache[index].valid, cache[index].tag);
    for (int i = 0; i < LINE_SIZE; i++) {
        printf("%02x ", cache[index].data[i]);
    }
    printf("\n");
}

void print_stats() {
    int total_accesses = hit_count + miss_count;
    printf("\nCache Statistics:\n");
    printf("Total accesses: %d\n", total_accesses);
    printf("Hits: %d (%.2f%%)\n", hit_count, (float)hit_count / total_accesses * 100);
    printf("Misses: %d (%.2f%%)\n", miss_count, (float)miss_count / total_accesses * 100);
}

void sequential_access_test(int num_accesses) {
    printf("\nSequential Access Test (%d accesses):\n", num_accesses);
    uint8_t data[8];
    for (int i = 0; i < num_accesses; i++) {
        cache_read(i * 8, data, 8);
    }
}

void random_access_test(int num_accesses) {
    printf("\nRandom Access Test (%d accesses):\n", num_accesses);
    uint8_t data[8];
    for (int i = 0; i < num_accesses; i++) {
        uint32_t address = rand() % (MEMORY_SIZE - 8);
        cache_read(address, data, 8);
    }
}

void locality_test(int num_accesses, int locality_range) {
    printf("\nLocality Test (%d accesses, range %d):\n", num_accesses, locality_range);
    uint8_t data[8];
    uint32_t base_address = rand() % (MEMORY_SIZE - locality_range);
    for (int i = 0; i < num_accesses; i++) {
        uint32_t offset = rand() % locality_range;
        cache_read(base_address + offset, data, 8);
    }
}

int main() {
    srand(time(NULL));
    init_cache();
    init_memory();

    // Basic tests
    uint8_t data[8];
    cache_read(0, data, 8);
    cache_read(16, data, 8);
    cache_read(1024, data, 8);
    cache_read(0, data, 8);

    uint8_t write_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    cache_write(256, write_data, 8);
    cache_read(256, data, 8);

    printf("Data read from address 256: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    print_cache_line(0);
    print_cache_line(1);
    print_cache_line(16);

    // Complex tests
    sequential_access_test(1000);
    random_access_test(1000);
    locality_test(1000, 1024);

    print_stats();

    return 0;
}