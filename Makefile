CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
BOOST_LIBS = -lboost_system -lpthread
INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin

TARGETS = $(BIN_DIR)/server $(BIN_DIR)/client

SERVER_OBJS = $(SRC_DIR)/server/server.cpp $(SRC_DIR)/resp/resp.cpp 
CLIENT_OBJS = $(SRC_DIR)/client/client.cpp $(SRC_DIR)/resp/resp.cpp

all: $(TARGETS)

$(BIN_DIR)/server: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) $(SERVER_OBJS) -o $(BIN_DIR)/server $(BOOST_LIBS)

$(BIN_DIR)/client: $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) $(CLIENT_OBJS) -o $(BIN_DIR)/client $(BOOST_LIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(BIN_DIR)/server $(BIN_DIR)/client
