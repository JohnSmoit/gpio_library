CC := gcc
AR := ar

SRCS := gpio.c i2c.c

BUILD_DIR := build
EXE_NAME := libCar

OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

CFLAGS := -Wall -Wextra -g -I.
LDFLAGS := -lrt -lm

INSTALL_DIR := bin

$(EXE_NAME): $(OBJS) 
	$(AR) rcs $@.a $^

dirs: 
	mkdir -p build


$(BUILD_DIR)/%.o: %.c dirs
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -r build
	rm $(EXE_NAME).a
.PHONY: clean

install: $(EXE_NAME)
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/include
	mkdir -p $(INSTALL_DIR)/lib

	mv $(EXE_NAME).a $(INSTALL_DIR)/lib
	cp ./*.h $(INSTALL_DIR)/include
