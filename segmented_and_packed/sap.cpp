#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <cmath>

/* L1 cache is 256k */ 
const int64_t L1_CACHE_SIZE = 262144;

#define CHECK_BIT(var,pos) ((((var) & (1<<(31-(pos%32))))==0)?0:1)
#define ZERO_BIT(var,pos) ((var) &= ~(1<<(31-(pos%32))))

int main(void) {
  int64_t limit = 100; //00000; 
  int64_t sqrt_lim = (int64_t) std::sqrt(limit);
  int64_t segment_size = 10;//(int64_t) std::sqrt(L1_CACHE_SIZE);

  /* generate sieving primes */
  std::vector<int64_t> sieving_primes;
  
  int64_t num_ints = (sqrt_lim/32) + ((sqrt_lim%32==0)?0:1);
  int32_t bit_array[num_ints];
  
  bit_array[0] = 0x3FFFFFFF;
  for (int i = 1; i < num_ints; i++) {
    bit_array[i] = 0xFFFFFFFF;
  }
  
  int64_t lim = (int64_t) std::sqrt(sqrt_lim);
  for (int64_t i = 0; i < lim; i++) {
    if (CHECK_BIT(bit_array[i/32],i)) {
      for (int64_t j = i*i; j < sqrt_lim; j+=i) ZERO_BIT(bit_array[j/32],j);
    }
  }

  for (int64_t i = 0; i < sqrt_lim; i++) {
    /* look at removing the %32 from checkbit */
    if (CHECK_BIT(bit_array[i/32],i%32)) sieving_primes.push_back(i); 
  }
  /* the sieving primes */
  int64_t * sps = sieving_primes.data();
  /* current multiples of the sieving primes */
  int64_t sp_mults[sieving_primes.size()];
  for (size_t i = 0; i < sieving_primes.size(); i++) sp_mults[i] = 1;
  /* number of sieving primes */
  int64_t num_sps = sieving_primes.size();

  /* main sieving loop */
  std::vector<int64_t> primes;
  int32_t sieve[segment_size];
  
  for (int64_t low = 0; low <= limit; low += segment_size*32) {
    for (int64_t j = 0; j < segment_size; j++) {
      sieve[j] = 0xFFFFFFFF;
    }
    if (!low) sieve[0] = 0x3FFFFFFF;
    
    int64_t high = low + segment_size - 1;
    high = std::min(limit, high);
    
    for (int64_t i = 0; i < num_sps; i++) {
      for (int64_t n = sps[i]*sp_mults[i];
           n <= high;
           n+=sps[i], sp_mults[i]++) {
        ZERO_BIT(sieve[n/32],n);
      }
    }
    
    for (int64_t i=0;i<segment_size && (low+i) < limit;i++) {
      if (CHECK_BIT(sieve[i/32],i)) {
        primes.push_back(low+i);
      }
    }
  }
  std::string fname = "primes";
  std::ofstream outfile;
  outfile.open(fname);
  for (int64_t prime : primes) outfile << prime << "\n";
  outfile.close();
  return 0;
}
