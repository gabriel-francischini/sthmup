CC = g++
CFLAGS = -Wall -Wextra -g -O0 -fpermissive -Wno-write-strings
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c*)
OBJS=$(patsubst $(SRC)/%.c*, $(OBJ)/%.o, $(SRCS))
LINKER_FLAGS = -lSDL2 -lSDL2_image
BIN = bin/shtmup

all:$(BIN)

release: CFLAGS=-Wall -O3 -DNDEBUG -fpermissive -Wno-write-strings
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) $(LINKER_FLAGS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BIN)

clean:
	rm -rf bin obj

tags: $(BIN)
	find $(SRC)/ -type f -iname "*.[chS]" | xargs etags -a
