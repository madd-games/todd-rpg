SRC := $(wildcard $(SRCDIR)/src/*.cpp)
CPP_FILES := $(notdir $(SRC))
DEP := $(CPP_FILES:.cpp=.d)
OBJ := $(CPP_FILES:.cpp=.o) $(EXTRA_OBJ)
CFLAGS := $(CFLAGS) -I $(SRCDIR)/skills -I $(SRCDIR)/se -I $(SRCDIR)/items -I $(SRCDIR)/atiles -I $(SRCDIR)/src -std=c++11

.PHONY: all maps install clean distclean

all: todd$(EXE) maps

todd$(EXE): $(OBJ)
	@echo ">Link into $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

maps:
	@python $(SRCDIR)/mkscenes.py $(SRCDIR)

-include $(DEP)

winrc/todd.o: $(SRCDIR)/winrc/todd.rc $(SRCDIR)/winrc/todd.ico
	@echo ">Compile $<"
	@$(WINDRES) $< -o $@

%.d: $(SRCDIR)/src/%.cpp
	@echo ">Parse $<"
	@set -e; rm -f $@; \
	$(CXX) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: $(SRCDIR)/src/%.cpp
	@echo ">Compile $<"
	@$(CXX) -c $< -o $@ $(CFLAGS)

install: todd$(EXE)
	@echo ">Install $<"
	@sh $(SRCDIR)/install.sh $(SRCDIR) $(PREFIX) $(EXEC_PREFIX) todd$(EXE)

clean:
	@echo ">Delete object and dependency files"
	@rm -f todd $(wildcard *.o) $(wildcard *.d)

distclean: clean
	@echo ">Delete configuration and output files"
	@rm -f config.mk todd$(EXE) Doxyfile Makefile

