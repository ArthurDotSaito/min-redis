CC=g++
CFLAGS=-Wall -Wextra -O2 -g 
INCLUDES=-I./libs
SERVER_SRC=./server/server.cpp
CLIENT_SRC=./client/client/client.cpp ./client/request/request.cpp ./client/response/response.cpp ./io/io_func.cpp
BUILD_DIR=./build

SERVER_OBJ=$(BUILD_DIR)/server.o
CLIENT_OBJ=$(BUILD_DIR)/client.o $(BUILD_DIR)/request.o $(BUILD_DIR)/response.o $(BUILD_DIR)/io_func.o

all: server client

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@  # Tab indentation here

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@  # Tab indentation here

# Object file rules (note the automatic dependency generation)
$(BUILD_DIR)/%.o: ./%.cpp
	mkdir -p $(BUILD_DIR)  
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@  # Tab indentation here

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) # Remove only object files and executables
	rm -f server client  # Remove executables
