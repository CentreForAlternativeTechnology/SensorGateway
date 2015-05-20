#include <cstddef>
#include <iostream>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterAttrMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/RegisterAttrMessage.hpp
	 */
	const size_t RegisterAttrMessage::INDEX_OF_ATTR_DEFAULT_VALUE = AttrMessage::INDEX_OF_LAST_DATA_ITEM;	

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
	Data RegisterAttrMessage::get_attr_default_value() const {
		return this->get_data_item_at(this->INDEX_OF_ATTR_DEFAULT_VALUE);
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/RegisterAttrMessage.hpp
	 */
	void RegisterAttrMessage::before_respond_to(RF24NetworkHeader header) {
		
		/* Decrypt and interpret the message. */
		AttrMessage::before_respond_to(header);
		
		/* Retrieve a copy of an appropriate EmoncmsAPI configuration. */
		web::EmoncmsApi api = web::EmoncmsApiConfig::get_copy();
		
		/* Send an attribute registration request to Emoncms. */
		api.register_attr(
			this->get_node_id(),
			this->get_group_id(),
			this->get_attr_id(),
			this->get_attr_number(),
			this->get_attr_default_value().str().c_str(),
			0
		);
	}
	
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp 
     */
	bool RegisterAttrMessage::respond_to(RF24NetworkHeader header) {
		std::cout << "RegisterAttrMessage received... " << std::endl;
		return AttrMessage::respond_to(header);
	}
	
}}}}
