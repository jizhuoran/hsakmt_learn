__kernel void addVectors(__global float *a, 
	__global float *b,
	__global float *c) {
		
		int gid = get_global_id(0);
		c[gid] = 0;
		while(true) {
			c[gid] += (a[gid] + b[gid]);
		}
	}
