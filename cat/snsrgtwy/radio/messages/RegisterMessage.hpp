#ifndef REGISTERMESSAGE_HPP
#define REGISTERMESSAGE_HPP

#include <cstddef>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstraction of messages from sensor nodes requesting to be registered
	 * with Emoncms.
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class RegisterMessage : public EncryptedMessage {
		public:
			/**
			 * The char that is used to identify this type of message
			 * according to the Low Power Radio To Gateway Communication
			 * Specification.
			 *
			 * At the time of writing, this is defined as 'R'.
			 */
			static const char TYPE = 'R';
			/**
			 * Constructs a message from a sensor node requesting to be
			 * registered with Emoncms.
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			RegisterMessage(char message[]) : EncryptedMessage(message) {
				this->has_valid_node_id = false;
			}
			
			/**
			 * Interprets the message and send a registration request to the
			 * Emoncms API.
			 * 
			 * @param header the header of the received message.
			 */
			virtual void before_respond_to(RF24NetworkHeader header);
			
			/**
			 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp 
			 */
			virtual bool respond_to(RF24NetworkHeader header);
		protected:
			/**
			 * The total number of data items expected for a message of this
			 * type.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 0.
			 */
			static const size_t EXPECTED_TOTAL_DATA_ITEMS;
			
			/**
			 * Whether or not the node_id data member has been assigned a
			 * valid value by the Emoncms API.
			 */
			bool has_valid_node_id;
			
			/**
			 * The node ID specified by the call to the Emoncms API.
			 */
			unsigned short node_id;
			
			/**
			 * Returns the total number of data items expected for a message of
			 * this type.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 0.
			 * 
			 * @return the total number of data items expected
			 * 
             * @see cat/snsrgtwy/radio/messages/DataMessage.hpp
             */
			virtual size_t expected_total_data_items() const;
			
			/**
			 * Assigns a pointer and a size to the given arguments respectively
			 * for a message to use as part of the response.
			 * 
			 * It is intended for this message to reflect the response from
			 * Emoncms after registering on behalf of the node.
			 * 
			 * @param message a reference that should be assigned a pointer
			 * to a response message that has been allocated memory
			 * @param size a reference that should be assigned the size of the
			 * response message
			 * @return true if the allocated memory of the message
			 * should be freed immediately after use, false otherwise
			 */
			
			virtual char get_response_status_code();
			virtual DataVector get_response_message_as_data_vector();
	};
	
}}}}

#endif
