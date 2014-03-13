TEXT := >>>>>Compilation finished<<<<<

DOC_DIR:=documentation
OBJ_DIR:=obj
SRC_DIR:=src
BIN_DIR:=bin


# main rules
all: dir doc main
	$(info =====>Compilation finished)

main:
	@make -s -C $(SRC_DIR)


# dirs
dir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# clean
clean:  
	@make -s -C $(DOC_DIR) clean
	@make -s -C $(SRC_DIR) clean
	@rm -f $(OBJ_DIR)/* $(BIN_DIR)/*

# tests
test: main_test
	$(info =====>Compilation finished)

main_test:
	@make -s -C $(SRC_DIR) test

# install documentation
doc: 
	@make -s -C $(DOC_DIR) docu
