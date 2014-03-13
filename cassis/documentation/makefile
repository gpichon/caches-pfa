TEXT = >>>>>Compilation of documentation<<<<<

SRCS:= $(shell find ../ -name '*.c')
SRCS_H:= $(shell find ../ -name '*.h')

docu: html/index.html

html/index.html: $(SRCS) $(SRCS_H)
	$(info $(TEXT))
	@doxygen doxygen.conf

clean:
	rm -rf html
