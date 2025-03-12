SCFLAGS = -arch arm64 -Wall -Wno-error=incompatible-function-pointer-types -Wno-error=int-conversion -c -Os -fno-stack-protector -fno-unwind-tables -fno-exceptions

all: clean
	@xcrun -sdk iphoneos clang main.c -o main.o $(SCFLAGS)
	@xcrun -sdk iphoneos clang dyld.c -o dyld.o $(SCFLAGS)
	@xcrun -sdk iphoneos clang utils.S -o utils.o $(SCFLAGS)
	@xcrun -sdk iphoneos ld main.o utils.o dyld.o -o stage1 -lSystem -lobjc -syslibroot $(shell xcrun --sdk iphoneos --show-sdk-path)
	@rm -f *.o
	@python3 -c "open('../stage1.js', 'w').write('var stage1 = new Uint8Array([' + ','.join(map(str, open('stage1', 'rb').read())) + ']);\n')"
	
clean:
	@rm -f stage1 ../stage1.js *.o
