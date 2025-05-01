container_tag := bigrsa

podman_run: podman_build
	podman run -it $(container_tag) make -f ./bigrsa/test/Makefile

podman_build: style
	podman build -t $(container_tag) -f test/Containerfile.test .

style:
	astyle --style=java --suffix=none --recursive "*.c" "*.h"
