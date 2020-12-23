__kernel void addVectors(__global float *a, 
	__global float *b,
	__global float *c) {
		
		int gid = get_global_id(0);
		c[gid] = 0;
		for(int i = 0; i < 1024 * 1024 * 32; ++i) {
			c[gid] += (a[gid] + b[gid]);
		}
	}