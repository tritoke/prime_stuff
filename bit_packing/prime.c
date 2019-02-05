#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CHECK_BIT(var,pos) ((((var) & (1<<(31-(pos%32))))==0)?0:1)
#define ZERO_BIT(var,pos) ((var) &= ~(1<<(31-(pos%32))))

long double square_root(long double n) {
	long double xk = 1;
	long double xn = 0;
	while (xn != xk){
		xn = xk;
		xk = (xk + n / xk) / 2;
	}
	return xk;
}

int main() {
  long long upper_limit;
	puts("enter the number you wish to find primes below");
	scanf("%lli", &upper_limit);  
	if (upper_limit < 0) {
    puts("can't find negative primes :/");
    exit(0);
  }
  printf("finding all the primes under: ");
  printf("%lli", upper_limit);
	puts("\ncreating bit array");
  long long numInts = (upper_limit/32)+((upper_limit%32==0)?0:1); 
  int * bitArray;
  bitArray = malloc(numInts * sizeof(int));
   
	puts("initializing bit array");

	for (long long i = 0; i < numInts; i++){
		if (i == 0) { 
			bitArray[i] = 0x3FFFFFFF;
		} else {
			bitArray[i] = 0xFFFFFFFF;
		}
	} 
	
	puts("array initialized");

	long double root_limit = square_root(( (long double) upper_limit ));
	
	puts("sieving primes");

	for (long long i = 0; i < root_limit; i++) {
    if (CHECK_BIT(bitArray[i/32],i)) {
      for (long long j = i*i; j < upper_limit; j+=i){
        ZERO_BIT(bitArray[j/32],j);
      }
		}
	}

	puts("primes sieved");
	puts("opening file for writing");

	FILE *fptr;
  char *fname = "primes";
	fptr = fopen(fname, "w");
	if (fptr == NULL) {
		puts("Error: couldn't create file.");
		return 1;
	}
	
	puts("writing to file");
	for (long long i = 0; i < upper_limit; i++) {
		if (CHECK_BIT(bitArray[i/32],i)) {
			fprintf(fptr, "%lli\n", i);
		}
	}
	
  printf("file written to %s", fname);
	puts("closing file");
	
  fclose(fptr);
	
  puts("file closed");
	puts("freeing memory");
  
  free(bitArray);
  
  puts("memory freed");
  puts("exiting");
  
  return 0;
}
