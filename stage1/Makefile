all: clean
	@xcrun -sdk iphoneos clang *.c *.S -o stage1 -lobjc -isysroot $(shell xcrun --sdk iphoneos --show-sdk-path)
	@python3 build.py
	
clean:
	@rm -f stage1 stage1.js ../stage1.js
