ifeq ($(OSTYPE),Darwin)
SRCS += simone/time/clock_gettime_stub.cpp
endif
