# make debug/release

ifeq ($(CC),cc)
    CC = clang
endif

LIBDIR := lib
SRCDIR := example
BINDIR := bin

CFLAGS ?= -Wall -Wextra -pedantic -std=c23
LDFLAGS ?= -L./$(LIBDIR)
LDLIBS ?= -lpcg_random
DEBUG_FLAGS = -DDEBUG -O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address
RELEASE_FLAGS = -DNDEBUG -O3
config ?= release #debug / release

ifeq ($(config), debug)
	CFLAGS += $(DEBUG_FLAGS)
else
	CFLAGS += $(RELEASE_FLAGS)
endif

SRCS := $(wildcard $(SRCDIR)/*.c)
BINS := $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%, $(SRCS))


all: $(BINDIR) $(BINS)

$(BINDIR):
	mkdir -p $(BINDIR)

# endian_test must compile with O0
$(BINDIR)/endian_test_example: CFLAGS = -Wall -Wextra -pedantic -std=c23 $(DEBUG_FLAGS)
$(BINDIR)/endian_test_example: $(SRCDIR)/endian_test_example.c
	$(CC) $< -o $@ $(CFLAGS)

# pcg_random_example must link with libpcg_random.a
$(BINDIR)/pcg_random_example: $(SRCDIR)/pcg_random_example.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

# everything else should compile just fine
$(BINDIR)/%: $(SRCDIR)/%.c
	$(CC) $< -o $@ $(CFLAGS)


.PHONY: all clean debug release
clean:
	rm -f $(BINS)

debug:
	@$(MAKE) config=debug

release:
	@$(MAKE) config=release
