export VK_LAYER_PATH = $(CURDIR)/lib/vulkan/Bin

CC = gcc -g
CFLAGS = -Ilib/vulkan/Include -Ilib/CUDA/v13.2/include
LDFLAGS = -Llib/vulkan/Lib -Llib/CUDA/v13.2/lib/x64 -lgdi32 -luser32 -ldwmapi -lpsapi -lcuda

SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) 
OBJ = $(SRC:.c=.o) 
BIN = bin
VERT_SPV = res/shaders/vert.spv
FRAG_SPV = res/shaders/frag.spv

.PHONY: all clean

all: libs game

$(VERT_SPV) $(FRAG_SPV):
	.\compile.bat

$(IMGUI_LIB):
	cd lib/imgui && make

libs: $(VERT_SPV) $(FRAG_SPV) $(IMGUI_LIB)

dirs:
	mkdir -p .\$(BIN)

run: all
	$(BIN)/S2

game: $(OBJ)
	$(CC) -o $(BIN)/S2 $^ res/res.o $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	del /Q bin\*.exe src\*.o 
