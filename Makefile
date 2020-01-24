CC=g++
ARGS = -std=c++17 -O2

BIN_DIR = bin
LIB_DIR = bin/lib
SRC_DIR = src

LIBRARIES = $(LIB_DIR)/Ray.o $(LIB_DIR)/Image.o $(LIB_DIR)/Vec3.o $(LIB_DIR)/LodePNG.o $(LIB_DIR)/RenderObject.o $(LIB_DIR)/Sphere.o $(LIB_DIR)/RenderObjectCollection.o $(LIB_DIR)/Random.o $(LIB_DIR)/Camera.o $(LIB_DIR)/ProgressBar.o $(LIB_DIR)/Material.o

$(LIB_DIR)/%.o: $(SRC_DIR)/lib/%.cpp
	$(CC) $(ARGS) -o $@ -c $<

test_image: Image.o LodePNG.o
	$(CC) $(ARGS) -o tests/bin/test_image.out tests/src/test_image.cpp bin/lib/Image.o bin/lib/LodePNG.o

test_progressbar: ProgressBar.o
	$(CC) $(ARGS) -o tests/bin/test_progressbar.out tests/src/test_progressbar.cpp bin/lib/ProgressBar.o

renderer: $(LIBRARIES)
	$(CC) $(ARGS) -o $(BIN_DIR)/renderer.out $(SRC_DIR)/Renderer.cpp $(LIBRARIES)
renderer_multithreaded: Ray.o Image.o Vec3.o LodePNG.o RenderObject.o Sphere.o RenderObjectCollection.o Random.o src/lib/Camera.cpp ProgressBar.o Material.o
	$(CC) $(ARGS) -DCAMERA_MULTITHREADED -o bin/lib/Camera.o -c src/lib/Camera.cpp
	$(CC) $(ARGS) -DCAMERA_MULTITHREADED -o bin/renderer.out src/Renderer.cpp bin/lib/Ray.o bin/lib/Vec3.o bin/lib/Image.o bin/lib/LodePNG.o bin/lib/RenderObject.o bin/lib/Sphere.o bin/lib/RenderObjectCollection.o bin/lib/Random.o bin/lib/Camera.o bin/lib/ProgressBar.o bin/lib/Material.o -lpthread
renderer_debug: $(LIBRARIES)
	$(CC) $(ARGS) -g -o $(BIN_DIR)/renderer.out $(SRC_DIR)/Renderer.cpp $(LIBRARIES)
