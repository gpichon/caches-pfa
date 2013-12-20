CC=gcc 
FLAGS=-g -W -Wall
XMLFLAGS= `xml2-config --cflags` `xml2-config --libs`

OBJS=cache block line list add_line_hierarchy architecture trace

BUILD_DIR=obj
SRC_DIR=src
DATA_DIR=bin
TESTS_DIR=tests

EXE=main

# main rules
all: $(BUILD_DIR) $(DATA_DIR)/$(EXE)
test1: $(BUILD_DIR) $(DATA_DIR)/$(TEST1)
test2: $(BUILD_DIR) $(DATA_DIR)/$(TEST2)
test3: $(BUILD_DIR) $(DATA_DIR)/$(TEST3)

# executable files
$(DATA_DIR)/$(EXE): $(SRC_DIR)/$(addsuffix .c,$(EXE)) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS)))
	${CC} ${FLAGS} $^ -o $@ ${LIBS} $(XMLFLAGS)

# objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(addprefix $(SRC_DIR)/,$(addsuffix .h,$(OBJS)))
	${CC} ${FLAGS} -c $< -o $@ $(XMLFLAGS)


# dirs
$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	mkdir $(DATA_DIR)

# clean
clean: 
	rm -rf $(BUILD_DIR) $(DATA_DIR) *~ */*~ *.mod $(EXE) $(TESTS)

# tests
testarchi: $(addprefix $(SRC_DIR)/,$(addsuffix .c,$(OBJS))) $(TESTS_DIR)/test_architecture.c
	$(CC) $(FLAGS) $^ -o $(DATA_DIR)/testarchi $(XMLFLAGS)

testmesi: $(addprefix $(SRC_DIR)/,$(addsuffix .c,$(OBJS))) $(TESTS_DIR)/test_without_parser.c
	$(CC) $(FLAGS) $^ -o $(DATA_DIR)/testmesi $(XMLFLAGS)

testreplacement: $(addprefix $(SRC_DIR)/,$(addsuffix .c,$(OBJS))) $(TESTS_DIR)/test_replacement.c
	$(CC) $(FLAGS) $^ -o $(DATA_DIR)/testreplacement $(XMLFLAGS)
