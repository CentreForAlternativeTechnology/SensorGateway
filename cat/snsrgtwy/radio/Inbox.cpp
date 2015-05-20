#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cat/snsrgtwy/radio/Inbox.hpp>
#include <cat/snsrgtwy/radio/messages/Message.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	void Inbox::delete_all(uint16_t address) {
		messages_t messages = this->map.at(address);
		for (size_t i = 0; i < messages.size(); i++) {
			delete messages.back();
			messages.pop_back();
		}
	}
	
	void Inbox::push_back_at(uint16_t address, messages::Message* message) {
		try {
			this->map.at(address);
		} catch (std::out_of_range oor) {
			this->map[address] = std::vector<messages::Message*>();
		}
		this->map[address].push_back(message);
	}
	
	std::string Inbox::to_json(uint16_t from_node) const {
		
		std::stringstream ss;
		messages_t messages = this->map.at(from_node);
		size_t size_minus_one = messages.size()-1;
		
		ss << "[";
		
		for (size_t i = 0; i < size_minus_one; i++) {
			ss << (messages.at(i))->to_json() << ",";
		}
		ss << (messages.at(size_minus_one))->to_json();
		
		ss << "]";
		
		return ss.str();
	}
}}}

