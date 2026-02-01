PROJECT_NAME = PerlinNoise


PROJECT_ROOT = $(realpath .)
OF_ROOT = ../../../../../

PROJECT_SRC = $(shell find src -name "*.cpp")


#PROJECT_SRC = src/PerlinNoise/ofApp.cpp src/PerlinNoise/main.cpp 

include $(OF_ROOT)libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

$(BIN_NAME): $(OBJS) $(TARGET_LIBS)
	@echo "Linking PerlinNoise application..."
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

.PHONY: run
run: all
	cd bin && ./$(BIN_NAME)
