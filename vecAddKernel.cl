__kernel void addVectors(__global const float *a, 
	__global const float *b,
	__global float *c) {
		
		int gid = get_global_id(0);
		for(int i = 0; i < 1024*1024; ++i) {
			c[gid] += a[gid] / b[gid];
		}
	}