#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
	int mem = 0;
	puts("starting");
	for (long long Kb = 1LL;mem == 0;Kb++) { 
		printf("%lli\n", Kb);
		long long size = (Kb * (1 << 10));
		int * mem = (long) malloc(size);
		printf("malloc returned: %i\n", *mem);
		printf("attempted to assign: %lli bytes\n", size);
		free(mem);
	}	
	return 0;
}
