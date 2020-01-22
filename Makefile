CC=g++
ARGS = -std=c++17 -O2

Image.o:
	$(CC) $(ARGS) -o bin/lib/Image.o -c src/lib/Image.cpp
LodePNG.o:
	$(CC) $(ARGS) -o bin/lib/LodePNG.o -c src/lib/lodepng.cpp
test_image: Image.o LodePNG.o
	$(CC) $(ARGS) -o tests/bin/test_image.out tests/src/test_image.cpp bin/lib/Image.o bin/lib/LodePNG.o

Vec3.o:
	$(CC) $(ARGS) -o bin/lib/Vec3.o -c src/lib/Vec3.cpp

Ray.o:
	$(CC) $(ARGS) -o bin/lib/Ray.o -c src/lib/Ray.cpp

RenderObject.o:
	$(CC) $(ARGS) -o bin/lib/RenderObject.o -c src/lib/RenderObject.cpp

Sphere.o:
	$(CC) $(ARGS) -o bin/lib/Sphere.o -c src/lib/Sphere.cpp

RenderObjectCollection.o:
	$(CC) $(ARGS) -o bin/lib/RenderObjectCollection.o -c src/lib/RenderObjectCollection.cpp

Random.o:
	$(CC) $(ARGS) -o bin/lib/Random.o -c src/lib/Random.cpp

Camera.o:
	$(CC) $(ARGS) -o bin/lib/Camera.o -c src/lib/Camera.cpp

RenderWorld.o:
	$(CC) $(ARGS) -o bin/lib/RenderWorld.o -c src/lib/RenderWorld.cpp

renderer: Ray.o Image.o Vec3.o LodePNG.o RenderObject.o Sphere.o RenderObjectCollection.o Random.o Camera.o RenderWorld.o
	$(CC) $(ARGS) -o bin/renderer.out src/Renderer.cpp bin/lib/Ray.o bin/lib/Vec3.o bin/lib/Image.o bin/lib/LodePNG.o bin/lib/RenderObject.o bin/lib/Sphere.o bin/lib/RenderObjectCollection.o bin/lib/Random.o bin/lib/Camera.o bin/lib/RenderWorld.o
