CC = g++ -mavx2
CFLAGS = -Iinclude/ -Ilib/glfw/include -Ilib/vulkan/Include -Ilib/imgui
LDFLAGS = -Llib/glfw/lib -Llib/vulkan/Lib lib/imgui/libimgui.a -lglfw3 -lgdi32 -luser32 -lvulkan-1 

SRC  = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) 
OBJ  = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: libs game

comp-shaders:
	.\compile.bat

libs: comp-shaders # sketch fix later
	cd lib/imgui && make

dirs:
	mkdir -p .\$(BIN)

run: all
	$(BIN)/S2

game: $(OBJ)
	$(CC) -o $(BIN)/S2 $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
		del /Q bin\*.exe src\main.o lib\imgui\*.o res\shaders\*.spv
