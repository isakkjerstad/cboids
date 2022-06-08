# Output name of binary.
BIN_NAME = boids

# Compiler options and flags.
COMPILER = gcc
CFLAGS = -O3 -Wall -Wextra
LIBS = `sdl2-config --libs --cflags` -ISDL2_image -lm

# Input fIle directory names.
SRC_DIR = sources
HDR_DIR = headers

# Files from the input directories.
SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HDR_DIR)/*.h)

all: $(BIN_NAME)

# Main make target, compiles the program.
$(BIN_NAME): $(SOURCES) $(HEADERS)
	$(COMPILER) $(CFLAGS) -o $@ $(SOURCES) -I$(HDR_DIR) $(LIBS)

clean:
	rm -f $(BIN_NAME)