#include <cstddef>
#include <iostream>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/radio/messages/FeedValueMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/message/PostMessage.hpp
	 */
	void FeedValueMessage::before_respond_to(RF24NetworkHeader header) {
		this->print(this->message, this->size);
		/* Decrypt and interpret the message. */
		AttrMessage::before_respond_to(header);
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp 
     */
	bool FeedValueMessage::respond_to(RF24NetworkHeader header) {
		std::cout << "FeedValueMessage received... " << std::endl;
		
		/* Decrypt and interpret the message. */
		this->before_respond_to(header);
		
		std::cout << "No response necessary." << std::endl;
		std::cout << std::endl;
		/* Inform SensorGateway to add this message to the inbox. */
		return false;
	}
	
}}}}
