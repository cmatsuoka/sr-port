
COMMON_OBJS	= sin1024.o sdl.o opengl.o

COMMON_PATH	= common

COMMON_PATH_OBJS = $(addprefix $(COMMON_PATH)/,$(COMMON_OBJS))

COMMON_LIB	= libcommon.a

OBJS += $(COMMON_PATH_OBJS)

default-common:
	$(MAKE) -C .. common-all

common-all: $(COMMON_PATH)/$(COMMON_LIB)

$(COMMON_PATH)/$(COMMON_LIB): $(COMMON_PATH_OBJS)
	$(AR) r $@ $(COMMON_PATH_OBJS)
	$(RANLIB) $@

