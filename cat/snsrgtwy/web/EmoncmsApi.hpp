#ifndef EMONCMSAPI_HPP
#define EMONCMSAPI_HPP

#include <cstdint>
#include <map>
#include <cat/snsrgtwy/web/Api.hpp>
#include <cat/snsrgtwy/web/http/Response.hpp>

namespace cat { namespace snsrgtwy { namespace web {
	
	class EmoncmsApi : protected Api {
		public:
			EmoncmsApi(
				const char destination[],
				const char api_key[]
			) : Api(destination, true) {
				this->api_key = std::string(api_key);
			}
			unsigned short register_node(
				uint16_t node,
				unsigned int timeout
			);
			void register_attr(
				unsigned short node,
				unsigned short groupid,
				unsigned short attrid,
				unsigned short attr_number,
				const char attr_default_value[],
				unsigned int timeout
			);
			void post(
				unsigned short node,
				unsigned short groupid,
				unsigned short attrid,
				unsigned short attr_number,
				const char attr_value[],
				unsigned int timeout
			);
		protected:
			std::string api_key;
			static std::string to_query_string(query_param_list_t params);
			void attr(
				const char resource[],
				unsigned short node,
				unsigned short groupid,
				unsigned short attrid,
				unsigned short attr_number,
				const char attr_value[],
				unsigned int timeout
			);
	};
}}}

#endif
