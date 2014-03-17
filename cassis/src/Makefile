TEXT1 := >>>>>main compiled<<<<<
TEXT2 := >>>>>test main compiled<<<<<

include conf.make

ifeq ($(AUTOMATE),NO)
	CFLAGS += -I includes
	SRC:=main.c
	OBJ:=main.o
else
	CFLAGS += -I includes-automate
	SRC:=main_automate.c
	OBJ:=main_automate.o
endif

SRC_TEST:=main_test.c
OBJ_TEST:=main_test.o
OBJS=$(shell find ../obj -name '*.o')

#règles générales

all: exe ../bin/simulator

test: test_exe ../bin/test

#règles pour le main

exe:
ifeq ($(AUTOMATE),NO)
	@make -s -C core
else
	@make -s -C gen-automate
	@make -s -C core-automate
endif

../bin/simulator: ../obj/$(OBJ) $(subst ../obj/$(OBJ),,$(subst ../obj/$(OBJ_TEST),,$(OBJS)))
	@$(CC) $(CFLAGS)  $(subst ../obj/$(OBJ_TEST),,$(OBJS)) -o $@ $(XMLFLAGS)
ifeq ($(DEBUG),YES)
	$(info !!Sources compiled for debug!!)
endif
	$(info $(TEXT1))


../obj/$(OBJ): $(SRC)
	@$(CC) -MMD $(CFLAGS) -c $< -o $@ $(XMLFLAGS)
	@cp ../obj/$*.d ./$(subst .c,.P,$(SRC)); \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < ../obj/$*.d >> ./$(subst .c,.P,$(SRC)); \
		rm -f ../obj/$*.d

-include $(subst .c,.P,$(SRC))


#règles pour le main de test

test_exe:
ifeq ($(AUTOMATE),NO)
	@make -s -C core
else
	@make -s -C gen-automate
	@make -s -C core-automate
endif
	@make -s -C tests

../bin/test:  ../obj/$(OBJ_TEST) $(subst ../obj/$(OBJ),,$(subst ../obj/$(OBJ_TEST),,$(OBJS)))
	$(CC) $(CFLAGS) $(subst ../obj/$(OBJ),,$(OBJS)) -o $@ $(XMLFLAGS)
ifeq ($(DEBUG),YES)
	$(info !!Sources compiled for debug!!)
endif
	$(info $(TEXT2))


../obj/$(OBJ_TEST): $(SRC_TEST)
	@$(CC) -MMD $(CFLAGS) -c $< -o $@ $(XMLFLAGS)
	@cp ../obj/$*.d ./$(subst .c,.P,$(SRC_TEST)); \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < ../obj/$*.d >> ./$(subst .c,.P,$(SRC_TEST)); \
		rm -f ../obj/$*.d

-include $(subst .c,.P,$(SRC_TEST))


#règles de mrpropre

clean: 
	rm -f ../bin/simulator ../obj/$(OBJ) $(subst .c,.P,$(SRC))
	rm -f ../bin/test ../obj/$(OBJ_TEST) $(subst .c,.P,$(SRC_TEST))
	@make -s -C core clean
	@make -s -C tests clean
	@make -s -C core-automate clean
	@make -s -C gen-automate clean
