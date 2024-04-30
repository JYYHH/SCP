- My name: Haoyu Jin
- My email: jin511@purdue.edu

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
### 3.1 Usage example
```bash
bash ./autotest_local.sh random
```

## 4. Sorce files
- see directory `src/`, along with `Makefile`

## 5. Overview of work accomplished
- I build the TCP socket for the network programming
- I build the key from password
- I encrypt the data, and then add a hash suffix before sending it to the network
    - on the receiver side, symmetrical actions are taken.
- I support the multithreading version of receiver, and more public-key based certification process on another branch `real`. (not required in this assignment)

## 6. Description of code and code layout
- Here I only explain the main content about each source file, please check the comment in source files to get the detail of each function
1. `dec.c`: main routine of receiver
2. `enc.c`: main routine of sender
3. `network.c`: my wrapper for building and using the socket
4. `crypto.c`: functions used for crypto purpose, building on `<gcrypt.h>`
5. `util.c`: extension for `dec.c` and `enc.c`

## 7. General comments and design decisions made
- see the source codes for comments 
- Design Decision:
    - for both local and remote mode, the `enc` should encrypt and append a hash before sending msg chunk, the `dec` should checking the hash and decrypt after receiving msg chunk.

## 8. Answer to the question
- I need a salt string
- It's used to protect the key, since directly mapping password to key may suffer a type of attack which uses brute force to sampling the mapping function.
    - The salt is like a user-specific characteristic.
- In this project, the salt just stays constant through both sides, for the simplicity.

## 9. Number of hours spent on the project and level of effort required
- 3 days, be stopped by a misfortune (delete the source code unconciously) for 1 day and more..
- workload is similar to lab2 (MiniVPN)

## -1. What's more?
- see branch `real` to see my progress on developing this project into more real-world-like senario.