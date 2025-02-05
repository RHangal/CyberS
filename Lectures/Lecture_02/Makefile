all: build run

build:
	podman build -t printb .

run:
	podman run -it printb nvim printb.c
