CC = gcc
CFLAGS = -Wall -Wextra -O2 -Ithird_party/iup/include $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -Lthird_party/iup/lib -Wl,-rpath,'$$ORIGIN/third_party/iup/lib'

SRCS = main.c \
       openbmp.c \
       grayscale.c \
       brightness.c \
       invert.c \
       flip.c \
       rotate.c \
       crop.c \
       blur.c \
       sharpen.c

OBJS = $(SRCS:.c=.o)
TARGET = bmpedit

IUP_LIBS = -liup $(shell pkg-config --libs gtk+-3.0) -lm

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET) $(IUP_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
