CC = g++ 
CFLAGS=-std=c++11 -O3 -pthread
SRCS = main.cpp utility.cpp progress.cpp interpret.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = MyClient
HEADERS = timing.h utility.h
 
.SUFFIXES : .cpp .o
 
all : 
	$(CC) $(CFLAGS) $(SRCS) 