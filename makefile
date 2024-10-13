# Set the compiler
CC = gcc

# Compilation flags: -Wall enables all warnings, -g for debugging info
CFLAGS = -Wall -g

# Linker flags for the HTTP server (libmicrohttpd)
LDFLAGS = -lmicrohttpd

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
	@echo "Linking object files to create the executable: $(TARGET)"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile each .c file into .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $< into object file $@"
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	@echo "Creating object directory: $(OBJ_DIR)"
	mkdir -p $(OBJ_DIR)

# Clean up all compiled files
clean:
	@echo "Cleaning up object files and executable"
	rm -rf $(OBJ_DIR) $(TARGET)

# Debug rules to enable debugging mode
debug: CFLAGS += -DDEBUG
debug: $(TARGET)
	@echo "Debug mode enabled"
