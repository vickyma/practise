CC          ?= gcc
  2 QUIET       ?=
  3 PREFIX      ?= .
  4 INCPATH     ?= $(TOPDIR)/include
  5 LIBPATH     ?= $(TOPDIR)/libs
  6 LIBNAME     ?= undef
  7 LDFLAGS     ?= $(EXTRALIB) -lpthread -lrt -lm -lc
  8 SHARED      ?= -shared -o
  9
 10 CFLAGS      ?= -g3 -O2 -Wall -W -Wno-error -pipe -fPIC
 11 CXXFLAGS    ?= -g3 -O2 -Wall -W -Wno-error -pipe -fPIC
 12
 13 DEBUG       ?=
 14
 15 HEADERS     = $(wildcard *.h)
 16
 17 .c.o:
 18     $(QUIET)echo -e "  CC   "$<
 19     $(QUIET)$(CC) $(CFLAGS) -c -I$(INCPATH) $(DEBUG) $< -o $@
 20
 21 .cpp.o:
 22     $(QUIET)echo -e "  CXX  "$<
 23     $(QUIET)$(CXX) $(CXXFLAGS) -c -I$(INCPATH) $(DEBUG) $< -o $@
 24
 25
 26 ifdef TEST_PROG
 27 $(TEST_PROG): ${TEST_OBJS}
 28     $(QUIET)$(CC) $(CFLAGS) -I$(INCPATH) -L$(LIBPATH) $(DEBUG) $< -o $@ $(TEST_LIBS)
 29 all:$(TEST_PROG)
 30 endif
 31
 32 all: install_headers install_libs
 33     #uninstall_headers uninstall_libs
 34
 35
 36 .PHONY: all
