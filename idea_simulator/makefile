CC=gcc 
FLAGS=-g -W -Wall 

OBJS=add_line cache def_architecture replace_line search_line

BUILD_DIR=obj
SRC_DIR=src
DATA_DIR=bin

EXE=main

# main rules
all: $(BUILD_DIR) $(DATA_DIR)/$(EXE)


# executable files
$(DATA_DIR)/$(EXE): $(SRC_DIR)/$(addsuffix .c,$(EXE)) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS))) 
	${CC} ${FLAGS} $^ -o $@ ${LIBS}


# objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	${CC} ${FLAGS} -c $< -o $@ 


# dirs
$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	mkdir $(DATA_DIR)


# clean
clean: 
	rm -rf $(BUILD_DIR) $(DATA_DIR) *~ */*~ *.mod $(EXE)

# tests
