
SCFLAGS = -arch arm64 -Wall -Wno-error=incompatible-function-pointer-types -Wno-error=int-conversion -c -Os -fno-stack-protector -fno-unwind-tables -fno-exceptions

all:
	@xcrun -sdk iphoneos clang utils.S -o utils.o $(SCFLAGS)
	@xcrun -sdk iphoneos clang main.c -o main.o $(SCFLAGS)
	@xcrun -sdk iphoneos ld utils.o main.o -o stage1 -dylib -lSystem -syslibroot $(shell xcrun --sdk iphoneos --show-sdk-path)
	@objcopy -O binary -j .text stage1 stage1.bin
	@rm -f stage1 utils.o main.o
	@python3 build.py

clean:
	@rm -f stage1 utils.o main.o stage1.bin stage1.js ../stage1.js
