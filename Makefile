CC=g++
ARGS = -std=c++17 -O2

NVCC=nvcc
NVCC_ARGS = -std=c++17

BIN_DIR = bin/linux
LIB_DIR = $(BIN_DIR)/lib
SRC_DIR = src
EXTENSION =out

OPTS =
EXT_LIBS =

# Check if we're on windows
ifeq ($(OS),Windows_NT)
BIN_DIR = bin/win
LIB_DIR = $(BIN_DIR)/lib
EXTENSION =exe
OPTS += -D WINDOWS
endif

LIBRARIES = $(LIB_DIR)/Ray.o $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o $(LIB_DIR)/RenderObject.a $(LIB_DIR)/Random.o $(LIB_DIR)/ProgressBar.o $(LIB_DIR)/Camera.o $(LIB_DIR)/RenderWorld.o $(LIB_DIR)/Materials.a $(LIB_DIR)/WorldIO.o $(LIB_DIR)/objects/RenderObjectCollection.o $(LIB_DIR)/scenes/RandomScene.o $(LIB_DIR)/Animations.a

ifdef THREADED
ifdef CUDA
$(error Cannot run threaded with CUDA)
endif
OPTS += -D RENDER_THREADED
LIBRARIES += $(LIB_DIR)/ThreadPool.o
EXT_LIBS += -lpthread
endif

ifdef CUDA
CC = $(NVCC)
ARGS = $(NVCC_ARGS)
OPTS += -D CAMERA_CUDA
endif

ifdef DEBUG
OPTS += -g
endif


# GENERAL OBJECT COMPILE RULES #
$(LIB_DIR)/%.o: $(SRC_DIR)/lib/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/objects/%.o: $(SRC_DIR)/lib/objects/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/materials/%.o: $(SRC_DIR)/lib/materials/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/scenes/%.o: $(SRC_DIR)/lib/scenes/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/animations/%.o: $(SRC_DIR)/lib/animations/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<
$(LIB_DIR)/animations/camera/%.o: $(SRC_DIR)/lib/animations/camera/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<


# SPECIAL OBJECT COMPILE RULES #
Renderer.o: $(SRC_DIR)/lib/Renderer.cpp
	$(CC) $(ARGS) $(OPTS) -o $(LIB_DIR)/Renderer.o -c $(SRC_DIR)/lib/Renderer.cpp


# ARCHIVE RULES #
$(LIB_DIR)/Image.a: $(LIB_DIR)/Frames.o $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
	ar rvs $(LIB_DIR)/Image.a $(LIB_DIR)/Frames.o $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
$(LIB_DIR)/RenderObject.a: $(LIB_DIR)/RenderObject.o $(LIB_DIR)/objects/Sphere.o $(LIB_DIR)/objects/RenderObjectCollection.o
	ar rvs $(LIB_DIR)/RenderObject.a $(LIB_DIR)/RenderObject.o $(LIB_DIR)/objects/Sphere.o $(LIB_DIR)/objects/RenderObjectCollection.o
$(LIB_DIR)/Materials.a: $(LIB_DIR)/Material.o $(LIB_DIR)/materials/Lambertian.o $(LIB_DIR)/materials/Metal.o $(LIB_DIR)/materials/Dielectric.o
	ar rvs $(LIB_DIR)/Materials.a $(LIB_DIR)/Material.o $(LIB_DIR)/materials/Lambertian.o $(LIB_DIR)/materials/Metal.o $(LIB_DIR)/materials/Dielectric.o
$(LIB_DIR)/Animations.a: $(LIB_DIR)/Animation.o $(LIB_DIR)/animations/CameraMovement.o $(LIB_DIR)/animations/camera/CameraRotation.o
	ar rvs $(LIB_DIR)/Animations.a $(LIB_DIR)/Animation.o $(LIB_DIR)/animations/CameraMovement.o $(LIB_DIR)/animations/camera/CameraRotation.o


# MAIN COMPILATION #
raytracer: Renderer.o $(LIBRARIES)
	$(CC) $(ARGS) $(OPTS) -o $(BIN_DIR)/raytracer.$(EXTENSION) $(SRC_DIR)/RayTracer.cpp $(LIB_DIR)/Renderer.o $(LIBRARIES) $(EXT_LIBS)
scene_creator: $(LIBRARIES)
	$(CC) $(ARGS) $(OPTS) -o $(BIN_DIR)/scene_creator.$(EXTENSION) $(SRC_DIR)/SceneCreator.cpp $(LIBRARIES)


# TEST COMPILE RULES #
test_image: $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
	$(CC) $(ARGS) -o tests/bin/test_image.$(EXTENSION) tests/src/test_image.cpp $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o

test_progressbar: $(LIB_DIR)/ProgressBar.o
	$(CC) $(ARGS) -o tests/bin/test_progressbar.$(EXTENSION) tests/src/test_progressbar.cpp $(LIB_DIR)/ProgressBar.o

test_json: $(LIB_DIR)/Vec3.o $(LIB_DIR)/WorldIO.o
	$(CC) $(ARGS) -o tests/bin/test_json.$(EXTENSION) tests/src/test_json.cpp $(LIB_DIR)/Vec3.o $(LIB_DIR)/WorldIO.o

test_frames: $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o
	$(CC) $(ARGS) -o tests/bin/test_frames.$(EXTENSION) tests/src/test_frames.cpp $(LIB_DIR)/Image.a $(LIB_DIR)/Vec3.o

clean:
	rm -f $(BIN_DIR)/*.out
	rm -f $(LIB_DIR)/*.o
	rm -f $(LIB_DIR)/materials/*.o
	rm -f $(LIB_DIR)/objects/*.o
	rm -f $(LIB_DIR)/*.a
	rm -f tests/bin/*.out
