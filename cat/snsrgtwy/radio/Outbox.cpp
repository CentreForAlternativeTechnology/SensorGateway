#include <cstdint>
#include <stdexcept>
#include <utility>
#include <map>
#include <vector>
#include <cat/snsrgtwy/web/server/NodeRequest.hpp>
#include <cat/snsrgtwy/radio/Outbox.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	void Outbox::delete_all(uint16_t address) {
		messages_t messages = this->map.at(address);
		for (size_t i = 0; i < messages.size(); i++) {
			delete messages.back();
			messages.pop_back();
		}
	}
	
	void Outbox::push_back_at(uint16_t address, web::server::NodeRequest* message) {
		try {
			this->map.at(address);
		} catch (std::out_of_range oor) {
			this->map[address] = std::vector<web::server::NodeRequest*>();
		}
		this->map[address].push_back(message);
	}

	
	void Outbox::send_all() {
		
		uint16_t address;
		messages_t msgs;
		
		for (std::map<uint16_t, messages_t>::iterator i = this->map.begin(); i != this->map.end(); i++) {
			
			address = i->first;
			msgs = i->second;
			
			for (messages_t::iterator j = msgs.begin(); j != msgs.end(); j++) {
				(*j)->send(address);
			}
		}
	}

}}}

