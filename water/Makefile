
WATER_OBJS	=

WATER_PATH	= water

WATER_PATH_OBJS	= $(addprefix $(WATER_PATH)/,$(WATER_OBJS))

WATER_BIN	= water

OBJS += $(WATER_PATH_OBJS)

default-water:
	$(MAKE) -C ..

water_all:

water_image: $(WATER_PATH)/kk.png

$(WATER_PATH)/$(WATER_BIN): $(WATER_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(WATER_PATH_OBJS) $(LIBS)
