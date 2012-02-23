# Makefile
all:
	@scons -Q
clean:
	@scons -Q -c
doc:
	markdown README.mkd > README.html
	doxygen Doxygen
.PHONY: all clean doc
