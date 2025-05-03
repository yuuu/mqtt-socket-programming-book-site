CC = gcc
CFLAGS = -Wall -Wextra -O2 -I ../../include
TARGET = main
SRC = main.c ../mqtt.c ../tcp.c


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
