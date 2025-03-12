SCFLAGS = -arch arm64 -Wall -Wno-error=incompatible-function-pointer-types -Wno-error=int-conversion -Os -fno-stack-protector -fno-unwind-tables -fno-exceptions

all: clean
	@xcrun -sdk iphoneos clang *.c -o stage1 -lobjc -isysroot $(shell xcrun --sdk iphoneos --show-sdk-path) $(SCFLAGS)
	@python3 -c "open('../stage1.js', 'w').write('var stage1 = new Uint8Array([' + ','.join(map(str, open('stage1', 'rb').read())) + ']);\n')"

clean:
	@rm -f stage1 ../stage1.js
