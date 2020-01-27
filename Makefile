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

LIBRARIES = $(LIB_DIR)/Ray.o $(LIB_DIR)/Image.o $(LIB_DIR)/Vec3.o $(LIB_DIR)/LodePNG.o $(LIB_DIR)/RenderObject.o $(LIB_DIR)/Sphere.o $(LIB_DIR)/RenderObjectCollection.o $(LIB_DIR)/Random.o $(LIB_DIR)/ProgressBar.o $(LIB_DIR)/Material.o $(LIB_DIR)/Camera.o $(LIB_DIR)/RenderWorld.o
SPEC_LIBS =
SPEC_LIBS_INCL =

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

$(LIB_DIR)/%.o: $(SRC_DIR)/lib/%.cpp
	$(CC) $(ARGS) $(OPTS) -o $@ -c $<

test_image: $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
	$(CC) $(ARGS) -o tests/bin/test_image.$(EXTENSION) tests/src/test_image.cpp $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o

test_progressbar: $(LIB_DIR)/ProgressBar.o
	$(CC) $(ARGS) -o tests/bin/test_progressbar.$(EXTENSION) tests/src/test_progressbar.cpp $(LIB_DIR)/ProgressBar.o

renderer: $(LIBRARIES) $(SPEC_LIBS_INCL)
	$(CC) $(ARGS) $(OPTS) -o $(BIN_DIR)/renderer.$(EXTENSION) $(SRC_DIR)/Renderer.cpp $(LIBRARIES) $(SPEC_LIBS) $(EXT_LIBS)

clean:
	rm -f $(BIN_DIR)/*.out
	rm -f $(LIB_DIR)/*.o
