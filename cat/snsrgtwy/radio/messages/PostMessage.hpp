#ifndef POSTMESSAGE_HPP
#define POSTMESSAGE_HPP

#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/messages/AttrMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstraction of messages from sensor nodes requesting for supplied
	 * data to be posted to Emoncms on their behalf.
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class PostMessage : public AttrMessage {
		public:
			/**
			 * The char that is used to identify this type of message
			 * according to the Low Power Radio To Gateway Communication
			 * Specification.
			 *
			 * At the time of writing, this is defined as 'P'.
			 */
			static const char TYPE = 'P';
			
			/**
			 * Constructs a message from a sensor node requesting for the
			 * supplied data to be posted to Emoncms on its behalf.
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			PostMessage(char message[]) : AttrMessage(message) {}
			
			/**
			 * Retrieves the attribute value from the message.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptionMessageException if the attribute value cannot be
			 * retrieved.
			 * 
             * @return the attribute value extracted from the message
             */
			virtual Data get_attr_value() const;
			
			/**
			 * Interprets the message and sends the appropriate request to
			 * Emoncms.
			 * 
			 * @param header the header from the received message 
			 */
			virtual void before_respond_to(RF24NetworkHeader header);
			
			/**
			 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp 
			 */
			virtual bool respond_to(RF24NetworkHeader header);
		protected:
			/**
			 * The index of the attribute value in reference to the data_items
			 * data member.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 4.
			 */
			static const size_t INDEX_OF_ATTR_VALUE;
	};
	
}}}}

#endif

