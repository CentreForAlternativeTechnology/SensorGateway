#ifndef DIFFIEHELLMAN_HPP
#define DIFFIEHELLMAN_HPP

#include <cstddef>

namespace cat { namespace snsrgtwy {

	/**
	 * Diffie-Hellman arithmetic calculator.
	 */
	class DiffieHellman {
		protected:
			/**
			 * Default generator to use to calculate public integers.
			 */
			static const char* DEFAULT_GENERATOR;
			/**
			 * Default modulus to use to calculate public integers and shared keys.
			 */
			static const char* DEFAULT_MODULUS;	
			/**
			 * Generates a random value that can be used to seed random number
			 * generators.
			 * @return a random value 
			 */
			unsigned long generate_seed();
		private:
			/*
			 * The following data members and member functions are for the 
			 * implementation of Diffie-Hellman by an anonymous user:
			 * http://www.cypherspace.org/adam/rsa/dh-in-C.html
			 */
			static const int S = 17;
			unsigned char m[S];
			unsigned char g[S];
			unsigned char e[S];
			unsigned char b[S];
			int n;
			int v;
			int d;
			int z;
			
			void a(unsigned char*, unsigned char*, int);
			void s(unsigned char*);
			void r(unsigned char*);
			void M(unsigned char*, unsigned char*);
			void h(char*, unsigned char*);
			char* p(unsigned char*);
		public:
			/** 
			 * The size of strings used to represent 128-bit integers
			 * (including the terminating '\0' character).
			 */
			static const size_t SIZE = (S*2)-1;
			DiffieHellman();
			/**
			 * Calculates a given generator raised to an given exponent,
			 * which in turn is "wrapped around" a modulus
			 * @param generator the value of the generator
			 * @param exponent the value of the exponent
			 * @param modulus the value of the modulus
			 * @return the result of the calculation
			 */
			char* calculate(
				const char* generator,
				const char* exponent,
				const char* modulus
			);
			/**
			 * Calculates the value of a shared key.
			 * @param generator the public integer received
			 * @param exponent a randomly generated secret value
			 * @return the value of the shared key
			 */
			char* calculate_with_default_modulus(
				const char* generator,
				const char* exponent
			);
			/**
			 * Calculates the value of a public integer.
			 * @param exponent a randomly generated secret value
			 * @return the value of the public integer
			 */
			char* calculate_with_default_all(
				const char* exponent
			);
			/**
			 * Generates a random 128-bit integer.
			 * @return a random 128-bit integer
			 */
			char* generate_random();
			~DiffieHellman();
	};
}}

#endif

