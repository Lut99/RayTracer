# MAKEFILE for the RAYTRACER PROJECT
#   by Lut99
#
# This is the improved, updated version of the original Makefile



### VARS ###

# Define some constants
GXX = g++
GXX_ARGS = -std=c++17 -O2 -Wall -Wextra

BIN = bin
OBJ = $(BIN)/obj
OBJ_SHARED = $(BIN)/obj_shared

SRC = src
LIB = $(SRC)/lib

SCENES_SRC = $(LIB)/scenes
SCENES_BIN = scenes/init
SCENES_OBJ = $(OBJ_SHARED)/scenes

LIBRARIES = $(OBJ)/Ray.o $(OBJ)/Frames.o $(OBJ)/Image.o $(OBJ)/LodePNG.o $(OBJ)/Vec3.o $(OBJ)/RenderObject.o $(OBJ)/objects/Sphere.o \
			$(OBJ)/objects/RenderObjectCollection.o $(OBJ)/Random.o $(OBJ)/ProgressBar.o $(OBJ)/Camera.o $(OBJ)/RenderWorld.o $(OBJ)/Material.o \
			$(OBJ)/materials/Lambertian.o $(OBJ)/materials/Metal.o $(OBJ)/materials/Dielectric.o $(OBJ)/RenderAnimation.o $(OBJ)/CameraMovement.o \
			$(OBJ)/animations/camera/CameraRotation.o $(OBJ)/StringError.o $(OBJ)/BoundingBox.o $(OBJ)/ObjectTree.o
SOURCES = $(shell find $(LIB) -name '*.cpp')
EXT_LIBS=-ldl

# Check for additional compile options
ifdef THREADED
GXX_ARGS += -D THREADED
LIBRARIES += $(OBJ)/ThreadPool.o
EXT_LIBS += -lpthread
endif

ifdef DEBUG
GXX_ARGS += -g
endif

# Create shared libraries list and includes list
LIBRARIES_SHARED = $(patsubst $(OBJ)/%.o,$(OBJ_SHARED)/%.o,$(LIBRARIES))
GXX_ARGS += -I $(SRC)/lib/include/ -I $(SRC)/lib/include/objects/ -I $(SRC)/lib/include/materials/ -I $(SRC)/lib/include/animations/camera



### COMPILE RULES ###

.PHONY: default all clean raytracer
default: raytracer

# Folder make rules
$(BIN):
	mkdir -p $@
$(OBJ): $(BIN)
	mkdir -p $@
$(OBJ_SHARED): $(OBJ)
	mkdir -p $@
dirs: $(OBJ_SHARED)

# Obj rules
$(OBJ_SHARED)/%.o: $(LIB)/%.cpp
	mkdir -p "$(@D)"
	$(GXX) $(GXX_ARGS) -fPIC -o $@ -c $<
$(OBJ)/%.o: $(LIB)/%.cpp
	mkdir -p "$(@D)"
	$(GXX) $(GXX_ARGS) -o $@ -c $<

# Special rules
$(OBJ)/RayTracer.o: $(SRC)/RayTracer.cpp
	mkdir -p "$(@D)"
	$(GXX) $(GXX_ARGS) -o $@ -c $<

# Scenes rules
$(SCENES_OBJ)/%.o: $(SCENES_SRC)/%.cpp | $(SCENES_OBJ)
	mkdir -p "$(@D)"
	$(GXX) $(GXX_ARGS) -fPIC -o $@ -c $<
$(SCENES_BIN)/%.so: $(SCENES_OBJ)/%.o | $(SCENES_BIN)
	mkdir -p "$(@D)"
	$(GXX) $(GXX_ARGS) -shared -o $@ $<

# Main rules
raytracer: $(OBJ)/RayTracer.o $(OBJ)/Renderer.o $(LIBRARIES)
	$(GXX) $(GXX_ARGS) -rdynamic -o $(BIN)/raytracer.out $(OBJ)/RayTracer.o $(OBJ)/Renderer.o $(LIBRARIES) $(EXT_LIBS)

scenes: $(SCENES_BIN)/random.so

all: raytracer

clean:
	rm -f $(BIN)/*.out
	-find $(OBJ) "*.o" -type f -delete
	-find $(OBJ_SHARED) "*.o" -type f -delete

	rm -f $(SCENES_BIN)/*.so
	rm -f $(SCENES_OBJ)/*.o
