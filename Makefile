CC	= gcc
CFLAGS	= -g -O3 -Wall -Icommon
LD	= gcc
LDFLAGS	=
LIBS	= -Lcommon -lcommon -Ldis -ldis -lm -lGL -lEGL -lGLU -lSDL -lX11
RANLIB	= ranlib
AR	= ar
POVRAY	= povray

all: common-all dis-all dots-all water-all tunneli-all

include common/Makefile
include dis/Makefile
include dots/Makefile
include water/Makefile
include tunneli/Makefile
include glenz/Makefile
include techno/Makefile

.SUFFIXES: .c .o .a .pov .png

.pov.png:
	$(POVRAY) +W1024 +H768 $<

.c.o:
	$(CC) -c $(CFLAGS) -o $*.o $<

clean:
	rm -f core *~ $(OBJS) depend

depend:
	@echo Generating dependencies...
	@$(CC) -MM $(CFLAGS) $(OBJS:.o=.c) > $@

include depend
