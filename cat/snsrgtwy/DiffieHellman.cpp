#include <cstddef>
#include <cstring>
#include <fstream>
#include <gmp.h>
#include <cat/snsrgtwy/DiffieHellman.hpp>

namespace cat { namespace snsrgtwy {
	
	const char* DiffieHellman::DEFAULT_GENERATOR = "6";
	const char* DiffieHellman::DEFAULT_MODULUS = "4341544D616A6F7250726F6A65637447";
	//const size_t DiffieHellman::PRIME_CERTAINTY = 1024;
	//const size_t DiffieHellman::PRIME_TIMEOUT = 64;

	DiffieHellman::DiffieHellman() {
		
	}

	void DiffieHellman::a(unsigned char *x, unsigned char *y, int o){
		d=0;
		for(v=S;v--;){
			d+=x[v]+y[v]*o;
			x[v]=d;d=d>>8;
		}
	}

	void DiffieHellman::s(unsigned char *x){
		for(v=0;(v<S-1)&&(x[v]==m[v]);) v++;
		if(x[v]>=m[v]) a(x,m,-1);
	}

	void DiffieHellman::r(unsigned char *x){
		d=0;
		for(v=0;v< S;){
			d|=x[v];
			x[v++]=d/2;d=(d&1)<<8;
		}
	}

	void DiffieHellman::M(unsigned char *x, unsigned char *y){
		unsigned char X[S],Y[S];
		memcpy(X,x,S);
		memcpy(Y,y,S);
		memset(x,0,S);
		for(z=S*8;z--;){
			if(X[S-1]&1){
				a(x,Y,1);
				s(x);
			}
			r(X);
			a(Y ,Y,1);
			s(Y);
		}
	}

	void DiffieHellman::h(char *x, unsigned char *y){
		memset(y,0,S);
		for(n=0;x[n]>0;n++){
			for(z=4;z--;) a(y,y ,1);
			x[n]|=32;
			y[S-1]|=x[n]-48-(x[n]>96)*39;
		}
	}

	char* DiffieHellman::p(unsigned char *x){
		size_t i = 0;
		char* result = new char[SIZE]();
		for(n=0;!x[n];)n++;
		for(;n< S;n++) {
			result[i++] = 48+x[n]/16+(x[n]>159)*7;
			result[i++] = 48+(x[n]&15)+7*((x[n]&15)>9);
		}
		result[i] = '\0';
		return result;
	}
	
	unsigned long DiffieHellman::generate_seed() {
		const size_t size = sizeof(unsigned long);
		union {
			char string[size];
			unsigned long unsigned_long;
		} seed;
		
		/* Open the /dev/random device for reading. */
		std::ifstream random("/dev/urandom", std::ifstream::binary);
		if (random) {
			random.read(seed.string, size);
			random.close();
		}
		return seed.unsigned_long;
	}
	
	char* DiffieHellman::calculate(
		const char* generator,
		const char* exponent,
		const char* modulus
	) {

		char gen[SIZE];
		char exp[SIZE];
		char mod[SIZE]; 
		strncpy(gen, generator, SIZE);
		strncpy(exp, exponent, SIZE);
		strncpy(mod, modulus, SIZE);

		h(gen,g);
		h(exp,e);
		h(mod,m);
		memset(b,0,S);
		b[S-1]=1;
		for(n=S*8;n--;){
			if(e[S-1]&1)M(b,g);
			M(g,g);
			r(e);
		}
		return p(b);
	}

	char* DiffieHellman::calculate_with_default_modulus(
		const char* generator,
		const char* exponent
	) {
		return this->calculate(
			generator,
			exponent,
			this->DEFAULT_MODULUS
		);
	}

	char* DiffieHellman::calculate_with_default_all(
		const char* exponent
	) {
		return this->calculate_with_default_modulus(
			this->DEFAULT_GENERATOR,
			exponent
		);
	}

	char* DiffieHellman::generate_random() {
		const unsigned long bits = (S-1)*8;

		gmp_randstate_t state;
		mpz_t random;
		char* random_string = NULL;

		/* Use Mersenne Twister to generate random numbers (fast and random). */
		gmp_randinit_default(state);
		gmp_randseed_ui(state, this->generate_seed());
		
		/* Generate a random number. */
		mpz_init(random);
		mpz_urandomb(random, state, bits);

		/* Allocate memory for the string. */
		random_string = new char[SIZE]();
		mpz_get_str(random_string, -16 /* Uppercase hexadecimal. */, random);
		/* Pad the string with zeros if required. */
		/*
		sprintf(prime_string, "%*s", SIZE-1, prime_string);
		for(size_t i=0; prime_string[i] == ' ' && i < SIZE; i++) {
			prime_string[i] = '0';
		}
		*/
		
		return random_string;
	}

	DiffieHellman::~DiffieHellman() {

	}

}}
