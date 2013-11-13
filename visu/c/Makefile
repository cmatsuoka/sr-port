
VISU_OBJS	= avid.o visu.o acalc.o adata.o avistan.o adraw.o opengl.o

VISU_PATH	= visu/c

VISU_PATH_OBJS = $(addprefix visu/,$(VISU_OBJS))

VISU_BIN	= u2a

#OBJS += $(VISU_PATH_OBJS)

default-visu:
	$(MAKE) -C ../.. visu-all

visu-all: $(VISU_PATH)/$(VISU_BIN)

$(VISU_PATH)/u2a: $(VISU_PATH)/u2a.o $(VISU_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL

$(VISU_PATH)/cplay: $(VISU_PATH)/cplay.o
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL
