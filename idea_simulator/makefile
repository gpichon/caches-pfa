CC=gcc 
FLAGS=-g -W -Wall
 
OBJS=cache block line

BUILD_DIR=obj
SRC_DIR=src
DATA_DIR=bin
TESTS_DIR=tests

EXE=main
TEST1=test_cache1
TEST2=test_2caches

# main rules
all: $(BUILD_DIR) $(DATA_DIR)/$(EXE)
test1: $(BUILD_DIR) $(DATA_DIR)/$(TEST1)
test2: $(BUILD_DIR) $(DATA_DIR)/$(TEST2)

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
$(DATA_DIR)/$(TEST1): $(TESTS_DIR)/$(addsuffix .c,$(TEST1)) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS))) 
	${CC} ${FLAGS} $^ -o $@ ${LIBS}

$(DATA_DIR)/$(TEST2): $(TESTS_DIR)/$(addsuffix .c,$(TEST2)) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS))) 
	${CC} ${FLAGS} $^ -o $@ ${LIBS}
