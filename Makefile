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
OBJ_SHARED = $(OBJ)/shared

SRC = src
LIB = $(SRC)/lib

LIBRARIES = $(OBJ)/Ray.o $(OBJ)/Frames.o $(OBJ)/Image.o $(OBJ)/LodePNG.o $(OBJ)/Vec3.o $(OBJ)/RenderObject.o $(OBJ)/Sphere.o \
			$(OBJ)/RenderObjectCollection.o $(OBJ)/Random.o $(OBJ)/ProgressBar.o $(OBJ)/Camera.o $(OBJ)/RenderWorld.o $(OBJ)/Material.o \
			$(OBJ)/Lambertian.o $(OBJ)/Metal.o $(OBJ)/Dielectric.o $(OBJ)/RenderAnimation.o $(OBJ)/CameraMovement.o $(OBJ)/CameraRotation.o \
			$(OBJ)/StringError.o $(OBJ)/BoundingBox.o $(OBJ)/ObjectTree.o
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
LIBRARIES_SHARED = $(pathsubst $(OBJ),$(OBJ_SHARED),$(LIBRARIES))
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
$(OBJ)/%.o: $(wildcard $(LIB)/*%.cpp) | $(OBJ)
	$(GXX) $(GXX_ARGS) -o $@ -c $<
$(OBJ_SHARED)/%.o: $(wildcard $(LIB)/*%.cpp) | $(OBJ_SHARED)
	$(GXX) $(GXX_ARGS) -fPIC -o $@ -c $<

# Special rules
$(OBJ)/RayTracer.o: $(SRC)/RayTracer.cpp | $(OBJ)
	$(GXX) $(GXX_ARGS) -o $@ -c $<

# Main rules
raytracer: $(OBJ)/RayTracer.o $(OBJ)/Renderer.o $(LIBRARIES)
	$(GXX) $(GXX_ARGS) -o $(BIN)/raytracer.out $(OBJ)/RayTracer.o $(OBJ)/Renderer.o $(LIBRARIES) $(EXT_LIBS)

all: raytracer

test:
	$(info $(wildcard $(LIB)/*.cpp))

clean:
	rm -f $(BIN)/*.out
	rm -f $(OBJ)/*.o
	rm -f $(OBJ_SHARED)/*.o
