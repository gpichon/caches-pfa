#/*
# * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
# * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
# *
# * This Source Code Form is subject to the terms of the Mozilla Public
# * License, v. 2.0. If a copy of the MPL was not distributed with this
# * file, You can obtain one at http://mozilla.org/MPL/2.0/.
# */

TEXT := >>>>>Compilation of a state machine file finished<<<<<


#################################################################
# Macros.
#

SM_SOURCES=     coherence.sm
SOURCES=        $(SM_SOURCES:.sm=_sm.c) \
	        coherence.c
HEADERS=        $(patsubst %.h,../includes-automate/%.h,$(SM_SOURCES:.sm=_sm.h)) \
		../includes-automate/coherence.h
OBJECTS=        $(patsubst %.o,../../obj/%.o,$(SOURCES:.c=.o))

# Uncomment to turn on debug message generation.
TRACE=          -g

SMC=            java -jar bin/Smc.jar
SMC_FLAGS=      -c $(TRACE)

CC=             gcc
CFLAGS=         -g -I../includes-automate

RM_F=           rm -f

#################################################################
# Rules.
#

#usefull for not remove temporary files (here coherence_sm.c)
.SECONDARY: $(SOURCES)

../includes-automate/%_sm.h %_sm.c : 	%.sm
					@$(SMC) $(SMC_FLAGS) $<
					$(info $(TEXT))
					$(info $*.sm)


%_sm.dot :      %.sm
		$(SMC) -graph -glevel 2 $<

%_sm.png :      %_sm.dot
		dot -T png -o $@ $<

%_sm.html :     %.sm
		$(SMC) -table $<

all :           $(OBJECTS)

../../obj/%.o :	%.c
		@$(CC) $(CFLAGS) -c $< -o $@
		$(info $(TEXT))
		$(info $*.c)

graph :         $(SM_SOURCES:%.sm=%_sm.dot)

png :           $(SM_SOURCES:%.sm=%_sm.png)

table :         $(SM_SOURCES:%.sm=%_sm.html)

clean :
		-$(RM_F) $(OBJECTS)
		-$(RM_F) *_sm.h
		-$(RM_F) *_sm.c
		-$(RM_F) *_sm.dot
		-$(RM_F) *_sm.png
		-$(RM_F) *_sm.html
