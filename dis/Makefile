
DIS_OBJS	= disc.o

DIS_PATH	= dis

DIS_PATH_OBJS	= $(addprefix $(DIS_PATH)/,$(DIS_OBJS))

DIS_LIB		= libdis.a

OBJS += $(DIS_PATH_OBJS)

default-dis:
	$(MAKE) -C .. dis-all

dis-all: $(DIS_PATH)/$(DIS_LIB)

$(DIS_PATH)/$(DIS_LIB): $(DIS_PATH_OBJS)
	$(AR) r $@ $(DIS_PATH_OBJS)
	$(RANLIB) $@

