CC	= gcc
CFLAGS	= -g -O0 -Wall -Icommon
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
	@rm -f $@
	@for i in $(OBJS); do \
	  j=`echo $$i | sed 's/\.o$$/.c/'`; \
	  $(CC) -MM -MT $$i $(CFLAGS) $$j >> $@; \
	done
	

include depend
