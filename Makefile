
CC = g++
CFLAGS = -O3
LIBS = -lOpenCL -lm

TARGETS = vecAdd

all:	$(TARGETS)

vecAdd: vecAdd.cpp /opt/rocm/lib/libhsakmt.so /opt/rocm-4.0.0/lib/librocm-dbgapi.so
	$(CC) vecAdd.cpp /opt/rocm/lib/libhsakmt.so /opt/rocm-4.0.0/lib/librocm-dbgapi.so -lOpenCL -ldl
clean:
	rm -f *.o $(TARGETS) *.stdout *.txt
