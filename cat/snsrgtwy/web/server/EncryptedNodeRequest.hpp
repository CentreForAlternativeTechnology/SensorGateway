#ifndef ENCRYPTEDNODEREQUEST_HPP
#define ENCRYPTEDNODEREQUEST_HPP

#include <cstddef>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/web/server/NodeRequest.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	class EncryptedNodeRequest : public NodeRequest {
		public:
			virtual bool send(uint16_t to_node);
		protected:
			static const size_t SIZE_OF_KEY = radio::messages::EncryptedMessage::SIZE_OF_KEY;
			char shared_key[SIZE_OF_KEY];
			bool has_determined_shared_key;
			virtual bool get_message(char* &message, size_t &size);
	};
}}}}

#endif

