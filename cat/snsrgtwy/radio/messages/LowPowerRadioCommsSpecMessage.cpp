#include <stdexcept>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/RF24Handler.hpp>
#include <cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp>

#include <iostream>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
	 */
	const char LowPowerRadioCommsSpecMessage::MAX_RETRIES = 3;
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
	 */
	const size_t LowPowerRadioCommsSpecMessage::SIZE_OF_BYTE_IN_BITS = 8;
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
	 */
	const size_t LowPowerRadioCommsSpecMessage::INDEX_OF_STATUS = 2;
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
	 */
	void LowPowerRadioCommsSpecMessage::print(char message[], size_t size) {
		size_t last_index = size - 1;
		std::cout << '['  << size << "] {";
		for (size_t i = 0; i < size; i++) {
			std::cout << (short) message[i];
			if (i < last_index) {
					std::cout << ", ";
			}
		}
		std::cout << '}' << std::endl;
	}
	
	/**
	 * Interprets the given char array as bytes and returns their
	 * equivalent value.
	 * 
	 * The array is interpreted as little-endian as per the Low
	 * Power Radio To Gateway Communications Specification.
	 * 
	 * @param bytes the char array to read
	 * @param bytes_size the the size of the char array
	 * @param read the number of bytes to read
	 * @param start_position the index to begin reading from
	 * @return the interpreted value of the read bytes
	 * @throw std::out_of_range
	 */
	long LowPowerRadioCommsSpecMessage::read(
		const char bytes[],
		size_t bytes_size,
		size_t read,
		size_t start_position,
		bool little_endian
	) {
		
		const size_t size_of_byte = SIZE_OF_BYTE_IN_BITS;
		long value = 0;
		long current_byte;
		const char* what = 0;
		
		if (start_position < 0 || start_position > bytes_size-1) {
			what = "specified start position is out of range";
		} else if (read > bytes_size-start_position) {
			what = "specifed number of bytes to read is out of range";
		}
		
		if (what) {
			throw std::out_of_range(what);
		}
		
		for (size_t i = 0; i < read; i++) {
			/* The equivalent multi-line code for this one-liner is as follows: */
			current_byte = (long) bytes[start_position];
			if (little_endian) {
				value += current_byte << size_of_byte * i;
			} else {
				value += current_byte << size_of_byte * (read-i);
			}
			start_position++;
		}
		return value;
	}
	
	/**
	 * Returns the response type associated with the specified request type.
	 * 
	 * This member function simply adds 32 to the given request type so that 
	 * the equivalent lowercase char is returned as according to UTF-8.
	 * 
	 * @param request_type the type associated with the received message
	 * @return the lowercase equivalent of the given type
	 */
	char LowPowerRadioCommsSpecMessage::get_response_type(char request_type) {
		return request_type + 32;
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
	 */
	void LowPowerRadioCommsSpecMessage::before_respond_to(RF24NetworkHeader header) {
		
	}
	
	/**
	 * Performs an appropriate response according to the contents of
	 * the message.
	 * 
	 * This member function to defines an abstract algorithm for performing
	 * responses by obtaining the return value of pure virtual member functions
	 * defined by child classes. These member functions are namely
	 * get_response_type() and get_response_message().
	 * 
	 * @param header the header to use as part of the response
	 * @return true if the response was performed, false otherwise
	 */
	bool LowPowerRadioCommsSpecMessage::respond_to(RF24NetworkHeader header) {
		
		char* message;
		size_t size;
		/* Whether or not the write was successful. */
		bool success = false;
		
		LowPowerRadioCommsSpecMessage::print(this->message, this->size);
		
		/* Perform any actions necessary before the response is made. */
		this->before_respond_to(header);
		
		std::cout << "Determining response type... " << std::flush;
		/* The type of the response. */
		char type = this->get_response_type(header.type);
		std::cout << type << std::endl;
		
		std::cout << "Determining response message... " << std::endl;
		/* Whether or not the message should be deleted after use. */
		bool delete_message = this->get_response_message(message, size);
		this->print(message, size);
		
		std::cout << "Sending response... " << std::flush;	
		if (RF24Handler::get_instance()->write(header.from_node, message, type, size, 0)) {
			std::cout << "success." << std::endl;
		} else {
			std::cout << "failure!" << std::endl;
		}
		std::cout << std::endl;
		if (delete_message) {
			delete[] message;
		}
		return success;
	}
	
}}}}
	
