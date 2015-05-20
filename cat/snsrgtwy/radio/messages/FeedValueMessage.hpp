#ifndef FEEDVALUEMESSAGE_HPP
#define FEEDVALUEMESSAGE_HPP

#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/messages/PostMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstraction of messages from sensor nodes responding to previous
	 * requests for feed data.
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class FeedValueMessage : public PostMessage {
		public:
			/**
			 * The char that is used to identify this type of message
			 * according to the Low Power Radio To Gateway Communication
			 * Specification.
			 *
			 * At the time of writing, this is defined as 'P'.
			 */
			static const char TYPE = 'p';
			
			/**
			 * Constructs a message from a sensor node requesting for the
			 * supplied data to be posted to Emoncms on its behalf.
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			FeedValueMessage(char message[]) : PostMessage(message) {}
			
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
	};
	
}}}}

#endif

