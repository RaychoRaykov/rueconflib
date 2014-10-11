# Пътища до отделните директории
SRC_PATH=src
LIB_PATH=lib
OBJ_PATH=obj
INC_PATH=inc
BIN_PATH=bin

# Нстройки на компилатора
CC=g++
CFLAGS=-std=c++11 -fextended-identifiers -static -s -c -DDOLOG
LDFLAGS=-static-libgcc -static -s

# Списък на фаловете
SOURCES=Configuration.cpp config.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

# Наименование на тестовата програма
EXECUTABLE=testconfig

# Стандартна компилация
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJ_PATH)/$(OBJECTS) -o $(BIN_PATH)/$@

.cpp.o:
	$(CC) $(CFLAGS) $(SRC_PATH)/$< -o $(OBJ_PATH)/$@
