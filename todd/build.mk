SRC := $(wildcard $(SRCDIR)/src/*.cpp)
CPP_FILES := $(notdir $(SRC))
DEP := $(CPP_FILES:.cpp=.d)
OBJ := $(CPP_FILES:.cpp=.o)

.PHONY: install clean distclean

todd$(EXE): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

-include $(DEP)

%.d: $(SRCDIR)/src/%.cpp
	set -e; rm -f $@; \
	$(CXX) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: $(SRCDIR)/src/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

install: todd
	sh $(SRCDIR)/install.sh $(SRCDIR) $(PREFIX) $(EXEC_PREFIX)

clean:
	rm -f todd $(wildcard *.o) $(wildcard *.d)

distclean: clean
	rm -f config.mk
