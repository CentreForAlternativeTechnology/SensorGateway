#include <cstddef>
#include <iostream>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/radio/messages/PostMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/PostMessage.hpp
	 */
	const size_t PostMessage::INDEX_OF_ATTR_VALUE = AttrMessage::INDEX_OF_LAST_DATA_ITEM;	

	/**
	 * Retrieves the attribute value from the message.
	 * 
	 * This member function throws a DataMessageCorruptionException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function) or if the attribute value simply does not exist in the raw
	 * message.
	 * 
	 * @return the attribute value extracted from the message
	 */
	Data PostMessage::get_attr_value() const {
		return this->get_data_item_at(this->INDEX_OF_ATTR_VALUE);
	}
	
	/**
	 * @see cat/snsrgtwy/radio/message/PostMessage.hpp
	 */
	void PostMessage::before_respond_to(RF24NetworkHeader header) {
		
		/* Decrypt and interpret the message. */
		AttrMessage::before_respond_to(header);
		
		/* Retreive a copy of the appropriate configuration. */
		web::EmoncmsApi api = web::EmoncmsApiConfig::get_copy();
		
		/* Send a post request to the Emoncms API. */
		api.post(
			this->get_node_id(),
			this->get_group_id(),
			this->get_attr_id(),
			this->get_attr_number(),
			this->get_attr_value().str().c_str(),
			0
		);
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp 
     */
	bool PostMessage::respond_to(RF24NetworkHeader header) {
		std::cout << "PostMessage received... " << std::endl;
		return AttrMessage::respond_to(header);
	}
	
}}}}
