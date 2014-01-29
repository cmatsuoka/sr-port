
PLZPART_OBJS	= opengl.o main.o

PLZPART_PATH	= plzpart-gl

PLZPART_PATH_OBJS = $(addprefix $(PLZPART_PATH)/,$(PLZPART_OBJS))

PLZPART_BIN	= plzpart

OBJS += $(PLZPART_PATH_OBJS)

default-plzpart:
	$(MAKE) -C .. plzpart-all

plzpart-all: $(PLZPART_PATH)/$(PLZPART_BIN)

$(PLZPART_PATH)/$(PLZPART_BIN): $(PLZPART_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(PLZPART_PATH_OBJS) $(LIBS)

plz.c: tweak.h ptau.pre
