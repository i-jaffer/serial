DIR_SRC := ./src
DIR_INC := ./include
DIR_OBJ := ./obj
DIR_LIB := ./lib

TARGET := example.out
LIB_TARGET := serial
CC ?= gcc
CFLAGS := -I ./include

BIN_TARGET := $(DIR_OBJ)/$(TARGET)
SRC := $(wildcard $(DIR_SRC)/*.c)
OBJ := $(patsubst %.c, $(DIR_OBJ)/%.o, $(notdir $(SRC)))

$(BIN_TARGET) : $(OBJ)
	$(CC) $^ -o $@

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY : lib
lib : $(DIR_LIB)/lib$(LIB_TARGET).so $(DIR_LIB)/lib$(LIB_TARGET)_arm.so \
	$(DIR_LIB)/lib$(LIB_TARGET)_arm64.so \
	$(DIR_LIB)/lib$(LIB_TARGET)_x86.a \
	$(DIR_LIB)/lib$(LIB_TARGET)_arm.a \
	$(DIR_LIB)/lib$(LIB_TARGET)_arm64.a

$(DIR_LIB)/lib$(LIB_TARGET).so : $(DIR_SRC)/$(LIB_TARGET).c
	gcc $< $(CFLAGS) -shared -fPIC -o $@
$(DIR_LIB)/lib$(LIB_TARGET)_arm.so : $(DIR_SRC)/$(LIB_TARGET).c
	arm-linux-gnueabihf-gcc $< $(CFLAGS) -std=c99 -shared -fPIC -o $@
$(DIR_LIB)/lib$(LIB_TARGET)_arm64.so : $(DIR_SRC)/$(LIB_TARGET).c
	aarch64-linux-gnu-gcc $< $(CFLAGS) -shared -fPIC -o $@

$(DIR_LIB)/lib$(LIB_TARGET)_x86.a : $(DIR_OBJ)/$(LIB_TARGET).o
	gcc $(DIR_SRC)/$(LIB_TARGET).c -c $(CFLAGS) -o $(DIR_OBJ)/$(LIB_TARGET)_x86.o
	ar crv $@ $(DIR_OBJ)/$(LIB_TARGET)_x86.o
	-rm obj/serial.o
$(DIR_LIB)/lib$(LIB_TARGET)_arm.a : $(DIR_OBJ)/$(LIB_TARGET).o
	arm-linux-gnueabihf-gcc $(DIR_SRC)/$(LIB_TARGET).c -std=c99 -c $(CFLAGS) -o $(DIR_OBJ)/$(LIB_TARGET)_arm.o
	ar crv $@ $(DIR_OBJ)/$(LIB_TARGET)_arm.o
$(DIR_LIB)/lib$(LIB_TARGET)_arm64.a : $(DIR_OBJ)/$(LIB_TARGET).o
	aarch64-linux-gnu-gcc $(DIR_SRC)/$(LIB_TARGET).c -c $(CFLAGS) -o $(DIR_OBJ)/$(LIB_TARGET)_arm64.o
	ar crv $@ $(DIR_OBJ)/$(LIB_TARGET)_arm64.o

.PHONY : clean
clean :
	-rm $(OBJ) $(BIN_TARGET)
	-rm $(DIR_LIB)/*.so
	-rm $(DIR_LIB)/*.a
	-rm $(DIR_OBJ)/*
