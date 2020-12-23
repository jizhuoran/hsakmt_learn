# 1 "/tmp/comgr-d56330/input/CompileSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 352 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/tmp/comgr-d56330/input/CompileSource" 2
__kernel void addVectors(__global float *a,
 __global float *b,
 __global float *c) {

  int gid = get_global_id(0);
  c[gid] = 0;
  while(true) {
   c[gid] += (a[gid] + b[gid]);
  }
 }
