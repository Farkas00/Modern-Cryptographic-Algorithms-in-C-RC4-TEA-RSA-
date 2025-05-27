CC = gcc
CFLAGS = -Wall
OBJS = main.o rc4.o tea.o rsa.o
TARGET = crypto

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

