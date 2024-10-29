cpuex 3班 simulator

parser
```
cd asm_to_binary
./parser
```
    assembly to binary
    入力: assembly.txt
    出力: output.txt
        出力の1行目にはassembly.txtの行数を2進数表示
    入力例:
    addi x5, x0, 5
    addi x6, x0, 10
    add x7, x6, x5
    spaceは必要
    
    binary to result
```
cd binary_to_result
./parser
```
    入力: instruction.txt   
    出力: result.txt
    入力例:
    00000000010000000000000000010011
    00000000000100000000000010010011