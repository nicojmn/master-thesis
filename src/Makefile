PKG_CONFIG_PATH = ../../spdk/build/lib/pkgconfig
SPDK_CFLAGS := $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" pkg-config --cflags spdk_bdev spdk_event spdk_event_bdev spdk_env_dpdk)
SPDK_LIB := $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" pkg-config --libs spdk_bdev spdk_event spdk_event_bdev spdk_env_dpdk)
SYS_LIB := $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)" pkg-config --libs --static spdk_syslibs)
CC = gcc
OUT = ./out

all : test-info rwstring basic-cp queue-cp
clean:
	rm -rf ./out/*

test-info:
	$(CC) $(SPDK_CFLAGS) -g -o $(OUT)/test-info ./get_info.c -pthread -Wl,--whole-archive -Wl,-Bstatic $(SPDK_LIB) \
                -Wl,-Bdynamic -Wl,--no-whole-archive $(SYS_LIB)
hello_bdev:
	$(CC) $(SPDK_CFLAGS) -g -o $(OUT)/hello_bdev ./hello_bdev.c -pthread -Wl,--whole-archive -Wl,-Bstatic $(SPDK_LIB) \
                -Wl,-Bdynamic -Wl,--no-whole-archive $(SYS_LIB)
rwstring:
	$(CC) $(SPDK_CFLAGS) -g -o $(OUT)/rwstring ./rwstring.c -pthread -Wl,--whole-archive -Wl,-Bstatic $(SPDK_LIB) \
                -Wl,-Bdynamic -Wl,--no-whole-archive $(SYS_LIB)
basic-cp:
	$(CC) $(SPDK_CFLAGS) -g -o $(OUT)/basic-cp ./basic-cp.c -pthread -Wl,--whole-archive -Wl,-Bstatic $(SPDK_LIB) \
                -Wl,-Bdynamic -Wl,--no-whole-archive $(SYS_LIB)
queue-cp:
	$(CC) $(SPDK_CFLAGS) -g -o $(OUT)/queue-cp ./queue-cp.c -pthread -Wl,--whole-archive -Wl,-Bstatic $(SPDK_LIB) \
                -Wl,-Bdynamic -Wl,--no-whole-archive $(SYS_LIB)

test-info-run: clean test-info
	sudo $(OUT)/test-info
rw-string-run: clean rwstring
	sudo $(OUT)/rwstring
basic-cp-demo: basic-cp
	sudo $(OUT)/basic-cp -f ./demo.txt

.PHONY: test-info rwstring basic-cp hello_bdev
