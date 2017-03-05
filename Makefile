CC	= gcc
CFLAGS	= -g -Wall
LFLAGS	= -lncurses -lm
TARGET = main
all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)
