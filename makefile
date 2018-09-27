CC = gcc
CCFLAGS = -std=c99

SRCS = $(wildcard src/*.c src/lib/*.c)
OBJS = $(SRCS:.c=.o)

OUTPUT = scheme


all: $(OUTPUT)


$(OUTPUT): $(OBJS)
	$(CC) -o $@ $(notdir $^)

%.o: %.c
	$(CC) -c $< $(CCFLAGS) -o $(notdir $@)

.PHONY: clean
clean:
	-rm -rf *.o
