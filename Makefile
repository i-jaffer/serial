DIR_SRC := ./src
DIR_INC := ./include
DIR_OBJ := ./obj
DIR_LIB := ./lib

TARGET := example.out
LIB_TARGET := serial
CC := aarch64-linux-gnu-gcc
CFLAGS := -I ./include

BIN_TARGET := $(DIR_OBJ)/$(TARGET)
SRC := $(wildcard $(DIR_SRC)/*.c)
OBJ := $(patsubst %.c, $(DIR_OBJ)/%.o, $(notdir $(SRC)))

$(BIN_TARGET) : $(OBJ)
	@echo cc = $(CC)
	$(CC) $^ -o $@

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY : lib
lib : $(DIR_LIB)/lib$(LIB_TARGET).so $(DIR_LIB)/lib$(LIB_TARGET).a

$(DIR_LIB)/lib$(LIB_TARGET).so : $(DIR_SRC)/$(LIB_TARGET).c
	$(CC) $< $(CFLAGS) -shared -fPIC -o $@

$(DIR_LIB)/lib$(LIB_TARGET).a : $(DIR_OBJ)/$(LIB_TARGET).o
	ar crv $@ $<


.PHONY : clean
clean :
	-rm $(OBJ) $(BIN_TARGET)
	-rm $(DIR_LIB)/*.so
	-rm $(DIR_LIB)/*.a
