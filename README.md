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

通信のsimulator
cpuex-v1.6/server/reader.pyの316行目
```
file_name = '../raytracer/sld/temp.sld'
```
のfile_nameを変更して
```
python3 reader.py
```
./formatted_sld_dataに成形したデータが出力される

## cache
```
cd cache
make
./direct
```