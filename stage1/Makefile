
SCFLAGS = -arch arm64 -Wall -Wno-error=incompatible-function-pointer-types -Wno-error=int-conversion -c -Os -fno-stack-protector -mno-sse -fno-unwind-tables -fno-exceptions 

all:
	xcrun -sdk iphoneos clang stage1.S -o stage1.o $(SCFLAGS)
	xcrun -sdk iphoneos clang stage1-c.c -o stage1-c.o $(SCFLAGS)
	xcrun -sdk iphoneos ld stage1.o stage1-c.o -o stage1 -dylib -lSystem -syslibroot $(shell xcrun --sdk iphoneos --show-sdk-path)
	/opt/homebrew/Cellar/binutils/2.44/bin/gobjcopy -O binary -j .text stage1 stage1.bin

clean:
	rm -f stage1 stage1.o stage1.bin
