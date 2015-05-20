#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <cat/snsrgtwy/radio/RF24Handler.hpp>
#include <cat/snsrgtwy/web/server/NodeRequest.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	bool NodeRequest::send(uint16_t to_node) {
		
		bool success;
		char* message;
		size_t size;
		bool is_deletable = this->get_message(message, size);
		
		success = radio::RF24Handler::get_instance()->write(
			to_node,
			message,
			this->get_type(),
			size,
			1
		);
		
		if (is_deletable) {
			delete[] message;
		}
		
		return success;
	}

}}}}

