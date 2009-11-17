ifeq ($(OSTYPE),Darwin)
SRCS += $(shell `pwd`)/clock_gettime_stub.cpp
endif
