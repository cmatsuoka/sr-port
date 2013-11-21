
LENS_OBJS	= main.o opengl.o

LENS_PATH	= lens

LENS_PATH_OBJS = $(addprefix $(LENS_PATH)/,$(LENS_OBJS))

LENS_BIN	= lens

OBJS += $(LENS_PATH_OBJS)

default-lens:
	$(MAKE) -C .. lens-all

lens-all: $(LENS_PATH)/$(LENS_BIN)

$(LENS_PATH)/$(LENS_BIN): $(LENS_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(LENS_PATH_OBJS) $(LIBS) -lSOIL
