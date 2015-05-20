#include <cstddef>
#include <cstring>
#include <sstream>
#include <string>
#include <iomanip>
#include <gmp.h>
#include <cat/snsrgtwy/radio/Data.hpp>

namespace cat { namespace snsrgtwy { namespace radio {

	
	size_t Data::to_total_bytes(char data_size) {
		size_t bytes = 0;
		switch (data_size) {
			case Data::CHAR:
			case Data::U_CHAR:
				bytes = 1;
				break;
			case Data::SHORT:
			case Data::U_SHORT:
				bytes = 2;
				break;
			case Data::INT:
			case Data::U_INT:
			case Data::FLOAT:
				bytes = 4;
				break;
			case Data::LONG:
			case Data::U_LONG:
				bytes = 8;
				break;
			case Data::LONG_LONG:
			case Data::U_LONG_LONG:
				bytes = 16;
				break;
		}
		return bytes;
	}
	
	size_t Data::total_bytes() {
		return this->to_total_bytes(this->size);
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/DataMessage.hpp
	 */
	Data Data::to_data_std(long value, char data_size) {
		
		Data data;
		
		/* Assign value to Data appropriately. */
		switch (data_size) {
			case Data::CHAR:
				data.size = Data::CHAR;
				data.signed_char = value;
				break;
			case Data::U_CHAR:
				data.size = Data::U_CHAR;
				data.unsigned_char = value;
				break;
			case Data::SHORT:
				data.size = Data::SHORT;
				data.signed_short = value;
				break;
			case Data::U_SHORT:
				data.size = Data::U_SHORT;
				data.unsigned_short = value;
				break;
			case Data::INT:
				data.size = Data::INT;
				data.signed_integer = value;
				break;
			case Data::U_INT:
				data.size = Data::U_INT;
				data.unsigned_integer = value;
				break;
			case Data::U_LONG:
				data.size = Data::U_LONG;
				data.unsigned_long = value;
				break;
			case Data::FLOAT:
				data.size = Data::FLOAT;
				data.floating_point = value;
				break;
			case Data::LONG:
			default:
				data.size = Data::LONG;
				data.signed_long = value;
				break;
		}
		return data;
	}
	
	Data Data::to_data_big(char* value, char data_size) {
		Data data;
		switch (data_size) {
			case Data::LONG_LONG:
				data.size = Data::LONG_LONG;
				break;
			case Data::U_LONG_LONG:
				data.size = Data::U_LONG_LONG;
				break;
			case Data::STRING:
			default:
				data.size = Data::STRING;
				break;
		}
		memcpy(data.string, value, Data::MAX_SIZE);
		return data;
	}
	
	size_t Data::to_hexadecimal_string(
		char (&string)[(Data::MAX_SIZE*2)+1],
		const char bytes[],
		size_t bytes_size,
		bool little_endian
	) {
		std::stringstream buffer;
		bool valid_current_byte = true;
		size_t total_chars = 0;
		size_t current_byte = (little_endian) ? bytes_size-1 : 0;
		size_t size = sizeof(string);
		while (total_chars+2 < size && valid_current_byte) {
			buffer << std::uppercase << std::hex
				<< std::setw(2) << std::setfill('0')
				<< int(bytes[current_byte])
			;
			total_chars += 2;
			if (little_endian) {
				current_byte--;
				valid_current_byte = current_byte >= 0;
			} else {
				current_byte++;
				valid_current_byte = current_byte < bytes_size;
			}
		}
		memcpy(string, buffer.str().c_str(), size);
		string[size-1] = '\0';
		
		return total_chars;
	}
	
	size_t Data::to_byte_array(
		char (&byte_array)[Data::MAX_SIZE],
		const char string[]
	) {
		size_t size;
		mpz_t intermediate_conversion;
		char byte_array_trimmed[sizeof(byte_array)];
		
		/* Clear byte array. */
		memset(&(byte_array[0]), 0, Data::MAX_SIZE);
		/* Convert string to mpz_t. */
		mpz_init_set_str(intermediate_conversion, string, 16);
		/* Convert mpz_t to byte array. */
		mpz_export(
			byte_array_trimmed, /* Array to assign conversion to. */
			&size, /* size_t to assign number of written bytes to. */
			1, /* Most significant word first. */
			1, /* Number of bytes per word. */
			1, /* Most significant byte first. */
			0, /* Produce full words. */
			intermediate_conversion /* Value to convert. */
		);
		
		/* Copy the trimmed array to the padded array appropriately. */
		for (
			size_t i = sizeof(byte_array)-size, j = 0;
			i < sizeof(byte_array) && j < size;
			i++, j++
		) {
			byte_array[i] = byte_array_trimmed[j];
		}
		
		/* Return the number of written bytes */
		return size;
	}
	
	std::string Data::str() {
		char hexadecimal_string[(Data::MAX_SIZE*2)+1];
		std::stringstream buffer;
		switch (this->size) {
			case Data::CHAR:
				buffer << "\"" << this->signed_char << "\"";
				break;
			case Data::U_CHAR:
				buffer << "\"" << this->unsigned_char << "\"";
				break;
			case Data::SHORT:
				buffer << this->signed_short;
				break;
			case Data::U_SHORT:
				buffer << this->unsigned_short;
				break;
			case Data::INT:
				buffer << this->signed_integer;
				break;
			case Data::U_INT:
				buffer << this->unsigned_integer;
				break;
			case Data::LONG:
				buffer << this->signed_long;
				break;
			case Data::U_LONG:
				buffer << this->unsigned_long;
				break;
			case Data::FLOAT:
				buffer << this->floating_point;
				break;
			case Data::LONG_LONG:
			case Data::U_LONG_LONG:
			default:
				Data::to_hexadecimal_string(
					hexadecimal_string,
					this->string,
					this->total_bytes(),
					true
				);
				buffer << "\"" << hexadecimal_string << "\"";
				break;
		}
		return buffer.str();
	}
	
}}}

