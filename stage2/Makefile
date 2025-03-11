SCFLAGS = -arch arm64 -Wall -Wno-error=incompatible-function-pointer-types -Wno-error=int-conversion -c -Os -fno-stack-protector -fno-unwind-tables -fno-exceptions

all: clean
	@xcrun -sdk iphoneos clang main.c -o main.o $(SCFLAGS)
	@xcrun -sdk iphoneos clang utils.S -o utils.o $(SCFLAGS)
	@xcrun -sdk iphoneos ld main.o utils.o -o stage2 -lSystem -lobjc -syslibroot $(shell xcrun --sdk iphoneos --show-sdk-path)
	@rm -f *.o
	@python3 build.py
	
clean:
	@rm -f stage2 stage2.js ../stage2.js *.o
