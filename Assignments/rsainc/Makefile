IMAGE = tester

all: build run

build:
	podman build -t $(IMAGE) .

run:
	podman run $(IMAGE) python3 tester.py


