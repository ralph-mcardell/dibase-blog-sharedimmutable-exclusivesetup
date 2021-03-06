# blog/sies application executables makefile
# Copyright (c) Dibase Limited 2013
# Author: Ralph E. McArdell

# Build configuration (debug | release), default to debug:
BUILD_CONFIG = release
ROOT_DIR = $(realpath $(CURDIR)/../..)

include $(ROOT_DIR)/makeinclude.mak

# Files and directories
SRC_FILES = racecheck.cpp

OBJ_FILES = $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)

OBJ_FILENAMES = $(SRC_FILES:%.cpp=%.o)

DEP_FILES = $(SRC_FILES:%.cpp=%.d)

# Executable target files have the name of the example source file less the 
# extension but might includ a build configuration specific suffix.
# Both lists of paths are required for the clean target:
TGT_FILES_RELEASE = $(SRC_FILES:%.cpp=$(EXEC_DIR)/%$(RELEASE_FILE_SUFFIX))
TGT_FILES_DEBUG = $(SRC_FILES:%.cpp=$(EXEC_DIR)/%$(DEBUG_FILE_SUFFIX))

# But only one list of target paths is used when building:
ifeq ($(BUILD_CONFIG),release)
	TGT_FILES = $(TGT_FILES_RELEASE)
else ifeq ($(BUILD_CONFIG),debug)
	TGT_FILES = $(TGT_FILES_DEBUG)
endif

.PHONY: lib all clean tidy

all: lib $(DEP_FILES) $(TGT_FILES)

tidy: 
	-$(RM) $(OBJ_FILENAMES:%=$(BUILD_DIR)/release/%)
	-$(RM) $(OBJ_FILENAMES:%=$(BUILD_DIR)/debug/%)

clean: tidy
	-$(RM) $(TGT_FILES_RELEASE)
	-$(RM) $(TGT_FILES_DEBUG)
	-$(RM) $(DEP_FILES)
	-$(RM) *.d.*
	-$(RM) *~

lib:
	$(MAKE) -C $(SRC_DIR) BUILD_CONFIG=$(BUILD_CONFIG) 

$(EXEC_DIR)/%$(FILE_SUFFIX): $(OBJ_DIR)/%.o $(LIB_DIR)/$(LIB_FILE)
	$(LD) -o $@ $(LD_ARGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(COMPILE_FLAGS) -I./.. -o $@ $< -pthread 

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(COMPILE_FLAGS) -MT'$$(OBJ_DIR)/$(<:%.cpp=%.o)' \
	-I./.. $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# $(CC) -MM $(COMPILE_FLAGS) -MT'$$(OBJ_DIR)/$(<:%.cpp=%.o)' \
#	-I./.. -I$(EXT_PACKAGES_DIR)/Catch/single_include $< > $@.$$$$; \

include $(DEP_FILES)

