CC=gcc 
FLAGS=-g -W -Wall 

OBJS=add_line cache def_architecture replace_line search_line

BUILD_DIR=obj
SRC_DIR=src
DATA_DIR=bin

EXE=main
TESTS1=test_cache1
TESTS2=test_2caches

# main rules
all: $(BUILD_DIR) $(DATA_DIR)/$(EXE)
tests1: $(BUILD_DIR) $(DATA_DIR)/$(TESTS1)
tests2: $(BUILD_DIR) $(DATA_DIR)/$(TESTS2)

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
	rm -rf $(BUILD_DIR) $(DATA_DIR) *~ */*~ *.mod $(EXE) $(TESTS)

# tests
$(DATA_DIR)/$(TESTS1): $(SRC_DIR)/$(addsuffix .c,$(TESTS1)) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS))) 
	${CC} ${FLAGS} $^ -o $@ ${LIBS}

$(DATA_DIR)/$(TESTS2): $(SRC_DIR)/$(addsuffix .c,$(TESTS2)) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS))) 
	${CC} ${FLAGS} $^ -o $@ ${LIBS}
