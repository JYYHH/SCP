## 0. Environment
- Ubuntu 20.04 x86_64 GNU/Linux
- gcc (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0
- GNU Make 4.2.1
- libgcrypt20

## 1. Compile
```bash
# working dir: SCP/
make
```
### 1.1 Clear (the binary)
```bash
# working dir: SCP/
make clean
```

## 2. Run
### 2.1 Sender
```bash
purenc <input file> [-d <output IP-addr:port>] [-l]
```
### 2.2 Receiver
```bash
purdec [port] [-l <input file>]
```

### 2.3 example for local run
```bash
# 127.0.0.1
cp data data.cpy
purenc data.cpy -l
rm data.cpy
purdec -l data.cpy.pur
diff data data.cpy
```

### 2.4 example for remote run
#### 2.4.1 Sender side
```bash
purenc data -d 192.168.3.5:7777
```
#### 2.4.2 Receiver side
```bash
purdec 7777
```

## 3. Auto Local Test
- This script along with files under `data` directory helps you check the correctness and functionality for the local run
- And you can directly run this script right after you download this repo to your machine and cd into it, instead of compiling first (running `make`).
- one parameter is needed, which is the file name (choose one from `data/`)
    - `need_pad` is a file with 1027 bytes, which is helpful for checking the padding
    - `an_executable` is a historical verision of a binary (`purenc`/`purdec`)
    - `random` is a file of random byte sequence, and with size around `10M`, which aims to show the robustness of the program
    - ___All the above files are also passed for the remote tests, using two remote machines___

