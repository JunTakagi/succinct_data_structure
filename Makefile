LIB = lib

all:build

build:
	make -C $(LIB)

clean:
	make clean -C $(LIB)
