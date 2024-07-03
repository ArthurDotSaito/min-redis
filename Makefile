CC=g++
CFLAGS=-Wall -Wextra -O2 -g 
INCLUDES=-I./libs
SERVER_SRC=./server/server.cpp
CLIENT_SRC=./client/client/client.cpp ./client/request/request.cpp ./client/response/response.cpp ./io/io_func.cpp
BUILD_DIR=./build

SERVER_OBJ=$(BUILD_DIR)/server.o $(BUILD_DIR)/io_func.o
CLIENT_OBJ=$(BUILD_DIR)/client.o $(BUILD_DIR)/request.o $(BUILD_DIR)/response.o $(BUILD_DIR)/io_func.o

SERVER_EXE=server_app
CLIENT_EXE=client_app

all: $(SERVER_EXE) $(CLIENT_EXE)

$(SERVER_EXE): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(CLIENT_EXE): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(BUILD_DIR)/server.o: $(SERVER_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/client.o: ./client/client/client.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/request.o: ./client/request/request.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/response.o: ./client/response/response.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/io_func.o: ./io/io_func.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ)
	rm -f $(SERVER_EXE) $(CLIENT_EXE)
	rm -rf $(BUILD_DIR)
