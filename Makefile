CC=g++
ARGS = -std=c++17 -O2

Image.o:
	$(CC) $(ARGS) -o bin/lib/Image.o -c src/lib/Image.cpp
test_image: Image.o
	$(CC) $(ARGS) -o tests/bin/test_image.out tests/src/test_image.cpp bin/lib/Image.o
