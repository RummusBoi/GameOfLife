__kernel void calNextGen (__global float *A, __global const int *width, __global const int *height, __global float *C, __global int *genInfo,
						  __global const int *gensPerFrame) {
	//iterate over every int in A. Calculate next gen and write it to C:
	const int MAX_SIZE = (*width) * (*height) - 1;
	const int gensPerFramePrivate = *gensPerFrame;
	
	
	int i = get_global_id (0);
	
	__global float* currGen;
	__global float* nextGen;
	
	if ((*genInfo) == 0) {
		currGen = A;
		nextGen = C;
	} else {
		currGen = C;
		nextGen = A;
	}
	
	//iterate over all eight cells around cell i:
//	for (int y = (i / (*width)) - 1; y <= (i / (*width)) + 1; y++) {
//		for (int x = i % (*width) - 1; x <= i % (*width) + 1; x++) {
//			if (x >= 0 && x < (*width) && y >= 0 && y < (*height) && !(x == i % (*width) && y == (i / (*width)))) {
//				if (A[x + (*width) * y] == 1) {
//					neighbours ++;
//				}
//			}
//		}
//	}
	
	int x = i % (*width);
	int y = i / (*width);
	
	int neighbours;
	
	neighbours = 0;
	neighbours += (currGen)[i];
	neighbours += (currGen)[x + (*width) * y - 1];
	neighbours += (currGen)[x + (*width) * y - 1 + (*width)];
	neighbours += (currGen)[x + (*width) * y - 1 - (*width)];
	neighbours += (currGen)[x + (*width) * y + (*width)];
	neighbours += (currGen)[x + (*width) * y + 1 + (*width)];
	neighbours += (currGen)[x + (*width) * y + 1 - (*width)];
	neighbours += (currGen)[x + (*width) * y - (*width)];
	neighbours += (currGen)[x + (*width) * y + 1];

	if(neighbours > 10 && neighbours < 50) {
		nextGen[i] = currGen[i] + 0.1f;
	} else {
		nextGen[i] = currGen[i] - 0.1f;
	}
	if(nextGen[i] < 0) nextGen[i] = 0;
}
