GXX=g++
GXX_ARGS = -std=c++17 -O2 -Wall -Wextra

BIN = bin/linux
LIB = $(BIN)/lib
SRC = src

SCENES_SRC = $(SRC)/lib/scenes
SCENES_OBJ = $(LIB)/scenes
SCENES_BIN = scenes/init

EXT_LIBS =-ldl

# Check if we're on macos
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
BIN = bin/macos
LIB = $(BIN)/lib
GXX_ARGS += -D MACOS
endif

LIBRARIES = $(LIB)/Ray.o $(LIB)/Image.a $(LIB)/Vec3.o $(LIB)/RenderObject.a $(LIB)/Random.o $(LIB)/ProgressBar.o $(LIB)/Camera.o $(LIB)/RenderWorld.o $(LIB)/Materials.a $(LIB)/Animations.a $(LIB)/StringError.o $(LIB)/BoundingBox.o $(LIB)/ObjectTree.o
INCLUDES = -I $(SRC)/lib/include/ -I $(SRC)/lib/include/objects/ -I $(SRC)/lib/include/materials/ -I $(SRC)/lib/include/animations/camera
GXX_ARGS += $(INCLUDES)

ifdef THREADED
GXX_ARGS += -D THREADED
LIBRARIES += $(LIB)/ThreadPool.o
EXT_LIBS += -lpthread
endif

ifdef DEBUG
GXX_ARGS += -g
endif

.PHONY: default all clean raytracer scene_creator scenes
default: raytracer

# FOLDER MK RULES #
$(BIN):
	mkdir -p $@
$(LIB): $(BIN)
	mkdir -p $@
$(LIB)/objects: $(LIB)
	mkdir -p $@
$(LIB)/materials: $(LIB)
	mkdir -p $@
$(LIB)/animations: $(LIB)
	mkdir -p $@
$(LIB)/animations/camera: $(LIB)/animations
	mkdir -p $@
$(SCENES_OBJ): $(LIB)
	mkdir -p $@
$(SCENES_BIN):
	mkdir -p $@

# GENERAL OBJECT COMPILE RULES #
$(LIB)/%.o: $(SRC)/lib/%.cpp | $(LIB)
	$(GXX) $(GXX_ARGS) -o $@ -c $<
$(LIB)/objects/%.o: $(SRC)/lib/objects/%.cpp | $(LIB)/objects
	$(GXX) $(GXX_ARGS) -o $@ -c $<
$(LIB)/materials/%.o: $(SRC)/lib/materials/%.cpp | $(LIB)/materials
	$(GXX) $(GXX_ARGS) -o $@ -c $<
$(LIB)/animations/%.o: $(SRC)/lib/animations/%.cpp | $(LIB)/animations
	$(GXX) $(GXX_ARGS) -o $@ -c $<
$(LIB)/animations/camera/%.o: $(SRC)/lib/animations/camera/%.cpp | $(LIB)/animations/camera
	$(GXX) $(GXX_ARGS) -o $@ -c $<


# SPECIAL OBJECT COMPILE RULES #
$(LIB)/RayTracer.o: $(SRC)/RayTracer.cpp | $(LIB)
	$(GXX) $(GXX_ARGS) -o $@ -c $<


# ARCHIVE RULES #
$(LIB)/Image.a: $(LIB)/Frames.o $(LIB)/Image.o $(LIB)/LodePNG.o
	ar rvs $(LIB)/Image.a $(LIB)/Frames.o $(LIB)/Image.o $(LIB)/LodePNG.o
$(LIB)/RenderObject.a: $(LIB)/RenderObject.o $(LIB)/objects/Sphere.o $(LIB)/objects/RenderObjectCollection.o
	ar rvs $(LIB)/RenderObject.a $(LIB)/RenderObject.o $(LIB)/objects/Sphere.o $(LIB)/objects/RenderObjectCollection.o
$(LIB)/Materials.a: $(LIB)/Material.o $(LIB)/materials/Lambertian.o $(LIB)/materials/Metal.o $(LIB)/materials/Dielectric.o
	ar rvs $(LIB)/Materials.a $(LIB)/Material.o $(LIB)/materials/Lambertian.o $(LIB)/materials/Metal.o $(LIB)/materials/Dielectric.o
$(LIB)/Animations.a: $(LIB)/RenderAnimation.o $(LIB)/CameraMovement.o $(LIB)/animations/camera/CameraRotation.o
	ar rvs $(LIB)/Animations.a $(LIB)/RenderAnimation.o $(LIB)/CameraMovement.o $(LIB)/animations/camera/CameraRotation.o


# MAIN COMPILATION #
$(BIN)/raytracer.out: $(LIB)/RayTracer.o $(LIB)/Renderer.o $(LIBRARIES)
	$(GXX) $(GXX_ARGS) -o $(BIN)/raytracer.out $(LIB)/RayTracer.o $(LIB)/Renderer.o $(LIBRARIES) $(EXT_LIBS)
raytracer: $(BIN)/raytracer.out

$(BIN)/scene_creator.out: $(LIBRARIES) $(LIB)/Materials.a $(LIB)/RenderObject.a $(LIB)/Animations.a
	$(GXX) $(GXX_ARGS) -o $(BIN)/scene_creator.out $(SRC)/SceneCreator.cpp $(LIBRARIES) $(LIB)/Materials.a $(LIB)/RenderObject.a $(LIB)/Animations.a
scene_creator: $(BIN)/scene_creator.out


# TEST COMPILE RULES #
# test_image: $(LIB)/Image.o $(LIB)/LodePNG.o
# 	$(GXX) $(GXX_ARGS) -o tests/bin/test_image.out tests/src/test_image.cpp $(LIB)/Image.o $(LIB)/LodePNG.o

# test_progressbar: $(LIB)/ProgressBar.o
# 	$(GXX) $(GXX_ARGS) -o tests/bin/test_progressbar.out tests/src/test_progressbar.cpp $(LIB)/ProgressBar.o

# test_json: $(LIB)/Vec3.o $(LIB)/WorldIO.o
# 	$(GXX) $(GXX_ARGS) -o tests/bin/test_json.out tests/src/test_json.cpp $(LIB)/Vec3.o $(LIB)/WorldIO.o

# test_frames: $(LIB)/Image.a $(LIB)/Vec3.o
# 	$(GXX) $(GXX_ARGS) -o tests/bin/test_frames.out tests/src/test_frames.cpp $(LIB)/Image.a $(LIB)/Vec3.o

# test_vector_math: $(LIB)/Image.a $(LIB)/Vec3.o
# 	$(GXX) $(GXX_ARGS) -o tests/bin/test_vector_math.out tests/src/test_vector_math.cpp $(LIB)/Image.a $(LIB)/Vec3.o

$(SCENES_OBJ)/%.o: $(SCENES_SRC)/%.cpp | $(SCENES_OBJ)
	$(GXX) $(GXX_ARGS) -fPIC -o $@ -c $<

$(SCENES_BIN)/random.so: $(SCENES_OBJ)/random.o $(LIB)/Random.o $(LIB)/RenderObject.a $(LIB)/Materials.a $(LIB)/RenderWorld.o | $(SCENES_BIN)
	$(GXX) $(GXX_ARGS) -shared -o $@ $< $(LIB)/Random.o $(LIB)/RenderObject.a $(LIB)/Materials.a $(LIB)/RenderWorld.o

scenes: $(SCENES_BIN)/random.so

all: raytracer scene_creator scenes

clean:
	rm -f $(BIN)/*.out
	rm -f $(LIB)/*.o
	rm -f $(LIB)/materials/*.o
	rm -f $(LIB)/objects/*.o
	rm -f $(LIB)/animations/*.o
	rm -f $(LIB)/animations/camera/*.o
	rm -f $(LIB)/*.a
	rm -f $(SCENES_OBJ)/*.o
	rm -f $(SCENES_BIN)/*.so
	rm -f tests/bin/*.out
