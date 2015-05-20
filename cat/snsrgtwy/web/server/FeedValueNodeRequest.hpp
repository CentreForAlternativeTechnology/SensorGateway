#ifndef FEEDNODEREQUEST_HPP
#define FEEDNODEREQUEST_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <cat/snsrgtwy/web/server/NodeRequest.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	class FeedValueNodeRequest : public NodeRequest {
		public:
			FeedValueNodeRequest(std::string node, const char id[], size_t id_size);
		protected:
			static const char TYPE;
			
			unsigned short node_id;
			unsigned short group_id;
			unsigned short attr_id;
			unsigned short attr_number;
			
			virtual char get_type();
			virtual bool get_message(char* &message, size_t &size);
	};
}}}}

#endif

