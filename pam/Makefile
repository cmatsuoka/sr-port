
PAM_OBJS	=

PAM_PATH	= pam

PAM_PATH_OBJS = $(addprefix $(PAM_PATH)/,$(PAM_OBJS))

PAM_BIN	= pam

OBJS += $(PAM_PATH_OBJS)

default-pam:
	$(MAKE) -C .. pam-all

pam-all: $(PAM_PATH)/$(PAM_BIN)

$(PAM_PATH)/$(PAM_BIN): $(PAM_PATH_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(PAM_PATH_OBJS) $(LIBS) -lSOIL

vfli: vfli.o
	gcc -o $@ $(LDFLAGS) vfli.o
