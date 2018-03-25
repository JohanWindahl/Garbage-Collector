# Flags
CC        :=  gcc
DEBUG     :=  -ggdb
WARNINGS  :=  -Wall -Wextra
TEST	  :=  -lcunit

CFLAGS    += $(DEBUG) $(WARNINGS)

# Directories
SRCDIR   :=  src
OBJDIR   :=  obj
BINDIR   :=  bin
TESTDIR  :=  tests



# File list (NO EXTENSION)
_FILES    := gc

## Add paths and suffixes
FILES     := $(patsubst %,$(SRCDIR)/%,$(addsuffix .c, $(_FILES)))
OBJFILES  := $(patsubst %,$(OBJDIR)/%,$(addsuffix .o, $(_FILES)))

OUT	  := $(BINDIR)/libgc.a

# Text formatting
TEXT_RED     := $$(tput setaf 1)
TEXT_GREEN   := $$(tput setaf 2)
TEXT_YELLOW  := $$(tput setaf 3)
TEXT_BLUE    := $$(tput setaf 4)
TEXT_BOLD    := $$(tput bold)
TEXT_RESET   := $$(tput sgr0)


# Default
all: help

# Print (Debugging make macros)
print:
	@echo $(_FILES)
	@echo "$(FILES)"
	@echo "$(OBJFILES)"

# COMPILATION
$(OBJDIR): $(OBJFILES)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $(TEXT_BOLD)$@$(TEXT_RESET)"
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "$(TEXT_GREEN)OK$(TEXT_RESET)"

$(BINDIR): $(OBJFILES)
	@echo ""
	@echo "Packaging $(TEXT_BLUE)static$(TEXT_RESET) library $(TEXT_BOLD)$(OUT)$(TEXT_RESET)"
	@ar rcs $(OUT) $(OBJFILES)
	@echo "$(TEXT_GREEN)OK$(TEXT_RESET)"

# TEST
test:
	@echo "    $(TEXT_RED)(!) NOT IMPLEMENTED$(TEXT_RESET)"

# DOCUMENTATION
docs:
	@find doc -maxdepth 1 -not -name '*.md' -not -name '*conf' -not -name 'doc' | xargs rm -rf
	@doxygen doc/doxconf

# Cleaning rules
clean:
	@find bin -type f -not -name '*.md' | xargs rm -rf
	@find obj -type f -not -name '*.md' | xargs rm -rf
	@find doc -maxdepth 1 -not -name '*.md' -not -name '*conf' -not -name 'doc' | xargs rm -rf

# Help text 
help:
	@echo ""
	@echo "    Available targets are:\n"
	@echo "    $(TEXT_BOLD)help$(TEXT_RESET)"
	@echo "        Prints this help text."
	@echo ""
	@echo "    $(TEXT_BOLD)docs$(TEXT_RESET)"
	@echo "        Clear & re-make documentation in /doc"
	@echo ""
	@echo "    $(TEXT_BOLD)clean$(TEXT_RESET)"
	@echo "        Cleans all binaries."
	@echo ""
	@echo "    $(TEXT_BOLD)obj$(TEXT_RESET)"
	@echo "        Compiles all source files in /src to object files in /obj."
	@echo ""
	@echo "    $(TEXT_BOLD)bin$(TEXT_RESET)"
	@echo "        Compiles object files and builds static library."
	@echo ""
	@echo "    $(TEXT_BOLD)$(OBJDIR)/%.o$(TEXT_RESET)"
	@echo "        Compiles $(SRCDIR)/%.c to object file $(OBJDIR)/%.o"
	@echo ""
