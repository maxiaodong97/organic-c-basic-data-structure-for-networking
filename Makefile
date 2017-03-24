CC = gcc
CFLAGS = -Wall -g -D_DEBUG=1
CFLAGS += -I.
COMPILE = $(CC) $(CFLAGS) -c
OBJFILES := $(patsubst %.c,%.o,$(wildcard *.c))
TARGET = ut.exe

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $@ $(OBJFILES)

%.o: %.c
	$(COMPILE) -o $@ $<

clean:
	rm -fr *.o $(TARGET)

