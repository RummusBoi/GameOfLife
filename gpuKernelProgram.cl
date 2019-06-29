__kernel void calNextGen (__global int *A, __global const int *width, __global const int *height, __global int *C, __global int *genInfo,
						  __global const int *gensPerFrame) {
	//iterate over every int in A. Calculate next gen and write it to C:
	const int MAX_SIZE = (*width) * (*height) - 1;
	const int gensPerFramePrivate = *gensPerFrame;
	
	
	int i = get_global_id (0);
	
	__global int* currGen;
	__global int* nextGen;
	
	if ((*genInfo) == 0) {
		currGen = A;
		nextGen = C;
	} else {
		currGen = C;
		nextGen = A;
	}
	
	//iterate over all eight cells around cell i:
	int neighbours = 0;
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
	
	neighbours = 0;
	neighbours += (currGen)[x + (*width) * y - 1];
	neighbours += (currGen)[x + (*width) * y - 1 + (*width)];
	neighbours += (currGen)[x + (*width) * y - 1 - (*width)];
	neighbours += (currGen)[x + (*width) * y + (*width)];
	neighbours += (currGen)[x + (*width) * y + 1 + (*width)];
	neighbours += (currGen)[x + (*width) * y + 1 - (*width)];
	neighbours += (currGen)[x + (*width) * y - (*width)];
	neighbours += (currGen)[x + (*width) * y + 1];

	//check for rules
	if (((currGen)[i] == 0 && (neighbours == 3 || neighbours == 3))) {
		(nextGen)[i] = 1;
	} else if ((currGen)[i] == 1 && (neighbours == 2 || neighbours == 3)) {
		(nextGen)[i] = 1;
	} else {
		(nextGen)[i] = 0;
	}

}
