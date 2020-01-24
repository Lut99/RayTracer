CC=g++
ARGS = -std=c++17 -O2

BIN_DIR = bin
LIB_DIR = bin/lib
SRC_DIR = src

LIBRARIES = $(LIB_DIR)/Ray.o $(LIB_DIR)/Image.o $(LIB_DIR)/Vec3.o $(LIB_DIR)/LodePNG.o $(LIB_DIR)/RenderObject.o $(LIB_DIR)/Sphere.o $(LIB_DIR)/RenderObjectCollection.o $(LIB_DIR)/Random.o $(LIB_DIR)/ProgressBar.o $(LIB_DIR)/Material.o

OPTS :=
LIBS :=

ifdef THREADS
OPTS += -D CAMERA_THREADS=$(THREADS)
LIBS += -lpthread
endif

ifdef DEBUG
OPTS += -g
endif

Camera.o: $(SRC_DIR)/lib/Camera.cpp
	$(CC) $(ARGS) $(OPTS) -o ${LIB_DIR}/Camera.o -c $(SRC_DIR)/lib/Camera.cpp

$(LIB_DIR)/%.o: $(SRC_DIR)/lib/%.cpp
	$(CC) $(ARGS) -o $@ -c $<

test_image: $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o
	$(CC) $(ARGS) -o tests/bin/test_image.out tests/src/test_image.cpp $(LIB_DIR)/Image.o $(LIB_DIR)/LodePNG.o

test_progressbar: $(LIB_DIR)/ProgressBar.o
	$(CC) $(ARGS) -o tests/bin/test_progressbar.out tests/src/test_progressbar.cpp $(LIB_DIR)/ProgressBar.o

renderer: $(LIBRARIES) Camera.o
	$(CC) $(ARGS) $(OPTS) -o $(BIN_DIR)/renderer.out $(SRC_DIR)/Renderer.cpp ${LIB_DIR}/Camera.o $(LIBRARIES) $(LIBS)
