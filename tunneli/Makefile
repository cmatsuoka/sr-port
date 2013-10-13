
TUNNELI_OBJS	= tun10.o ballgen2.o singen.o opengl.o

TUNNELI_PATH	= tunneli

TUNNELI_PATH_OBJS = $(addprefix $(TUNNELI_PATH)/,$(TUNNELI_OBJS))

TUNNELI_BIN	= tun10

OBJS += $(TUNNELI_PATH_OBJS)

default-tunneli:
	$(MAKE) -C .. tunneli-all

tunneli-all: $(TUNNELI_PATH)/$(TUNNELI_BIN)

$(TUNNELI_PATH)/$(TUNNELI_BIN): $(TUNNELI_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(TUNNELI_PATH_OBJS) $(LIBS)
