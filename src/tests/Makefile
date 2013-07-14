# blog/sies test executables makefile
# Copyright (c) Dibase Limited 2013
# Author: Ralph E. McArdell

# Build configuration (debug | release), default to debug:
BUILD_CONFIG = debug
ROOT_DIR = $(realpath $(CURDIR)/../..)

include $(ROOT_DIR)/makeinclude.mak

# Files and directories
SRC_FILES = test_main.cpp \
            text_info-unittests.cpp\
            call_context_validator-unittests.cpp\
            atomic-policies-unittests.cpp\
            text_registry-unittests.cpp\
            random_in_range-unittests.cpp\
            rnd_text_info_maker-unittests.cpp\
            logger-unittests.cpp\
            task-unittests.cpp

OBJ_FILES = $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)
OBJ_FILENAMES = $(SRC_FILES:%.cpp=%.o)

DEP_FILES = $(SRC_FILES:%.cpp=%.d)

TGT_FILE_RELEASE = $(TEST_DIR)/unittests-$(LIB_NAME_BASE)$(RELEASE_FILE_SUFFIX)

TGT_FILE_DEBUG = $(TEST_DIR)/unittests-$(LIB_NAME_BASE)$(DEBUG_FILE_SUFFIX)

# But only one list of target paths is used when building:
ifeq ($(BUILD_CONFIG),release)
	TGT_FILE = $(TGT_FILE_RELEASE)
else ifeq ($(BUILD_CONFIG),debug)
	TGT_FILE = $(TGT_FILE_DEBUG)
endif

.PHONY: lib all clean tidy

all: lib $(DEP_FILES) $(TGT_FILE)

tidy: 
	-$(RM) $(OBJ_FILENAMES:%=$(BUILD_DIR)/release/%)
	-$(RM) $(OBJ_FILENAMES:%=$(BUILD_DIR)/debug/%)

clean: tidy
	-$(RM) $(TGT_FILE_RELEASE)
	-$(RM) $(TGT_FILE_DEBUG)
	-$(RM) $(DEP_FILES)
	-$(RM) *.d.*
	-$(RM) *~

lib:
	$(MAKE) -C $(SRC_DIR) BUILD_CONFIG=$(BUILD_CONFIG) 

$(TGT_FILE): $(OBJ_FILES)
	$(LD) -o $@ $(LD_ARGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(COMPILE_FLAGS) -I./.. -I$(EXT_PACKAGES_DIR)/Catch/single_include -o $@ $<

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(COMPILE_FLAGS) -MT'$$(OBJ_DIR)/$(<:%.cpp=%.o)' \
	-I./.. -I$(EXT_PACKAGES_DIR)/Catch/single_include $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(DEP_FILES)

