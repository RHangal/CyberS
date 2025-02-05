all: build run

build:
	podman build -t tester .

run:
	podman run tester /bin/bash tester.sh
