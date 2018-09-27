CC = gcc
CCFLAGS = -std=c99

SRCS = $(wildcard src/*.c src/lib/*.c)
OBJS = $(SRCS:.c = .o)

OUTPUT = scheme.exe
all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CCFLAGS)
%.o: %.c
	$(CC) -c $< $(CCFLAGS)
