CC = gcc
CCWARNINGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
CCOPTS = -g -O0
CCFLAGS = $(CCWARNINGS) $(CCOPTS)

SOURCES = main.c decoder.c registers.c program.c memory.c
OBJECTS = $(SOURCES:.c=.o)

EXE = RISC-V
ifeq ($(OS),Windows_NT)
EXE := $(EXE).exe
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
	./$(OUTPUT_DIR)/$(EXE)
else
	$(OUTPUT_DIR)/$(EXE)
endif
