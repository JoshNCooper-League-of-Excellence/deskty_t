COMPILER := clang
COMPILER_FLAGS := -std=c23 -g
LD_FLAGS := -lGL -lGLEW -lglfw -lm
OBJ_DIR := objs
BIN_DIR := bin

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(wildcard *.c))

all: directories desky_t

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

desky_t: $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) -o $(BIN_DIR)/$@ $^ $(LD_FLAGS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all desky_t
	./$(BIN_DIR)/desky_t
