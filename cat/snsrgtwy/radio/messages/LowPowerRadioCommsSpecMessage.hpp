#ifndef LOWPOWERRADIOCOMMSSPECMESSAGE_HPP
#define LOWPOWERRADIOCOMMSSPECMESSAGE_HPP
#include <stdexcept>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>
#include <cat/snsrgtwy/radio/messages/Message.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	using namespace cat::snsrgtwy::radio;
	
	/**
	 * An abstract representation of a message received from a sensor node
	 * according to the Low Power Radio To Gateway Communications
	 * Specification.
	 */
	class LowPowerRadioCommsSpecMessage : public Message {
		public:
			/**
			 * Interprets the given char array as bytes and returns their
			 * equivalent value.
			 * 
			 * The array should be interpreted as little endian as per the Low
			 * Power Radio To Gateway Communications Specification.
			 * 
			 * @param bytes the char array to read
			 * @param bytes_size the the size of the char array
			 * @param read the number of bytes to read
			 * @param start_position the index to begin reading from
			 * @param little_endian whether or not the char array should be
			 * treated as little-endian
			 * @return the interpreted value of the read bytes
			 * @throw std::out_of_range
			 */
			static long read(
				const char bytes[],
				size_t bytes_size,
				size_t read,
				size_t start_position,
				bool little_endian
			);
			
			/**
			 * Prints a given message.
			 * 
			 * @param message the message to print
			 * @param size the number of characters in the message to print
			 * (from the left-hand side)
			 */
			static void print(char message[], size_t size);
			
			/**
			 * Constructs an abstract representation of a message received from
			 * a sensor node according to the Low Power Radio To Gateway
			 * Communications Specification.
			 * 
			 * At the time of writing, the specification defines the first 2 
			 * bytes as the length of the raw message. Therefore, this
			 * constructor should interpret these values to reallocate the 
			 * appropriate amount of memory.
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			LowPowerRadioCommsSpecMessage(char message[]) : Message(
				message,
				message == NULL ? 0 : this->read(message,MAX_SIZE,2,0,true)
					+ 4 /* 2 length bytes + 1 status byte + 1 data items byte */
			) {
				this->status = 0x01;
				try {
					this->status = this->get_char_at(this->INDEX_OF_STATUS);
				} catch (std::out_of_range oor) {
					/* Do nothing. */
				}
			}
			
			/**
			 * Performs the actions necessary for the appropriate response
			 * according to the contents of the message.
			 * 
			 * It is intended for this member function to define an abstract 
			 * algorithm for performing responses by obtaining the return value
			 * of pure virtual member functions defined by child classes. These
			 * member functions are namely get_response_type() and
			 * get_response_message().
			 * 
			 * @param header the header of the received message
			 * @return true if the response was performed, false otherwise
			 */
			virtual bool respond_to(RF24NetworkHeader header);
			
			/**
			 * Called by the respond_to() member function before any action is
			 * undertaken.
			 * 
			 * It is intended for this member function to provide no
			 * functionality, but child classes should be permitted to override
			 * the definition in order to do so.
			 * @param header the header of the received message
			 */
			virtual void before_respond_to(RF24NetworkHeader header);
			
		protected:
			/**
			 * The number of attempts that should be made to respond to the
			 * message before the perform_response() member function returns
			 * false.
			 */
			static const char MAX_RETRIES;
			/**
			 * The size of a byte represented as the number of bits.
			 * 
			 * It is very likely that this data member will be defined as 8.
			 */
			static const size_t SIZE_OF_BYTE_IN_BITS;
			/**
			 * The index of the char that defines the status of the message.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 2. 
			 */
			static const size_t INDEX_OF_STATUS;
			/**
			 * The status code of the message as defined by the Low Power Radio
			 * To Gateway Communications Specification.
			 */
			char status;
			
			/**
			 * Returns the response type associated with the specified request
			 * type.
			 * 
			 * It is intended for this member function to define a generic
			 * implementation that converts the specified char to its lower
			 * case equivalent.
			 * 
             * @param request_type the type associated with the received
			 * message
             * @return the type to use as part of the response
             */
			virtual char get_response_type(char request_type);
			
			/**
			 * Assigns a pointer and a size to the given arguments respectively
			 * for a message to use as part of the response.
			 * 
			 * @param message a reference that should be assigned a pointer
			 * to a response message that has been allocated memory
			 * @param size a reference that should be assigned the size of the
			 * response message
			 * @return true if the allocated memory of the message
			 * should be freed immediately after use, false otherwise
			 */
			virtual bool get_response_message(char* &message, size_t &size)=0;
			
		private:
			/**
			 * An abstract representation of the radio used to communicate with
			 * sensor nodes.
			 */
			RF24* radio;
			/**
			 * An abstract representation of the network used to communicate
			 * with sensor nodes.
			 */
			RF24Network* network;
			/**
			 * An abstract representation of the mesh networking layer used to
			 * communicate with sensor nodes.
			 */
			RF24Mesh* mesh;			
	};
	
}}}}

#endif
