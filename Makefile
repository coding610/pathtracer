#_____________________VARIABLES____________________
CC=clang++

# Paths
EXE_PATH=bin/
EXE_NAME=$(EXE_PATH)main

SRC=$(wildcard src/*.cpp src/engines/scene/*.cpp src/engines/camera/*.cpp src/engines/window/*.cpp src/engines/render/*.cpp src/engines/ui/*.cpp)
OBJ=$(patsubst src/%.cpp, build/app/%.o, $(SRC))  # Convert .cpp -> .o

SRC_DIR=src
INC_DIR=include
OBJ_PATH=build/app/
CPP_VERSION=20

# Flags
CFLAGS := -Wall -I$(INC_DIR) -Iextern/imgui -std=c++$(CPP_VERSION) -Wno-reorder-ctor
LDFLAGS := -lGL -lglfw -lGLEW -lfmt $(wildcard build/imgui/*.o)

# Ensure base directories exist
$(shell mkdir -p $(OBJ_PATH) $(EXE_PATH))

#_____________________COMPILE______________________
.PHONY: all debug clean run

all: $(EXE_NAME)

$(EXE_NAME): $(OBJ)
	@echo "[LINK] Building executable: $(EXE_NAME)"
	@$(CC) $(OBJ) $(LDFLAGS) -o $@

# Create missing directories before compiling
build/app/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "[COMPILE] $< → $@"
	@$(CC) -c $< $(CFLAGS) -o $@

# Compile ImGui separately
build/imgui/%.o: extern/imgui/%.cpp
	@mkdir -p $(dir $@)
	@echo "[COMPILE] ImGui: $< → $@"
	@$(CC) -c $< $(CFLAGS) -o $@

debug: CFLAGS += --debug
debug: all

#_____________________CLEAN________________________
clean:
	@echo "[CLEAN] Removing build files..."
	@rm -rf build/app/ build/imgui/ $(EXE_NAME)

#_____________________RUN__________________________
run: all
	@echo "[RUN] Executing $(EXE_NAME)..."
	@./$(EXE_NAME)
