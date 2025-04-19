CC := gcc
AR := ar

SRCS := core/gpio.c core/i2c.c 

BUILD_DIR := build
EXE_NAME := libCar

OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

INCLUDE_DIRS := . ./utilities

CFLAGS := -Wall -Wextra -Werror -g $(INCLUDE_DIRS:%= -I%)
LDFLAGS := -lrt -lm

INSTALL_DIR := bin

$(EXE_NAME): $(OBJS) 
	$(AR) rcs $@.a $^

dirs: 
	mkdir -p build/core
	mkdir -p build/utilities
	mkdir -p build/drivers


$(BUILD_DIR)/%.o: %.c dirs
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -r build
	rm -r bin
	rm $(EXE_NAME).a


.PHONY: clean

install_dirs:
	mkdir -p $(INSTALL_DIR)/lib
	mkdir -p $(INSTALL_DIR)/include

install: $(EXE_NAME) install_dirs

	mv $(EXE_NAME).a $(INSTALL_DIR)/lib
	find . -name '*.h' -exec cp --parents {} $(INSTALL_DIR)/include \;
