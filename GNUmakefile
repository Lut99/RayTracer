CC=g++
ARGS = -std=c++17 -O2 -Wall -Wextra

NVCC=nvcc
NVCC_ARGS = -std=c++17

BIN_DIR = bin/linux
LIB_DIR = $(BIN_DIR)/lib
SRC_DIR = src
EXTENSION =out

OPTS =
EXT_LIBS =

# Check if we're on windows
ifeq ($(OS), Windows_NT)
BIN_DIR = bin/win
LIB_DIR = $(BIN_DIR)/lib
EXTENSION = exe
endif

# Check if we're on macos
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
BIN_DIR = bin/macos
LIB_DIR = $(BIN_DIR)/lib
OPTS += -D MACOS
endif

LIBRARIES = $(LIB_DIR)/Ray.o $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o $(LIB_DIR)/RenderObject.a $(LIB_DIR)/Random.o $(LIB_DIR)/ProgressBar.o $(LIB_DIR)/Camera.o $(LIB_DIR)/RenderWorld.o $(LIB_DIR)/Materials.a $(LIB_DIR)/scenes/RandomScene.o $(LIB_DIR)/Animations.a $(LIB_DIR)/StringError.o $(LIB_DIR)/BoundingBox.o $(LIB_DIR)/ObjectTree.o
INCLUDES = -I $(SRC_DIR)/lib/include/ -I $(SRC_DIR)/lib/include/objects/ -I $(SRC_DIR)/lib/include/materials/ -I $(SRC_DIR)/lib/include/animations/camera -I $(SRC_DIR)/lib/include/scenes/
OPTS += $(INCLUDES)

ifdef THREADED
OPTS += -D THREADED
LIBRARIES += $(LIB_DIR)/ThreadPool.o
EXT_LIBS += -lpthread
endif

ifdef DEBUG
OPTS += -g
endif

.PHONY: default
default: raytracer

# FOLDER MK RULES #
$(LIB_DIR):
	mkdir -p $@
$(LIB_DIR)/objects: $(LIB_DIR)
	mkdir -p $@
$(LIB_DIR)/materials: $(LIB_DIR)
	mkdir -p $@
$(LIB_DIR)/scenes: $(LIB_DIR)
	mkdir -p $@
$(LIB_DIR)/animations: $(LIB_DIR)
	mkdir -p $@
$(LIB_DIR)/animations/camera: $(LIB_DIR)/animations
	mkdir -p $@

# GENERAL OBJECT COMPILE RULES #
$(LIB_DIR)/%.o: $(SRC_DIR)/lib/%.cpp | $(LIB_DIR)
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/objects/%.o: $(SRC_DIR)/lib/objects/%.cpp | $(LIB_DIR)/objects
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/materials/%.o: $(SRC_DIR)/lib/materials/%.cpp | $(LIB_DIR)/materials
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/scenes/%.o: $(SRC_DIR)/lib/scenes/%.cpp | $(LIB_DIR)/scenes
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/animations/%.o: $(SRC_DIR)/lib/animations/%.cpp | $(LIB_DIR)/animations
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/animations/camera/%.o: $(SRC_DIR)/lib/animations/camera/%.cpp | $(LIB_DIR)/animations/camera
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<


# SPECIAL OBJECT COMPILE RULES #
$(LIB_DIR)/RayTracer.o: $(SRC_DIR)/RayTracer.cpp | $(LIB_DIR)
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<


# ARCHIVE RULES #
$(LIB_DIR)/Image.a: $(LIB_DIR)/Frames.o $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
	ar rvs $(LIB_DIR)/Image.a $(LIB_DIR)/Frames.o $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
$(LIB_DIR)/RenderObject.a: $(LIB_DIR)/RenderObject.o $(LIB_DIR)/objects/Sphere.o $(LIB_DIR)/objects/RenderObjectCollection.o
	ar rvs $(LIB_DIR)/RenderObject.a $(LIB_DIR)/RenderObject.o $(LIB_DIR)/objects/Sphere.o $(LIB_DIR)/objects/RenderObjectCollection.o
$(LIB_DIR)/Materials.a: $(LIB_DIR)/Material.o $(LIB_DIR)/materials/Lambertian.o $(LIB_DIR)/materials/Metal.o $(LIB_DIR)/materials/Dielectric.o
	ar rvs $(LIB_DIR)/Materials.a $(LIB_DIR)/Material.o $(LIB_DIR)/materials/Lambertian.o $(LIB_DIR)/materials/Metal.o $(LIB_DIR)/materials/Dielectric.o
$(LIB_DIR)/Animations.a: $(LIB_DIR)/RenderAnimation.o $(LIB_DIR)/CameraMovement.o $(LIB_DIR)/animations/camera/CameraRotation.o
	ar rvs $(LIB_DIR)/Animations.a $(LIB_DIR)/RenderAnimation.o $(LIB_DIR)/CameraMovement.o $(LIB_DIR)/animations/camera/CameraRotation.o


# MAIN COMPILATION #
raytracer: $(LIB_DIR)/RayTracer.o $(LIB_DIR)/Renderer.o $(LIBRARIES)
	$(CC) $(ARGS) $(OPTS) -o $(BIN_DIR)/raytracer.$(EXTENSION) $(LIB_DIR)/RayTracer.o $(LIB_DIR)/Renderer.o $(LIBRARIES) $(EXT_LIBS)
scene_creator: $(LIBRARIES) $(LIB_DIR)/Materials.a $(LIB_DIR)/RenderObject.a $(LIB_DIR)/Animations.a
	$(CC) $(ARGS) $(OPTS) -o $(BIN_DIR)/scene_creator.$(EXTENSION) $(SRC_DIR)/SceneCreator.cpp $(LIBRARIES) $(LIB_DIR)/Materials.a $(LIB_DIR)/RenderObject.a $(LIB_DIR)/Animations.a


# TEST COMPILE RULES #
test_image: $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
	$(CC) $(ARGS) -o tests/bin/test_image.$(EXTENSION) tests/src/test_image.cpp $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o

test_progressbar: $(LIB_DIR)/ProgressBar.o
	$(CC) $(ARGS) -o tests/bin/test_progressbar.$(EXTENSION) tests/src/test_progressbar.cpp $(LIB_DIR)/ProgressBar.o

test_json: $(LIB_DIR)/Vec3.o $(LIB_DIR)/WorldIO.o
	$(CC) $(ARGS) -o tests/bin/test_json.$(EXTENSION) tests/src/test_json.cpp $(LIB_DIR)/Vec3.o $(LIB_DIR)/WorldIO.o

test_frames: $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o
	$(CC) $(ARGS) -o tests/bin/test_frames.$(EXTENSION) tests/src/test_frames.cpp $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o

test_vector_math: $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o
	$(CC) $(ARGS) -o tests/bin/test_vector_math.$(EXTENSION) tests/src/test_vector_math.cpp $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o

clean:
	rm -f $(BIN_DIR)/*.out
	rm -f $(LIB_DIR)/*.o
	rm -f $(LIB_DIR)/materials/*.o
	rm -f $(LIB_DIR)/objects/*.o
	rm -f $(LIB_DIR)/animations/*.o
	rm -f $(LIB_DIR)/animations/camera/*.o
	rm -f $(LIB_DIR)/scenes/*.o
	rm -f $(LIB_DIR)/*.a
	rm -f tests/bin/*.out
