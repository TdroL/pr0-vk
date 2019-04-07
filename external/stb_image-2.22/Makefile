CC := $(CXX)
RM ?= rm -f

CXXFLAGS = -Wall -Wextra -Wfatal-errors -std=c++17 -m64 -march=native -I ./include
CXXFLAGS_RELEASE = -O3 -funroll-loops
CXXFLAGS_DEBUG = -g -Og
LFLAGS = -static -lws2_32
LFLAGS_RELEASE = -s
LFLAGS_DEBUG =

CXXFLAGS += $(DEFINES) -I$(SRCDIR)

all:
	@$(CC) $(CXXFLAGS) $(CXXFLAGS_RELEASE) $(LFLAGS) $(LFLAGS_RELEASE) -c src/stb_image.c -o lib/stb_image.o
	@ar cr lib/libstb_image.a lib/stb_image.o
	@$(RM) lib/stb_image.o
	@echo "  lib/libstb_image.a  done"

	@$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(LFLAGS) $(LFLAGS_DEBUG) -c src/stb_image.c -o lib/stb_imaged.o
	@ar cr lib/libstb_imaged.a lib/stb_imaged.o
	@$(RM) lib/stb_imaged.o
	@echo "  lib/libstb_imaged.a  done"

.PHONY: all