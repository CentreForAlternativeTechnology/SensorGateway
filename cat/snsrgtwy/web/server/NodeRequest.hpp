#ifndef NODEREQUEST_HPP
#define NODEREQUEST_HPP

#include <cstddef>
#include <cstdint>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	class NodeRequest {
		public:
			virtual bool send(uint16_t node);
		protected:
			virtual char get_type()=0;
			virtual bool get_message(char* &message, size_t &size)=0;
	};
}}}}

#endif

