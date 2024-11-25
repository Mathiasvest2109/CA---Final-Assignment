CC = gcc
CCWARNINGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
CCOPTS = -g -O0
CCFLAGS = $(CCWARNINGS) $(CCOPTS)

SOURCES = main.c decoder.c registers.c program.c memory.c executor.c
OBJECTS = $(SOURCES:.c=.o)

TEST_SOURCES = testing.c decoder.c registers.c program.c memory.c executor.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

EXE = RISC-V
TEST_EXE = test

ifeq ($(OS),Windows_NT)
EXE := $(EXE).exe
TEST_EXE := $(TEST_EXE).exe
endif

OUTPUT_DIR = output

.PHONY: all clean run

all: | $(OUTPUT_DIR) $(OBJECTS)
	$(CC) $(CCFLAGS) $(OBJECTS:%=$(OUTPUT_DIR)/%) -o $(OUTPUT_DIR)/$(EXE)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $(OUTPUT_DIR)/$@

clean:
ifneq ($(OS),Windows_NT)
	rm -rf $(OUTPUT_DIR)
else
	if exist $(OUTPUT_DIR) rd /s /q $(OUTPUT_DIR)
endif

run: | all
ifneq ($(OS),Windows_NT)
	./$(OUTPUT_DIR)/$(EXE) $(PROGRAM)
else
	$(OUTPUT_DIR)/$(EXE)
endif

test: | $(OUTPUT_DIR) $(TEST_OBJECTS)
	$(CC) $(CCFLAGS) $(TEST_OBJECTS:%=$(OUTPUT_DIR)/%) -o $(OUTPUT_DIR)/$(TEST_EXE)
ifneq ($(OS),Windows_NT)
	./$(OUTPUT_DIR)/$(TEST_EXE)
else
	$(OUTPUT_DIR)/$(TEST_EXE)
endif
