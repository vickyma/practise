TOPDIR=..

#OBJ		= $(addsuffix .o, $(basename $(wildcard *.c))) $(addsuffix .o, $(basename $(wildcard *.cpp)))

LIBNAME		=libbfilter.so
OBJS		= bfilter.o

TEST_PROG	?= bfilter_bench_test 
TEST_OBJS	?= bfilter_bench_test.o 
TEST_LIBS   ?= -lbfilter

.PHONY: all clean

all:$(OBJS) $(TEST_PROG)


include $(TOPDIR)/rule.make
