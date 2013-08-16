CC			?= gcc
QUIET		?= 
PREFIX		?= .
INCPATH		?= $(TOPDIR)/include
LIBPATH		?= $(TOPDIR)/libs
LIBNAME		?= undef
LDFLAGS     ?= $(EXTRALIB) -lpthread -lrt -lm -lc
SHARED		?= -shared -o

CFLAGS		?= -g3 -O2 -Wall -W -Wno-error -pipe -fPIC
CXXFLAGS	?= -g3 -O2 -Wall -W -Wno-error -pipe -fPIC

DEBUG		?=

HEADERS		= $(wildcard *.h)

.c.o:
	$(QUIET)echo -e "  CC	"$<
	$(QUIET)$(CC) $(CFLAGS) -c -I$(INCPATH) $(DEBUG) $< -o $@

.cpp.o:
	$(QUIET)echo -e "  CXX	"$<
	$(QUIET)$(CXX) $(CXXFLAGS) -c -I$(INCPATH) $(DEBUG) $< -o $@


ifdef TEST_PROG	
$(TEST_PROG): ${TEST_OBJS} 
	$(QUIET)$(CC) $(CFLAGS) -I$(INCPATH) -L$(LIBPATH) $(DEBUG) $< -o $@ $(TEST_LIBS) 
all:$(TEST_PROG)
endif

all: install_headers install_libs
	#uninstall_headers uninstall_libs 


.PHONY: all

clean: uninstall_headers uninstall_libs
	$(QUIET)rm -rf $(OBJS)
	$(QUIET)find . -name "*~" | xargs rm -rf
ifdef TEST_PROG	
	$(QUIET)rm -rf $(TEST_PROG) $(TEST_OBJS)	
endif

install_headers:
	$(QUIET)mkdir -p $(INCPATH)
	$(QUIET)cp $(HEADERS) $(INCPATH)

install_libs: $(OBJS)
	$(QUIET)mkdir -p $(LIBPATH)
	$(QUIET)echo "  ld	"$(LIBPATH)/$(LIBNAME)
	$(QUIET)$(LD) $(SHARED) $(LDFLAGS) $(OBJS) -o $(LIBPATH)/$(LIBNAME)

#$(QUIET)ar csr $(LIBPATH)/$(LIBNAME) $(OBJS)

uninstall_headers:
	$(QUIET)for file in $(HEADERS); do rm -rf $(INCPATH)/$$file; done

uninstall_libs:
	$(QUIET)rm -rf $(LIBPATH)/$(LIBNAME)
