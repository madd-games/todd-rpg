SRC := $(wildcard $(SRCDIR)/src/*.cpp)
CPP_FILES := $(notdir $(SRC))
DEP := $(CPP_FILES:.cpp=.d)
OBJ := $(CPP_FILES:.cpp=.o) $(EXTRA_OBJ)
CFLAGS := $(CFLAGS) -I $(SRCDIR)/skills -I $(SRCDIR)/se

.PHONY: install clean distclean

todd$(EXE): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

-include $(DEP)

winrc/todd.o: $(SRCDIR)/winrc/todd.rc $(SRCDIR)/winrc/todd.ico
	$(WINDRES) $< -o $@

%.d: $(SRCDIR)/src/%.cpp
	set -e; rm -f $@; \
	$(CXX) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: $(SRCDIR)/src/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

install: todd$(EXE)
	sh $(SRCDIR)/install.sh $(SRCDIR) $(PREFIX) $(EXEC_PREFIX) todd$(EXE)

clean:
	rm -f todd $(wildcard *.o) $(wildcard *.d)

distclean: clean
	rm -f config.mk
