
PLZPART_OBJS	= plz.o plza.o plzfill.o copper.o asmyt.o tweak.o \
		  vect.o main.o sinit.o spline.o vga.o opengl.o

PLZPART_PATH	= plzpart

PLZPART_PATH_OBJS = $(addprefix $(PLZPART_PATH)/,$(PLZPART_OBJS))

PLZPART_BIN	= plzpart

OBJS += $(PLZPART_PATH_OBJS)

default-plzpart:
	$(MAKE) -C .. plzpart-all

plzpart-all: $(PLZPART_PATH)/$(PLZPART_BIN)

$(PLZPART_PATH)/$(PLZPART_BIN): $(PLZPART_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(PLZPART_PATH_OBJS) $(LIBS)

plz.c: tweak.h ptau.pre
