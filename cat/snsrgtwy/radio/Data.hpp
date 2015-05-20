#ifndef DATA_HPP
#define	DATA_HPP

#include <cstddef>
#include <string>

namespace cat { namespace snsrgtwy { namespace radio {

	/**
	 * An abstraction of possible data items that can be found in a standard
	 * sensor node radio message as defined in the Low Power Radio To Gateway
	 * Communications Specification.
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class Data {
		public:	
			static const size_t MAX_SIZE = 16;
			enum : char {
				STRING      = 0x01,
				CHAR        = 0x02,
				U_CHAR      = 0x03,
				SHORT       = 0x04,
				U_SHORT     = 0x05,
				INT         = 0x06,
				U_INT       = 0x07,
				LONG        = 0x08,
				U_LONG      = 0x09,
				LONG_LONG   = 0x0A,
				U_LONG_LONG = 0x0B,
				FLOAT       = 0x0C
				
			} size;
			union {
				char string[MAX_SIZE];
				signed char signed_char;
				unsigned char unsigned_char;
				signed short signed_short;
				unsigned short unsigned_short;
				signed int signed_integer;
				unsigned int unsigned_integer;
				signed long signed_long;
				unsigned long unsigned_long;
				float floating_point;
			};
			/**
			 * Converts the data size identifier to the associated number of
			 * bytes as defined by the Low Power Radio To Gateway
			 * Communications Specification
			 * 
			 * @param size a data size identifier as defined in the Low Power
			 * Radio To Gateway Communications Specification 
			 * @return the number of bytes associated with the given data size
			 * identifier as defined in the Low Power Radio To Gateway
			 * Communications Specification 
			 */
			static size_t to_total_bytes(char data_size);
			
			/**
			 * Constructs and returns Data based on the given value and
			 * specified size (which must be no larger than a LONG).
			 * 
			 * @param value the value to assign to the Data
			 * @param data_size the desired size of the Data
			 */
			static Data to_data_std(long value, char data_size);
			
			/**
			 * Constructs and returns Data based on the given value and
			 * specified size (which must be no smaller than a LONG_LONG).
			 * 
			 * @param value the value to assign to the Data
			 * @param data_size the desired size of the Data
			 */
			static Data to_data_big(char* value, char data_size);
			
			/**
			 * Converts an arrays of bytes to a string representing its
			 * hexadecimal value and assigns this to the given string.
			 * 
			 * @param string a reference to a string that should be assigned
			 * the conversion
			 * @param size the maximum capacity of the given string, including
			 * the '\0' terminating character
			 * @param bytes the array of bytes to be converted to hexadecimal
			 * @param byte_size the total number of bytes in the given array
			 * @param little_endian wether or not the given bytes are in a
			 * little-endian format
			 * @return the number of bytes written to the string
			 */
			static size_t to_hexadecimal_string(
				char (&string)[(Data::MAX_SIZE*2)+1],
				const char bytes[],
				size_t bytes_size,
				bool little_endian
			);
			
			static size_t to_byte_array(
				char (&byte_array)[Data::MAX_SIZE],
				const char string[]
			);
			
			/**
			 * Returns the number of bytes that the value of the data will
			 * occupy.
			 * 
			 * @return the number of bytes that the value of the data will
			 * occupy
			 */
			size_t total_bytes();
			
			/**
			 * Returns a string representation of the value of the data.
			 */
			std::string str();
	};

}}}

#endif

