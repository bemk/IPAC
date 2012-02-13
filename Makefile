MAKE=make

LINUX=linux
WINDOWS=windows

.PHONY: all $(WINDOWS) $(LINUX) clean
all: $(WINDOWS)

$(LINUX):
	$(MAKE) -f Makefile.$(LINUX) linux

$(WINDOWS):
	$(MAKE) -f Makefile.$(WINDOWS)

clean:
	$(MAKE) -f Makefile.$(LINUX) clean
