
TECHNO_OBJS	= koe.o koea.o

TECHNO_PATH	= techno

TECHNO_PATH_OBJS = $(addprefix $(TECHNO_PATH)/,$(TECHNO_OBJS))

TECHNO_BIN	= techno

OBJS += $(TECHNO_PATH_OBJS)

default-techno:
	$(MAKE) -C .. techno-all

techno-all: $(TECHNO_PATH)/$(TECHNO_BIN)

$(TECHNO_PATH)/$(TECHNO_BIN): $(TECHNO_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(TECHNO_PATH_OBJS) $(LIBS)
