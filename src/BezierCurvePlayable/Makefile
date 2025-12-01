PROJECT_NAME = BezierCurbePlayable

PROJECT_ROOT = $(realpath .)
OF_ROOT = /home/clover/Documents/INFO/of_v0.12.1_linux64_gcc6_release

PROJECT_SRC = src/BezierCurbePlayable/ofApp.cpp src/BezierCurbePlayable/main.cpp

include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

$(BIN_NAME): $(OBJS) $(TARGET_LIBS)
	@echo "Linking BezierCurbePlayable application..."
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

.PHONY: run
run: all
	cd bin && ./$(BIN_NAME)
