CC	= gcc
CFLAGS	= -g -O3 -Wall -Icommon
LD	= gcc
LDFLAGS	=
LIBS	= -Lcommon -lcommon -lm -lGL -lEGL -lGLU -lSDL -lX11
RANLIB	= ranlib
AR	= ar

all: common/libcommon.a dots/dots

include common/Makefile
include dots/Makefile

.SUFFIXES: .c .o .pov .a

.c.o:
	$(CC) -c $(CFLAGS) -o $*.o $<

clean:
	rm -f core *~ $(OBJS) depend

depend:
	@echo Generating dependencies...
	@$(CC) -MM $(CFLAGS) $(OBJS:.o=.c) > $@

include depend
