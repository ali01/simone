DIR_OUEHW19P := simone/time
SUB_OUEHW19P := 

SRCS += $(wildcard $(DIR_OUEHW19P)/*.$(EXT))

MK_OUEHW19P := $(foreach sdir,$(SUB_OUEHW19P),$(wildcard $(DIR_OUEHW19P)/$(sdir)/*.make))
ifneq ($(MK_OUEHW19P),)
$(info -- including subdirectory makefiles ($(DIR_OUEHW19P)): $(MK_OUEHW19P)) 
include $(MK_OUEHW19P)
endif
