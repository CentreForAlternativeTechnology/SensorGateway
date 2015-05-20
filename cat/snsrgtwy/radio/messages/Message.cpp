#include <cstddef>
#include <stdexcept>
#include <string>
#include <cat/snsrgtwy/radio/messages/Message.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	size_t Message::sanitize(size_t message_size) {
		if (message_size > MAX_SIZE || message_size < 1) {
			message_size = MAX_SIZE;
		}
		return message_size;
	}
	
	char* Message::copy(char message[], size_t size) {
		char* message_copy = new char[size]();
		memcpy(message_copy, message, size);
		return message_copy;
	}
	
	/**
	 * Constructs an abstract representation of a message received from a
	 * sensor node.
	 * 
	 * The char array given to the constructor is copied to the message data
	 * member of the object.
	 *
	 * If the given size is greater than the value of Message::MAX_SIZE, it
	 * will be limited to the value of Message::MAX_SIZE.
	 * 
	 * @param message the raw content of the message represented as an array of
	 * chars
	 * @param size the size of the given char array
	 */
	Message::Message(char message[], size_t size = MAX_SIZE) {
		if (message == NULL) {
			this->size = 0;
			this->message = NULL;
		} else {
			this->size = this->sanitize(size);
			this->message = this->copy(message, this->size);
		}
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/Message.hpp
	 */
	char Message::get_char_at(size_t index) const {
		
		if (this->size < 1 || index > this->size-1) {
			throw std::out_of_range(
				"the given index is out of range of the message size"
			);
		}
		
		return this->message[index];
	}
	
	std::string Message::to_json() {
		return "{}";
	}
	
	/**
	 * Destructs the message.
	 */
	Message::~Message() {
		if (this->message != NULL) {
			delete[] this->message;
		}
	}
	
}}}}
	
