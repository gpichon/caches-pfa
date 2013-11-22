CC=gcc 
FLAGS=-g -W -Wall

OBJS=read_traces
EXE=test
BUILD_DIR=.
SRC_DIR=.

# main rules
all: $(EXE)

# executable files
$(EXE): $(addprefix $(SRC_DIR)/,$(addsuffix .c,$(EXE))) $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(OBJS))) 
	${CC} ${FLAGS} $^ -o $@ ${LIBS}

# objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	${CC} ${FLAGS} -c $< -o $@ 

# clean
clean: 
	rm -rf *~ *.o 
