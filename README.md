cpuex 3班 simulator

## 
parser
```
cd pipeline
make
./pipeline
```
    入力: assembly.txt
    出力: binary.txt : binaryのcode
          transition.md : registerの遷移を表示
          pipeline.txt : pipelineの表示

    入力例:
    addi x5, x0, 5
    addi x6, x0, 10
    add x7, x6, x5
    
binary to result
```
cd binary_to_result
./parser
```
    入力: asm_to_binary/binary.txt
    (asm_to_binaryでbinaryを出力した場合はdirectoryを移動してそのまま実行可)
    出力: result.txt(最終結果)
    transition.md(逐次のregister値)
    入力例:
    00000000010000000000000000010011
    00000000000100000000000010010011


float計算
```
cd float
make
./test
```
    入力: flu.txt   
    出力: output.txt

## cache
```
cd cache
make
./direct
```