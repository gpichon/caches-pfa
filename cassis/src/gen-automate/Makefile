TEXT := >>>>>Compilation of a state machine file finished<<<<<


#################################################################
# Macros.
#

SM_SOURCES=     coherence.sm
SOURCES=        $(SM_SOURCES:.sm=_sm.c) \
	        coherence.c
HEADERS=        $(SM_SOURCES:.sm=_sm.h) \
		../includes-automate/coherence.h
OBJECTS=        $(SOURCES:.c=.o)

# Uncomment to turn on debug message generation.
TRACE=          -g

SMC=            java -jar bin/Smc.jar
SMC_FLAGS=      -c $(TRACE)

CC=             gcc
CFLAGS=         -g -I../includes-automate -c

RM_F=           rm -f

#################################################################
# Rules.
#

%_sm.h %_sm.c : %.sm
		$(SMC) $(SMC_FLAGS) $<
		$(info $(TEXT))
		$(info $*_sm.c)
		$(info $*_sm.h)
		mv $*_sm.h ../includes-automate/$*_sm.h

%_sm.dot :      %.sm
		$(SMC) -graph -glevel 1 $<

%_sm.png :      %_sm.dot
		dot -T png -o $@ $<

%_sm.html :     %.sm
		$(SMC) -table $<

all :           $(HEADERS) $(SOURCES)
		$(CC) $(CFLAGS) $(SOURCES)
		$(info $(TEXT))
		$(info $(SOURCES))
		mv *.o ../../obj/



graph :         $(SM_SOURCES:%.sm=%_sm.dot)

png :           $(SM_SOURCES:%.sm=%_sm.png)

table :         $(SM_SOURCES:%.sm=%_sm.html)

clean :
		-$(RM_F) *_sm.h
		-$(RM_F) *_sm.c
		-$(RM_F) *_sm.dot
		-$(RM_F) *_sm.png
		-$(RM_F) *_sm.html
