.PHONY: build run clean check

build:
	mkdir -p build && cd build && cmake ../. && cmake --build .

run: build
	cd build && ./Debug/myProg ./../fileToCopy.txt out.txt

clean:
	rm -rf build

check:
	@if FC fileToCopy.txt build/out.txt >"build/comparing_log.txt" 2>&1; \
	then \
		echo "Files are equal"; \
	else \
		echo "Files are different"; \
	fi

all: clean build run check
