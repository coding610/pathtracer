#_____________________VARIABLES____________________
CC=clang++

# FORMAT: bin/
EXE_PATH:=bin/
EXE_NAME=$(EXE_PATH)main


SRC=src/*.cpp
SRC_DIR=src
INC_DIR=include
OBJ_PATH=build/
CPP_VERSION=17

# Add -O3 for preformance
# For debbing, refrain to using -O3
CFLAGS := -O3 -Wall -g -I$(INC_DIR) -std=c++$(CPP_VERSION) -Wno-reorder-ctor
LDFLAGS := -lraylib

#_____________________COMPILE______________________
#_____SILENT COMPILATION ENABLED BELOW_____ 
# .SILENT: 
.PHONY:
all: compile link clean_opt run

.PHONY:
debug: debugcompile debug_clean

.PHONY:
debugcompile:
	$(CC) --debug $(SRC) $(CFLAGS) $(LDFLAGS)

debug_clean:
	rm -r $(EXE_PATH)debug.dSYM
	mv a.out $(EXE_PATH)debug
	mv a.out.dSYM $(EXE_PATH)debug.dSYM

.PHONY:
compile:
	$(CC) -c $(SRC) $(CFLAGS)

.PHONY:
link:
	$(CC) ./*.o $(LDFLAGS)

#_____________________CLEAN________________________
clean_opt: obj exe

ifdef OBJ_PATH
obj:
	mv ./*.o $(OBJ_PATH)
else
obj:
	mkdir -p $(OBJ_PATH)
	mv ./*.o $(OBJ_PATH)
endif

ifdef EXE_PATH
exe:
	mkdir -p $(EXE_PATH)
	mv a.out $(EXE_NAME)
else
exe:
	mv a.out $(EXE_NAME)
endif

#_____________________RUN__________________________
.PHONY:
run:
	./$(EXE_NAME)
