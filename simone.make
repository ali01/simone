DIR_73K3LKB5 := simone
SUB_73K3LKB5 := network time

SRCS += $(wildcard $(DIR_73K3LKB5)/*.$(EXT))

MK_73K3LKB5 := $(foreach sdir,$(SUB_73K3LKB5),$(wildcard $(DIR_73K3LKB5)/$(sdir)/*.make))
ifneq ($(MK_73K3LKB5),)
$(info -- including subdirectory makefiles ($(DIR_73K3LKB5)): $(MK_73K3LKB5)) 
include $(MK_73K3LKB5)
endif
