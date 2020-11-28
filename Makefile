DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin
DIR_CGI = .


SRC = $(wildcard $(DIR_SRC)/*.cpp)
OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir $(SRC)))

CXX = g++
CFLAGS = -std=c++14 -g -Wall -I$(DIR_INC) -pedantic -lpthread -lboost_system -lboost_filesystem
CXX_INCLUDE_DIRS = /usr/local/include
CXX_INCLUDE_PARAMS = $(addprefix -I, $(CXX_INCLUDE_DIRS))
CXX_LIB_DIRS = /usr/local/lib
CXX_LIB_PARAMS = $(addprefix -L, $(CXX_LIB_DIRS))

TARGET = http_server
TARGET2 = cgi_server
CGI_TARGET = console

mkdir = 
CGI_SUFFIX =
CGI_FLAG =

ifeq ($(OS), Windows_NT)
	DIR_INC = include
	DIR_SRC = src
	DIR_OBJ = obj
	DIR_BIN = bin
	DIR_CGI = 
	mkdir = if not exist $(DIR_OBJ) mkdir $(DIR_OBJ)
	CFLAGS = -std=c++14 -g -Wall -I$(DIR_INC) -lws2_32 -lwsock32
	CXX_INCLUDE_PARAMS = 
	CXX_LIB_PARAMS = 
	CGI_SUFFIX = .o
	CGI_FLAG = -c -o
else
	mkdir = mkdir -p $(DIR_OBJ)
	CGI_SUFFIX = .cgi
	CGI_FLAG = -o
endif

all:

part1:$(TARGET) $(CGI_TARGET)

part2:$(TARGET2)

$(TARGET):$(filter-out $(DIR_OBJ)/$(TARGET2).o, $(OBJ))
	$(mkdir)
	$(CXX) -o $@ $^ $(CFLAGS) $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS)

$(TARGET2):$(filter-out $(DIR_OBJ)/$(TARGET).o, $(OBJ))
	$(mkdir)
	$(CXX) -o $@ $^ -lws2_32 -lwsock32 -std=c++14

$(CGI_TARGET) : $(DIR_OBJ)/$(CGI_TARGET).o
	$(mkdir)
	$(CXX) $(CGI_FLAG) $(DIR_CGI)/$@$(CGI_SUFFIX) $^ $(CFLAGS) $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(mkdir)
	$(CXX) -c -o $@ $< $(CFLAGS) $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS)


.PHONY:clean
clean:
	rm -rf $(DIR_OBJ)/*.o
	rm -f  $(DIR_CGI)/$(CGI_TARGET).cgi
	rm -f  $(TARGET)
