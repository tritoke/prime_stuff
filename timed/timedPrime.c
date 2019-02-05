#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define CHECK_BIT(var,pos) ((((var) & (1<<(31-(pos%32))))==0)?0:1)
#define ZERO_BIT(var,pos) ((var) &= ~(1<<(31-(pos%32))))

int main() {
   
	int upper_limit = 10000000;
  int numInts = upper_limit/32; 
  int * bitArray;
  bitArray = malloc(numInts * sizeof(int));
   
	for (int i = 0; i < numInts; i++){
		if (i == 0) { 
			bitArray[i] = 0x3FFFFFFF;
		} else {
			bitArray[i] = 0xFFFFFFFF;
		}
	} 
	
	long double root_limit = 3163;
	
	for (long long i = 0; i < root_limit; i++) {
    char checkResult = CHECK_BIT(bitArray[i/32],i%32);
      if (checkResult == 1) {
        for (long long j = i*i; j < upper_limit; j+=i){
          ZERO_BIT(bitArray[j/32],j);
      }
		}
	}

	FILE *fptr;
	fptr = fopen("primes", "w");

	for (long long i = 0; i < upper_limit; i++) {
		if (CHECK_BIT(bitArray[i/32],i)) {
			fprintf(fptr, "%lli\n", i);
		}
	}
	
  fclose(fptr);
  free(bitArray);
  return 0;
}
