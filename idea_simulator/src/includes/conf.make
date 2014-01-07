DEBUG=YES

CC=gcc 
CFLAGS=-W -Wall
XMLFLAGS= `xml2-config --cflags` `xml2-config --libs` `xslt-config --cflags` `xslt-config --libs`

ifeq ($(DEBUG),YES)
     CFLAGS += -g
endif