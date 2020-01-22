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

