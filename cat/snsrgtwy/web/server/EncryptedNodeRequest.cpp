#include <cstddef>
#include <cstdint>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/radio/messages/EncryptedMessageException.hpp>
#include <cat/snsrgtwy/web/server/EncryptedNodeRequestException.hpp>
#include <cat/snsrgtwy/web/server/EncryptedNodeRequest.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	bool EncryptedNodeRequest::send(uint16_t to_node) {
		using namespace cat::snsrgtwy::radio::messages;
		
		try {
			EncryptedMessage::determine_shared_key_for(
				this->shared_key,
				to_node
			);
			this->has_determined_shared_key = true;
		} catch (EncryptedMessageException eme) {
			throw EncryptedNodeRequestException(eme.what());
		}
		
		return NodeRequest::send(to_node);
	}
	
	bool EncryptedNodeRequest::get_message(char* &message, size_t &size) {
		using namespace cat::snsrgtwy::radio::messages;
		
		if (!this->has_determined_shared_key) {
			throw EncryptedNodeRequestException(
				"shared key has not yet been determined"
			);
		}
		
		EncryptedMessage::convert_to_encrypted_message(
			message,
			size,
			this->shared_key
		); 
		
		return true;
	}

}}}}

