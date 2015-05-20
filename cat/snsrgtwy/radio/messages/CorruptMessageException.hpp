#ifndef CORRUPTMESSAGEEXCEPTION_HPP
#define	CORRUPTMESSAGEEXCEPTION_HPP

#include <stdexcept>
#include <cat/snsrgtwy/radio/messages/MessageException.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {

	class CorruptMessageException : public MessageException {
		public:
			CorruptMessageException(const char* what) : MessageException(what){}
	};

}}}}

#endif
