
VISU_OBJS	= avid.o visu.o acalc.o adata.o avistan.o adraw.o \
		  avidfill.o opengl.o

VISU_PATH	= visu/c

VISU_PATH_OBJS = $(addprefix visu/,$(VISU_OBJS))

VISU_BIN	= cplay u2a u2e

#OBJS += $(VISU_PATH_OBJS)

default-visu:
	$(MAKE) -C ../.. visu-all

visu-all: $(addprefix $(VISU_PATH)/,$(VISU_BIN))

$(VISU_PATH)/u2a: $(VISU_PATH)/u2a.o $(VISU_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL

$(VISU_PATH)/u2e: $(VISU_PATH)/u2e.o $(VISU_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL

$(VISU_PATH)/cplay: $(VISU_PATH)/cplay.o $(VISU_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $+ $(LIBS) -lSOIL
