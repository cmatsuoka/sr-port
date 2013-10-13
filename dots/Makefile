
DOTS_OBJS	= main.o opengl.o

DOTS_PATH	= dots

DOTS_PATH_OBJS	= $(addprefix $(DOTS_PATH)/,$(DOTS_OBJS))

DOTS_BIN	= dots

OBJS += $(DOTS_PATH_OBJS)

default-dots:
	$(MAKE) -C .. dots-all

dots-all: $(DOTS_PATH)/$(DOTS_BIN)

$(DOTS_PATH)/$(DOTS_BIN): $(DOTS_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(DOTS_PATH_OBJS) $(LIBS)
