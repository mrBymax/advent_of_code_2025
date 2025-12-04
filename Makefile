# Compiler Settings
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O3 -g
LDFLAGS :=

# Directories
BUILD_DIR := ./build
SRC_DIRS := ./src

# 1. Source Discovery
# -------------------
ALL_SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
DAY_SRCS := $(shell find $(SRC_DIRS) -name 'day_*.cpp')
COMMON_SRCS := $(filter-out $(DAY_SRCS), $(ALL_SRCS))

# 2. Object Definitions
# ---------------------
# Map src/%.cpp -> build/src/%.o
COMMON_OBJS := $(COMMON_SRCS:%.cpp=$(BUILD_DIR)/%.o)
DAY_OBJS := $(DAY_SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 3. Executable Definitions (The Fix)
# -----------------------------------
# We take the Object paths (build/src/day_01.o) and remove the .o suffix.
# Result: build/src/day_01
DAY_EXECS := $(DAY_OBJS:.o=)

# 4. Dependency Management
# ------------------------
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# 5. Targets
# ----------

.PHONY: all
all: $(DAY_EXECS)

# Link Step: Create the executable 'day_x' from 'day_x.o' + common objects
$(DAY_EXECS): % : %.o $(COMMON_OBJS)
	@echo "Linking $@"
	$(CXX) $< $(COMMON_OBJS) -o $@ $(LDFLAGS)

# Compile Step: Create 'day_x.o' from 'day_x.cpp'
# The pattern %.o matches the dependency %.cpp provided we set the path correctly
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Convenience: Allow typing 'make day_01' to build 'build/src/day_01'
DAY_NAMES := $(notdir $(DAY_EXECS))

.PHONY: $(DAY_NAMES)
$(DAY_NAMES): %:
	@# Find the specific executable that ends with the requested name
	@$(MAKE) $(filter %/$@, $(DAY_EXECS))

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include dependencies
-include $(DAY_OBJS:.o=.d)
-include $(COMMON_OBJS:.o=.d)
