
GLENZ_OBJS	= vec.o main.o zoomer.o math.o mathsin.o fc.o

GLENZ_PATH	= glenz

GLENZ_PATH_OBJS = $(addprefix $(GLENZ_PATH)/,$(GLENZ_OBJS))

GLENZ_BIN	= glenz

OBJS += $(GLENZ_PATH_OBJS)

default-glenz:
	$(MAKE) -C .. glenz-all

glenz-all: $(GLENZ_PATH)/$(GLENZ_BIN)

$(GLENZ_PATH)/$(GLENZ_BIN): $(GLENZ_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(GLENZ_PATH_OBJS) $(LIBS)
