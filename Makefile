# 1. Env
SRC_DIR := src
BIN_DIR := bin
DATA_DIR := data
LOG_DIR := log
CC := gcc
FLAGS := -w -pthread -lgcrypt -o
HEADER := $(SRC_DIR)/common.h

# 2. Target
Tar_enc := $(BIN_DIR)/purenc
Tar_dec := $(BIN_DIR)/purdec

# 3. Source 
Src_enc := $(SRC_DIR)/enc.c
Src_dec := $(SRC_DIR)/dec.c
Src_common := $(SRC_DIR)/crypto.c $(SRC_DIR)/network.c $(SRC_DIR)/util.c

# 4. Build all the executable
normal: $(BIN_DIR) $(DATA_DIR) $(LOG_DIR) $(Tar_enc) $(Tar_dec)
$(Tar_enc): $(HEADER) $(Src_enc) $(Src_common)
	$(CC) $^ $(FLAGS) $@
$(Tar_dec): $(HEADER) $(Src_dec) $(Src_common)
	$(CC) $^ $(FLAGS) $@
$(BIN_DIR):
	mkdir -p bin
$(DATA_DIR):
	mkdir -p data
$(LOG_DIR):
	mkdir -p log
# 5. Clean up
clean: 
	rm -f bin/*