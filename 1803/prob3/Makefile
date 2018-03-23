CC = g++ 
CFLAGS=-std=c++11 -O3 -pthread
SRCS = temp.cpp utility.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = MyClient
HEADERS = timing.h utility.h
 
.SUFFIXES : .cpp .o
 
all : 
	$(CC) $(CFLAGS) $(SRCS) 