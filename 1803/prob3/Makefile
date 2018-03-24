CC = g++ 
CFLAGS=-std=c++11 -O3 -pthread
SRCS = utility.cpp progress.cpp interpret.cpp function.cpp constant.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = MyClient
HEADERS = timing.h utility.h
 
.SUFFIXES : .cpp .o
 
all:
	$(CC) $(CFLAGS) $(SRCS) main.cpp
temp :
    $(CC) $(CFLAGS) $(SRCS) temp.cpp
