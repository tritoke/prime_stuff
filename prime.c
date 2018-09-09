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
	scanf("%lli", &upper_limit);  	//scans the number that the user enters
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
	} //this sets up the bit array with indices 0 and 1 being false, and the rest being true	
	
	puts("array initialized");

	long double root_limit = square_root(( (long double) upper_limit ));
	
	puts("sieving primes");

	for (long long i = 0; i < root_limit; i++) {
        //printf("%x \t %lli\n", bitArray[i/32], i);
        char checkResult = CHECK_BIT(bitArray[i/32],i%32);
        //printf("bit at position %lld of bitArray[%lld] = %d\n", i%32, i/32, checkResult);
        if (checkResult == 1) {
            //printf("%d\n",checkResult);
			//printf("i*i = %lli, upper_limit = %lli", i*i, upper_limit);
            for (long long j = i*i; j < upper_limit; j+=i){
				//printf("%lli\n", j);
                ZERO_BIT(bitArray[j/32],j%32);
			    //printf("bit %lld of bitArray[%lld] 0=%d\n", j%32, j/32, CHECK_BIT(bitArray[j/32],j%32));
            }
		}
	}

	puts("primes sieved");
	puts("opening file for writing");

	FILE *fptr;
	fptr = fopen("../prime_output", "w");
	if (fptr == NULL) {
		puts("Error: couldn't create file.");
		return 1;
	}
	
	puts("writing to file");
	for (long long i = 0; i < upper_limit; i++) {
		if (CHECK_BIT(bitArray[i/32],i%32) == 1) {
			fprintf(fptr, "%lli\n", i);
		}
	}
	puts("file written to ../prime_output");
	puts("closing file");
	fclose(fptr);
	puts("file closed");
	puts("freeing memory");
    free(bitArray);
    puts("memory freed");
    puts("exiting");
	return 0;
}
