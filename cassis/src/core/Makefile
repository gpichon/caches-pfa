#/*
# * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
# * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
# *
# * This Source Code Form is subject to the terms of the Mozilla Public
# * License, v. 2.0. If a copy of the MPL was not distributed with this
# * file, You can obtain one at http://mozilla.org/MPL/2.0/.
# */

TEXT := >>>>>Compilation of a core-automate file finished<<<<<

include ../conf.make


CFLAGS += -I ../includes-automate
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

