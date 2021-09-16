DIR_SRC := ./src
DIR_INC := ./include
DIR_OBJ := ./obj
DIR_LIB := ./lib

TARGET := example.out
CC := gcc
CFLAGS := -I ./include

BIN_TARGET := $(DIR_OBJ)/$(TARGET)
SRC := $(wildcard $(DIR_SRC)/*.c)
OBJ := $(patsubst %.c, $(DIR_OBJ)/%.o, $(notdir $(SRC)))

$(BIN_TARGET) : $(OBJ)
	$(CC) $^ -o $@

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY : lib
lib : $(DIR_LIB)/libserial.so $(DIR_LIB)/libserial_arm.so $(DIR_LIB)/libserial_arm64.so

$(DIR_LIB)/libserial.so : $(DIR_SRC)/serial.c
	gcc $< $(CFLAGS) -shared -fPIC -o $@
$(DIR_LIB)/libserial_arm.so : $(DIR_SRC)/serial.c
	arm-linux-gnueabihf-gcc $< $(CFLAGS) -std=c99 -shared -fPIC -o $@
$(DIR_LIB)/libserial_arm64.so : $(DIR_SRC)/serial.c
	aarch64-linux-gnu-gcc $< $(CFLAGS) -shared -fPIC -o $@

.PHONY : clean
clean :
	-rm $(OBJ) $(BIN_TARGET)
	-rm $(DIR_LIB)/*.so