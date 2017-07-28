CC := $(CXX)
RM ?= rm -f

CXXFLAGS = -Wall -Wextra -Wfatal-errors -std=c++17 -m64 -march=native
CXXFLAGS_RELEASE = -O3 -funroll-loops
CXXFLAGS_DEBUG = -g -Og
LFLAGS = -static -lws2_32
LFLAGS_RELEASE = -flto -s
LFLAGS_DEBUG =

DEFINES = -DRSIZE_T_DEFINED

CXXFLAGS += $(DEFINES) -I$(SRCDIR)

all:
	@$(CC) $(CXXFLAGS) $(CXXFLAGS_RELEASE) $(LFLAGS) $(LFLAGS_RELEASE) -c lib/Remotery.c -o lib/Remotery.o
	@ar cr lib/libRemotery.a lib/Remotery.o
	@$(RM) lib/Remotery.o
	@echo "  lib/libRemotery.a  done"

	@$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(LFLAGS) $(LFLAGS_DEBUG) -c lib/Remotery.c -o lib/Remoteryd.o
	@ar cr lib/libRemoteryd.a lib/Remoteryd.o
	@$(RM) lib/Remoteryd.o
	@echo "  lib/libRemoteryd.a  done"

.PHONY: all