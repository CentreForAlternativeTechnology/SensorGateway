#include <cstddef>
#include <string>
#include <sstream>
#include <iostream>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiException.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseException.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterMessage.hpp>

#include <iostream>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/RegisterMessage.hpp
	 */
	const size_t RegisterMessage::EXPECTED_TOTAL_DATA_ITEMS = 0;
	
	/**
	 * @see cat/snsrgtwy/radio/messages/RegisterMessage.hpp
	 */
	size_t RegisterMessage::expected_total_data_items() const {
		return this->EXPECTED_TOTAL_DATA_ITEMS;
	}
	
	void RegisterMessage::before_respond_to(RF24NetworkHeader header) {
		
		NodeDatabase* db = NULL;
		std::stringstream ss;
		std::string address = NodeDatabase::to_string(header.from_node);
		
		/* Decrypt and interpret the message. */
		EncryptedMessage::before_respond_to(header);
		
		/* Retreive an appropriate configuration of an EmoncmsAPI. */
		web::EmoncmsApi api = web::EmoncmsApiConfig::get_copy();
		
		/* Send a registration request to Emoncms. */
		try {
			this->node_id = api.register_node(header.from_node, 0);
			this->has_valid_node_id = true;
			
			/* Associate EMonCMS ID with address. */
			db = NodeDatabaseCreator::create();
			ss << this->node_id;
			db->set_emoncms_id(address.c_str(), ss.str().c_str());
			delete db;
			
		} catch (web::EmoncmsApiException eae) {
			/* Maintain this->has_valid_node_id as false. */
		} catch (NodeDatabaseException ndbe) {
			delete db;
		}
	}
	
	char RegisterMessage::get_response_status_code() {
		return (this->has_valid_node_id) ? 0x00 : 0x01 ;
	}
	
	DataVector RegisterMessage::get_response_message_as_data_vector() {
		DataVector dv;
		if (this->has_valid_node_id) {
			dv.push_back(Data::to_data_std(this->node_id, Data::U_SHORT));
		}
		return dv;
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp 
	 */
	bool RegisterMessage::respond_to(RF24NetworkHeader header) {
		std::cout << "RegisterMessage received... " << std::endl;
		return DataMessage::respond_to(header);
	}
	
}}}}
