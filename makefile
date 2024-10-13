# Set the compiler
CC = gcc

# Compilation flags: -Wall enables all warnings, -g for debugging info
CFLAGS = -Wall -g

# Source directory
SRC_DIR = src

# Output directory for object files
OBJ_DIR = obj

# Header files directory
INCLUDE_DIR = include

# List of source files (.c)
SRC = $(wildcard $(SRC_DIR)/*.c)

# Convert source files to object files (.o)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Name of the executable
TARGET = safepath

# Rule to create the final target (the executable)
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile each .c file into .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up all compiled files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Debug rules
debug: CFLAGS += -DDEBUG
debug: $(TARGET)
