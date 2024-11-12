#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INSTRUCTION 1000

#define CACHE_LINE 1024
#define LINE_SIZE 128
#define MEMORY_SIZE 4096


struct cache_line{
    char valid;
    uint32_t tag;
    char data[LINE_SIZE];
};

struct cache_line cache[CACHE_LINE];
char main_memory[MEMORY_SIZE];

void init_cache(){
    for(int i=0; i<LINE_SIZE; i++){
        cache[i].tag = 0;
        cache[i].valid = 0;
        memset(cache[i].data, 0, LINE_SIZE);
    }
}

void init_memory(){
    for (int i = 0; i < MEMORY_SIZE; i++) {
        main_memory[i] = rand() % 256;  // 0-255のランダムな値で初期化
    }
}

void read_from_memory(uint32_t address, char* data, int size) {
    memcpy(data, &main_memory[address], size);
}

void write_to_memory(uint32_t address, const char* data, int size) {
    memcpy(&main_memory[address], data, size);
}

void cache_read(uint32_t address, char*data, int size){
    int cache_line = log2(CACHE_LINE);
    int line_size = log2(LINE_SIZE);
    uint32_t tag = (address >> (line_size + cache_line)) & 0x7FFF;
    uint32_t index = (address >> line_size) & 0x3FF;
    uint32_t offset = address & 0x7F;

    if(cache[index].valid && cache[index].tag == tag){
        printf("Hit\n");
        memcpy(data, &cache[index].data[offset], size);
    } else {
        printf("Miss\n");
        read_from_memory(address - offset, cache[index].data, LINE_SIZE);
        cache[index].tag = tag;
        cache[index].valid = 1;
        
        memcpy(data, &cache[index].data[offset], size);
    }
}

int cache_write(uint32_t address, const char* data, int size){
    int cache_line = log2(CACHE_LINE);
    int line_size = log2(LINE_SIZE);
    uint32_t tag = (address >> (line_size + cache_line)) & 0x7FFF;
    uint32_t index = (address >> line_size) & 0x3FF;
    uint32_t offset = address & 0x7F;
    
    if(cache[index].valid && cache[index].tag == tag){
        printf("Hit\n");
        memcpy(&cache[index].data[offset], data, size);
        write_to_memory(address, data, size);
        return 1;
    } else {
        printf("Miss\n");
        read_from_memory(address - offset, cache[index].data, LINE_SIZE);
        cache[index].tag = tag;
        cache[index].valid = 1;
        memcpy(&cache[index].data[offset], data, size);
        write_to_memory(address, data, size);
        return 0; // Cache miss
    }

}


int main(){
    init_cache();    

    char data[8];
    
    cache_read(0, data, 8);
    cache_read(128, data, 8);
    cache_read(1024, data, 8);
    cache_read(0, data, 8);
    
    char write_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    cache_write(256, write_data, 8);
    cache_read(256, data, 8);
    
    printf("Data read from address 256: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    return 0;
}