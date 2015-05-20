#ifndef ENCRYPTEDNODEREQUESTEXCEPTION_HPP
#define ENCRYPTEDNODEREQUESTEXCEPTION_HPP

#include <stdexcept>
#include <cat/snsrgtwy/web/server/NodeRequestException.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace server {

	class EncryptedNodeRequestException : public NodeRequestException {
		public:
			EncryptedNodeRequestException(const char* what) : NodeRequestException(what) {}
	};

}}}}

#endif
