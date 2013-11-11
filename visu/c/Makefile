
VISU_OBJS	= vec.o main.o zoomer.o math.o mathsin.o opengl.o

VISU_PATH	= visu/c

VISU_PATH_OBJS = $(addprefix $(VISU_PATH)/,$(VISU_OBJS))

VISU_BIN	= u2a

OBJS += $(VISU_PATH_OBJS)

default-visu:
	$(MAKE) -C ../.. visu-all

visu-all: $(VISU_PATH)/$(VISU_BIN)

$(VISU_PATH)/u2a: $(VISU_PATH)/u2a.o
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL

$(VISU_PATH)/cplay: $(VISU_PATH)/cplay.o
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL
