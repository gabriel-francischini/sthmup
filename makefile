CC = g++
CFLAGS = -Wall -Wextra -g -O0 -fpermissive -Wno-write-strings -Isrc
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/**/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
OBJS_SUBFOLDERS=$(patsubst $(SRC)/%, $(OBJ)/%, $(dir $(OBJS)))
LINKER_FLAGS = -lSDL2 -lSDL2_image
BIN = bin/shtmup

all:$(BIN)

release: CFLAGS=-Wall -O3 -DNDEBUG -fpermissive -Wno-write-strings -Isrc
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) $(LINKER_FLAGS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p obj $(OBJS_SUBFOLDERS)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BIN)

clean:
	rm -rf bin obj

tags: $(BIN)
	find $(SRC)/ -type f -iname "*.[chSp]*" | xargs etags -a
