WORKDIR = %cd%

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

INC = 
CFLAGS = -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-default -Weffc++ -Wzero-as-null-pointer-constant -Wmain -pedantic-errors -pedantic -Wextra -Wall -std=c++14 -m32
RESINC = 
LIBDIR = 
LIB = -lopengl32 -lglu32 -lgdi32 -lfreeglut
LDFLAGS = -static-libstdc++ -static -m32

INC_DEBUG = $(INC) -Iinclude
CFLAGS_DEBUG = $(CFLAGS) -Weffc++ -Wall -g -DLOGGING_ENABLED
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) -static-libgcc -ggdb
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/stl_viewer.exe

INC_RELEASE = $(INC) -Iinclude
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/stl_viewer.exe

INC_DEBUG_PROFILE = $(INC) -Iinclude
CFLAGS_DEBUG_PROFILE = $(CFLAGS) -pg -g
RESINC_DEBUG_PROFILE = $(RESINC)
RCFLAGS_DEBUG_PROFILE = $(RCFLAGS)
LIBDIR_DEBUG_PROFILE = $(LIBDIR)
LIB_DEBUG_PROFILE = $(LIB)
LDFLAGS_DEBUG_PROFILE = $(LDFLAGS) -pg -lgmon
OBJDIR_DEBUG_PROFILE = obj/DebugProfile
DEP_DEBUG_PROFILE = 
OUT_DEBUG_PROFILE = bin/DebugProfile/stl_viewer.exe

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/main.o $(OBJDIR_DEBUG)/src/CVector3d.o $(OBJDIR_DEBUG)/src/CTriangle.o $(OBJDIR_DEBUG)/src/CTextOutput.o $(OBJDIR_DEBUG)/src/CStlLoader.o $(OBJDIR_DEBUG)/src/CRenderer.o $(OBJDIR_DEBUG)/src/CQuaternion.o $(OBJDIR_DEBUG)/src/CModel.o $(OBJDIR_DEBUG)/src/CLogger.o $(OBJDIR_DEBUG)/src/CFpsCounter.o $(OBJDIR_DEBUG)/src/CApp.o $(OBJDIR_DEBUG)/src/C3DFacet.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/main.o $(OBJDIR_RELEASE)/src/CVector3d.o $(OBJDIR_RELEASE)/src/CTriangle.o $(OBJDIR_RELEASE)/src/CTextOutput.o $(OBJDIR_RELEASE)/src/CStlLoader.o $(OBJDIR_RELEASE)/src/CRenderer.o $(OBJDIR_RELEASE)/src/CQuaternion.o $(OBJDIR_RELEASE)/src/CModel.o $(OBJDIR_RELEASE)/src/CLogger.o $(OBJDIR_RELEASE)/src/CFpsCounter.o $(OBJDIR_RELEASE)/src/CApp.o $(OBJDIR_RELEASE)/src/C3DFacet.o

OBJ_DEBUG_PROFILE = $(OBJDIR_DEBUG_PROFILE)/src/main.o $(OBJDIR_DEBUG_PROFILE)/src/CVector3d.o $(OBJDIR_DEBUG_PROFILE)/src/CTriangle.o $(OBJDIR_DEBUG_PROFILE)/src/CTextOutput.o $(OBJDIR_DEBUG_PROFILE)/src/CStlLoader.o $(OBJDIR_DEBUG_PROFILE)/src/CRenderer.o $(OBJDIR_DEBUG_PROFILE)/src/CQuaternion.o $(OBJDIR_DEBUG_PROFILE)/src/CModel.o $(OBJDIR_DEBUG_PROFILE)/src/CLogger.o $(OBJDIR_DEBUG_PROFILE)/src/CFpsCounter.o $(OBJDIR_DEBUG_PROFILE)/src/CApp.o $(OBJDIR_DEBUG_PROFILE)/src/C3DFacet.o

all: before_build build_debug build_release build_debug_profile after_build

clean: clean_debug clean_release clean_debug_profile

before_build: 

after_build: 

before_debug: 
	mkdir -p bin/Debug
	mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 

build_debug: before_debug out_debug after_debug

debug: before_build build_debug after_build

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/main.cpp -o $(OBJDIR_DEBUG)/src/main.o

$(OBJDIR_DEBUG)/src/CVector3d.o: src/CVector3d.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CVector3d.cpp -o $(OBJDIR_DEBUG)/src/CVector3d.o

$(OBJDIR_DEBUG)/src/CTriangle.o: src/CTriangle.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CTriangle.cpp -o $(OBJDIR_DEBUG)/src/CTriangle.o

$(OBJDIR_DEBUG)/src/CTextOutput.o: src/CTextOutput.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CTextOutput.cpp -o $(OBJDIR_DEBUG)/src/CTextOutput.o

$(OBJDIR_DEBUG)/src/CStlLoader.o: src/CStlLoader.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CStlLoader.cpp -o $(OBJDIR_DEBUG)/src/CStlLoader.o

$(OBJDIR_DEBUG)/src/CRenderer.o: src/CRenderer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CRenderer.cpp -o $(OBJDIR_DEBUG)/src/CRenderer.o

$(OBJDIR_DEBUG)/src/CQuaternion.o: src/CQuaternion.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CQuaternion.cpp -o $(OBJDIR_DEBUG)/src/CQuaternion.o

$(OBJDIR_DEBUG)/src/CModel.o: src/CModel.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CModel.cpp -o $(OBJDIR_DEBUG)/src/CModel.o

$(OBJDIR_DEBUG)/src/CLogger.o: src/CLogger.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CLogger.cpp -o $(OBJDIR_DEBUG)/src/CLogger.o

$(OBJDIR_DEBUG)/src/CFpsCounter.o: src/CFpsCounter.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CFpsCounter.cpp -o $(OBJDIR_DEBUG)/src/CFpsCounter.o

$(OBJDIR_DEBUG)/src/CApp.o: src/CApp.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CApp.cpp -o $(OBJDIR_DEBUG)/src/CApp.o

$(OBJDIR_DEBUG)/src/C3DFacet.o: src/C3DFacet.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/C3DFacet.cpp -o $(OBJDIR_DEBUG)/src/C3DFacet.o

clean_debug: 
	rm --force $(OBJ_DEBUG) $(OUT_DEBUG)
	rmdir bin/Debug
	rmdir $(OBJDIR_DEBUG)/src

before_release: 
	mkdir -p bin/Release
	mkdir -p $(OBJDIR_RELEASE)/src

after_release: 

build_release: before_release out_release after_release

release: before_build build_release after_build

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) -mwindows $(LIB_RELEASE)

$(OBJDIR_RELEASE)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/main.cpp -o $(OBJDIR_RELEASE)/src/main.o

$(OBJDIR_RELEASE)/src/CVector3d.o: src/CVector3d.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CVector3d.cpp -o $(OBJDIR_RELEASE)/src/CVector3d.o

$(OBJDIR_RELEASE)/src/CTriangle.o: src/CTriangle.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CTriangle.cpp -o $(OBJDIR_RELEASE)/src/CTriangle.o

$(OBJDIR_RELEASE)/src/CTextOutput.o: src/CTextOutput.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CTextOutput.cpp -o $(OBJDIR_RELEASE)/src/CTextOutput.o

$(OBJDIR_RELEASE)/src/CStlLoader.o: src/CStlLoader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CStlLoader.cpp -o $(OBJDIR_RELEASE)/src/CStlLoader.o

$(OBJDIR_RELEASE)/src/CRenderer.o: src/CRenderer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CRenderer.cpp -o $(OBJDIR_RELEASE)/src/CRenderer.o

$(OBJDIR_RELEASE)/src/CQuaternion.o: src/CQuaternion.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CQuaternion.cpp -o $(OBJDIR_RELEASE)/src/CQuaternion.o

$(OBJDIR_RELEASE)/src/CModel.o: src/CModel.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CModel.cpp -o $(OBJDIR_RELEASE)/src/CModel.o

$(OBJDIR_RELEASE)/src/CLogger.o: src/CLogger.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CLogger.cpp -o $(OBJDIR_RELEASE)/src/CLogger.o

$(OBJDIR_RELEASE)/src/CFpsCounter.o: src/CFpsCounter.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CFpsCounter.cpp -o $(OBJDIR_RELEASE)/src/CFpsCounter.o

$(OBJDIR_RELEASE)/src/CApp.o: src/CApp.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CApp.cpp -o $(OBJDIR_RELEASE)/src/CApp.o

$(OBJDIR_RELEASE)/src/C3DFacet.o: src/C3DFacet.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/C3DFacet.cpp -o $(OBJDIR_RELEASE)/src/C3DFacet.o

clean_release: 
	rm --force $(OBJ_RELEASE) $(OUT_RELEASE)
	rmdir bin/Release
	rmdir $(OBJDIR_RELEASE)/src

before_debug_profile: 
	mkdir -p bin/DebugProfile
	mkdir -p $(OBJDIR_DEBUG_PROFILE)/src

after_debug_profile: 

build_debug_profile: before_debug_profile out_debug_profile after_debug_profile

debug_profile: before_build build_debug_profile after_build

out_debug_profile: before_debug_profile $(OBJ_DEBUG_PROFILE) $(DEP_DEBUG_PROFILE)
	$(LD) $(LIBDIR_DEBUG_PROFILE) -o $(OUT_DEBUG_PROFILE) $(OBJ_DEBUG_PROFILE)  $(LDFLAGS_DEBUG_PROFILE) $(LIB_DEBUG_PROFILE)

$(OBJDIR_DEBUG_PROFILE)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/main.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/main.o

$(OBJDIR_DEBUG_PROFILE)/src/CVector3d.o: src/CVector3d.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CVector3d.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CVector3d.o

$(OBJDIR_DEBUG_PROFILE)/src/CTriangle.o: src/CTriangle.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CTriangle.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CTriangle.o

$(OBJDIR_DEBUG_PROFILE)/src/CTextOutput.o: src/CTextOutput.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CTextOutput.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CTextOutput.o

$(OBJDIR_DEBUG_PROFILE)/src/CStlLoader.o: src/CStlLoader.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CStlLoader.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CStlLoader.o

$(OBJDIR_DEBUG_PROFILE)/src/CRenderer.o: src/CRenderer.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CRenderer.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CRenderer.o

$(OBJDIR_DEBUG_PROFILE)/src/CQuaternion.o: src/CQuaternion.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CQuaternion.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CQuaternion.o

$(OBJDIR_DEBUG_PROFILE)/src/CModel.o: src/CModel.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CModel.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CModel.o

$(OBJDIR_DEBUG_PROFILE)/src/CLogger.o: src/CLogger.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CLogger.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CLogger.o

$(OBJDIR_DEBUG_PROFILE)/src/CFpsCounter.o: src/CFpsCounter.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CFpsCounter.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CFpsCounter.o

$(OBJDIR_DEBUG_PROFILE)/src/CApp.o: src/CApp.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/CApp.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/CApp.o

$(OBJDIR_DEBUG_PROFILE)/src/C3DFacet.o: src/C3DFacet.cpp
	$(CXX) $(CFLAGS_DEBUG_PROFILE) $(INC_DEBUG_PROFILE) -c src/C3DFacet.cpp -o $(OBJDIR_DEBUG_PROFILE)/src/C3DFacet.o

clean_debug_profile: 
	rm --force $(OBJ_DEBUG_PROFILE) $(OUT_DEBUG_PROFILE)
	rmdir bin/DebugProfile
	rmdir $(OBJDIR_DEBUG_PROFILE)/src

.PHONY: before_build after_build before_debug after_debug clean_debug before_release after_release clean_release before_debug_profile after_debug_profile clean_debug_profile

