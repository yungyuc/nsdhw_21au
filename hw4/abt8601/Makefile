DEFAULT_PROFILE = RELEASE
MOD_NAME = _matrix

# Flags

CPPFLAGS_DEFAULT = -DNDEBUG

CXXFLAGS_BASE    = -std=c++11 -pedantic -Wall -Wextra -Werror $(shell python3-config --includes) -I/usr/include/mkl
CXXFLAGS_DEBUG   = -g -fsanitize=address -fsanitize=undefined
CXXFLAGS_DEFAULT = -O3 -funroll-loops -march=native

LDLIBS_BASE  = -lmkl_rt
LDLIBS_DEBUG = -lasan -lubsan

# ------------------------------------------------------------------------------

PROFILE = $(DEFAULT_PROFILE)

CPPFLAGS = $(CPPFLAGS_BASE) $(CPPFLAGS_$(PROFILE))
CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_$(PROFILE))
LDLIBS   = $(LDLIBS_BASE) $(LDLIBS_$(PROFILE))

MOD = $(MOD_NAME)$(shell python3-config --extension-suffix)
BUILD_DIR = build/$(PROFILE)

.PHONY: all test bench clean-mod clean-profile clean

all: $(MOD)

$(MOD): $(BUILD_DIR)/$(MOD)
	cp $< $@

$(BUILD_DIR)/$(MOD): $(BUILD_DIR)/matrix_pybind.o $(BUILD_DIR)/matrix_mul.o $(BUILD_DIR)/allocator.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -shared -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -fPIC -MMD -MF $(BUILD_DIR)/$*.d -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(MOD)
	python3 -m pytest

bench: $(MOD)
	./benchmark.sh

clean-mod:
	$(RM) $(MOD)

clean-profile:
	$(RM) -r $(BUILD_DIR) $(MOD)

clean:
	$(RM) -r build $(MOD)

-include $(BUILD_DIR)/*.d
