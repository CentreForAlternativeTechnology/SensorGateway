#include <cstddef>
#include <string>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/DataVector.hpp>
#include <cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp>
#include <cat/snsrgtwy/web/server/FeedValueNodeRequest.hpp>

#include <iostream>
#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	const char FeedValueNodeRequest::TYPE = 'P';
	
	FeedValueNodeRequest::FeedValueNodeRequest(std::string node, const char id[], size_t id_size) {
		using namespace cat::snsrgtwy::radio::messages;
		
		/* Attempt to convert id to individual values. */
		DataVector dv;
		
		/* Convert id to usable types. */
		this->node_id = atoi(node.c_str());
		try {
		this->group_id = LowPowerRadioCommsSpecMessage::read(
			id,
			id_size,
			2, /* 2 bytes for Group ID. */
			0,
			false
		);
		this->attr_id = LowPowerRadioCommsSpecMessage::read(
			id,
			id_size,
			2, /* 2 bytes for Attribute ID. */
			2,
			false
		);
		this->attr_number = LowPowerRadioCommsSpecMessage::read(
			id,
			id_size,
			2, /* 12-bits for Attribute Number. */
			4,
			false
		);
		} catch (std::out_of_range oor) {
			std::cout << id_size << std::endl;
		}
		this->attr_number = this->attr_number >> 4; /* trim to 12-bits */
	}	
	
	char FeedValueNodeRequest::get_type() {
		return TYPE;
	}
	
	bool FeedValueNodeRequest::get_message(char* &message, size_t &size) {
		using namespace cat::snsrgtwy::radio;
		
		DataVector dv;
		/* Format the message as a DataVector. */
		dv.push_back(Data::to_data_std(this->node_id, Data::U_SHORT));
		dv.push_back(Data::to_data_std(this->group_id, Data::U_SHORT));
		dv.push_back(Data::to_data_std(this->attr_id, Data::U_SHORT));
		dv.push_back(Data::to_data_std(this->attr_number, Data::U_SHORT));
				
		/* Convert the DataVector to an array of bytes. */
		size = dv.get_bytes();
		message =  new char[size]();
		dv.to_data_message(message, 0x00);
		
		return true;
	}
	
}}}}

