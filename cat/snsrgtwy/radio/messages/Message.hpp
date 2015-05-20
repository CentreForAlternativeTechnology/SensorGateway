#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstddef>
#include <string>
#include <RF24Network/RF24Network.h>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstract representation of a message received from a sensor node.
	 * @param message the raw content of the message represented as an array of
	 * chars
	 */
	class Message {
		public:
			/**
			 * The maximum number of chars that can be allocated for this
			 * message.
			 */
			static const size_t MAX_SIZE = 128;
			
			static size_t sanitize(size_t message_size);
			static char* copy(char message[], size_t size);
			
			/**
			 * Constructs an abstract representation of a message received from
			 * a sensor node.
			 * 
			 * It is recommended that the array given to the constructor is
			 * copied to the message data member of the class. 
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			Message(char message[], size_t size);
			
			/**
			 * Performs the necessary actions to respond to the message
			 * depending on the message's content.
			 * 
			 * @return true if the response was performed, or false otherwise
			 */
			virtual bool respond_to(RF24NetworkHeader header)=0;
			
			/**
			 * Returns a string reprenting the messages as JSON.
			 */
			virtual std::string to_json();
		
			/**
			 * Destructs the message object.
			 */
			virtual ~Message();
		
		protected:
			/**
			 * the raw content of the message represented as an array of chars
			 */
			char* message;
			/**
			 * the size of the array of chars that represent the message
			 */
			size_t size;
			/**
			 * Retrieves a single character from the raw message at the given
			 * index.
			 * 
			 * If the given index is out of range of the size of the message,
			 * a std::out_of_range error is thrown.
			 * 
			 * @param index the index at which to retrieve the char
			 * @return the char at the given index in the message.
			 * @throw std::out_of_range
			 */
			char get_char_at(size_t index) const;
	};
	
}}}}

#endif
