# 1 "/tmp/comgr-71c74c/input/CompileSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 352 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/tmp/comgr-71c74c/input/CompileSource" 2
__kernel void addVectors(__global const float *a,
 __global const float *b,
 __global float *c) {

  int gid = get_global_id(0);
  c[gid] = a[gid] + b[gid];
 }
