
COMMON_OBJS	= sin1024.o

COMMON_PATH	= common

COMMON_PATH_OBJS = $(addprefix $(COMMON_PATH)/,$(COMMON_OBJS))

COMMON_LIB	= libcommon.a

OBJS += $(COMMON_PATH_OBJS)

default-dots:
	$(MAKE) -C ..

$(COMMON_PATH)/$(COMMON_LIB): $(COMMON_PATH_OBJS)
	$(AR) r $@ $(COMMON_PATH_OBJS)
	$(RANLIB) $@
