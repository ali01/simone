DIR_5YE1FAYJ := simone/network
SUB_5YE1FAYJ := 

SRCS += $(wildcard $(DIR_5YE1FAYJ)/*.$(EXT))

MK_5YE1FAYJ := $(foreach sdir,$(SUB_5YE1FAYJ),$(wildcard $(DIR_5YE1FAYJ)/$(sdir)/*.make))
ifneq ($(MK_5YE1FAYJ),)
$(info -- including subdirectory makefiles ($(DIR_5YE1FAYJ)): $(MK_5YE1FAYJ)) 
include $(MK_5YE1FAYJ)
endif
