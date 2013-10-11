CC	= gcc
CFLAGS	= -g -O0 -Wall -Wno-unused-function
LD	= gcc
LDFLAGS	=
LIBS	= -lm -lGL -lEGL -lGLU -lSDL -lX11
OBJS	= dots.o sdl.o opengl.o sin1024.o

.c.o:
	$(CC) -c $(CFLAGS) -o $*.o $<

all: poc

poc: $(OBJS) Makefile
	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	rm -f core *~ $(OBJS) depend

depend:
	@echo Generating dependencies...
	@$(CC) -MM $(CFLAGS) $(OBJS:.o=.c) > $@

include depend
