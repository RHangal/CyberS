all: build compile test

build:
	podman build -t enigma .

compile:
	podman run --rm enigma gcc enigma.c -o enigma.out

run-exec:
	@echo "\nRunning local executable ------------------------"
	podman run --rm enigma ./enigma.out "AAA"

test-local: build compile run-exec

test:
	podman run --rm enigma python3 tester.py

