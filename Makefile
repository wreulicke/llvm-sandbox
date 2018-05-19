
clean:
	rm -rf dist
	mkdir dist

build:
	g++ `llvm-config --cxxflags --ldflags --libs --system-libs` main.cpp -o dist/main