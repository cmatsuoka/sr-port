
BEG_OBJS	= beg.o

BEG_PATH	= beg

BEG_PATH_OBJS = $(addprefix $(BEG_PATH)/,$(BEG_OBJS))

BEG_BIN	= beg

OBJS += $(BEG_PATH_OBJS)

default-beg:
	$(MAKE) -C .. beg-all

beg-all: $(BEG_PATH)/$(BEG_BIN)

$(BEG_PATH)/$(BEG_BIN): $(BEG_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(BEG_PATH_OBJS) $(LIBS) -lSOIL
