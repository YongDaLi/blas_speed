CC = gcc	# compiler
LD = gcc	# linker

LDFLAGS = -g # linker flags

# use regular Netlib BLAS
# have header include path at the start, before -Wall, or else debug symbols don't get created
CFLAGS = -I/usr/local/include -Iutil -I -Wall -g -c	# compiler flags
LIBS = -lblas -lm

# use AMD Ryzen BLAS, statically link
# CFLAGS = -Wall -I/usr/local/include -I/home/liyongda/Downloads/amd-blis/include/LP64 -Iutil -I -g -c	# compiler flags
# LIBS = /home/liyongda/Downloads/amd-blis/lib/LP64/libblis.a  -lm -lpthread

SOURCES :=	\
	main.c	\
	mymatrix.c	\
	logger.c 	\

HEADERS := $(SOURCES:.c=.h)
OBJECTS := $(SOURCES:.c=.o)
DEPENDENCIES := $(SOURCES:.c=.d)

$(info $$SOURCES is [${SOURCES}])
$(info $$HEADERS is [${HEADERS}])
$(info $$OBJECTS is [${OBJECTS}])
$(info $$DEPENDENCIES is [${DEPENDENCIES}])

all: main.out

# link all objects together into single executable
main.out: $(OBJECTS)
	$(LD) $^ $(LDFLAGS) $(LIBS) -o $@

# for every item in objects list, it depends on the *.o -> *.c version of the code
$(OBJECTS): %.o:%.c
# produce a .d makefile syntax file that contains the header file dependencies
	$(CC) -MT $@ -MM $(CFLAGS) $< > $(<:%.c=%.d)
# for every .c file, compile a produce a .o file
	$(CC) $(CFLAGS) $< -o $@

# pull in dependency information for existing .o files
 -include $(OBJECTS:.o=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDENCIES)

run: main.out
	./$<

debug: main.out
	gdb $<

test: test.out
	./$<
