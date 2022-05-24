CC=gcc
CFLAGS	= -g -Wall
LFLAGS	= -lncurses -lm
TARGET1 = 1player
TARGET2 = 2players

1player: $(TARGET1)
$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c $(LFLAGS)

2players: $(TARGET2)
$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c $(LFLAGS)

