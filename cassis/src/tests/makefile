TEXT := >>>>>Compilation of a test file finished<<<<<

include ../includes/conf.make


CFLAGS += -I ../includes
SRCS=$(wildcard *.c) 
OBJS=$(patsubst %.c,../../obj/%.o,$(SRCS))


all:$(OBJS)


../../obj/%.o:%.c
	@$(CC) -MMD $(CFLAGS) -c $< -o $@ $(XMLFLAGS)
	@cp ../../obj/$*.d ./$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < ../../obj/$*.d >> ./$*.P; \
		rm -f ../../obj/$*.d
	$(info $(TEXT))
	$(info $*.c)

-include *.P

clean: 
	rm -f *.P
	rm -f $(OBJS)
