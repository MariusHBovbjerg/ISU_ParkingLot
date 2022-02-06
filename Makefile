SOURCES=updatedparkinglot.cpp MessageQueue.cpp handlers.cpp CarMessage.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=$(SOURCES:.cpp=.d)
EXE=prog
CXXFLAGS=-I. -g
	
ifeq (${ARCH},host)
CXX=g++
BUILD_DIR=build/host
BIN_DIR=bin/host

endif

ifeq (${ARCH},target)
CXX=arm-rpizw-g++
BUILD_DIR=build/target
BIN_DIR=bin/target
endif

$(BIN_DIR)/$(EXE): $(addprefix $(BUILD_DIR)/,$(OBJECTS))
	mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ -pthread

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(BUILD_DIR)/%.d: %.cpp
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< >$@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >>$@

format: $(SOURCES:.cpp=.format)
%.format: %.cpp
	@echo "Formatting file '$<' " ...
	@clang-format -i $<

tidy: $(SOURCES:.cpp=.tidy)
%.tidy: %.cpp
	@echo "Tidyingfile'$<' "...
	@clang-tidy  $< -- $(CXXFLAGS)

.PHONY: clean

clean: 
	rm -rf build bin

ifneq ($(filter-out clean format tidy,$(MAKECMDGOALS)),)
-include $(DEPS)
endif