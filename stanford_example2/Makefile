# Add .d to Make's recognized suffixes.
.SUFFIXES:
.SUFFIXES: .cpp .o .d

#We don't need to clean up when we're making these targets
NODEPS:=clean cleaner ctags

CXX=g++-14
CXX_VERSION=c++23
OPTIMIZE_FLAG=-O3
DEBUG_FLAG=-g
WARNING_FLAGS=-Wall -Wextra -Wconversion
CXXFLAGS=-std=$(CXX_VERSION) $(OPTIMIZE_FLAG) $(WARNING_FLAGS) $(DEBUG_FLAG)
INCLUDE_DIR=/usr/local/include
LIB_DIR=/usr/local/lib
BINDIR := bin
OBJDIR := obj
SRCDIR := src
RM=rm
DEPDIR := $(OBJDIR)/.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td


# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard *.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

LIBRARIES=

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
# In case the compile is interupted, we use temp .d files for the dependencies.
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

$(OBJDIR)/%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(BINDIR): ; @mkdir -p $@

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)
$(DEPFILES):
include $(wildcard $(DEPFILES))

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

# The build target
TARGET = $(BINDIR)/test_program.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ -L$(LIB_DIR) $(LIBRARIES) $^

clean:
	$(RM) -f $(OBJDIR)/*.o $(TARGET)

cleaner: clean
	$(RM) -f $(DEPDIR)/*.d tags

ctags:
	ctags --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ *.cpp

.DELETE_ON_ERROR:
.PHONY: all clean
