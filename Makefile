CC	= gcc
CFLAGS	= -g -Wall
LFLAGS	= -lncurses
TARGET = main
all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)
