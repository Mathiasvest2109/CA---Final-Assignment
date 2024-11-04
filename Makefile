CC = gcc
CCWARNINGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
CCOPTS = -g -O0
CCFLAGS = $(CCWARNINGS) $(CCOPTS)

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

EXE = RISC-V.exe

OUTPUT_DIR = output

all: | $(OUTPUT_DIR)
	make $(OBJECTS)
	$(CC) $(CCFLAGS) $(OUTPUT_DIR)/$(OBJECTS) -o $(OUTPUT_DIR)/$(EXE)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

%.o: %.c
		$(CC) $(CCFLAGS) -c $< -o $(OUTPUT_DIR)/$@

clean: | $(OUTPUT_DIR)
	rd /s /q $(OUTPUT_DIR)


