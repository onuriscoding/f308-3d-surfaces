ifndef OF_ROOT
    OF_ROOT=$(HOME)/Documents/INFO/ULB_Projects/BA3/PROJ3/of_v0.12.1_linux64_gcc6_release
endif

APPNAME = f308-3d-surfaces

PROJECT_ROOT = $(CURDIR)

include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk
