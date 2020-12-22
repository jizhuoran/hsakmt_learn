
CC = g++
CFLAGS = -O3
LIBS = -lOpenCL -lm

TARGETS = vecAdd

all:	$(TARGETS)

vecAdd: vecAdd.c /opt/rocm/lib/libhsakmt.so
	$(CC) vecAdd.c /opt/rocm/lib/libhsakmt.so -lOpenCL
clean:
	rm -f *.o $(TARGETS) *.stdout *.txt
