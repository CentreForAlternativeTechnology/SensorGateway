#pragma once
#include <string>
#include <map>

namespace cat { namespace snsrgtwy { namespace web { namespace http {
	
	class Response {
		public:
			//typedef std::map<std::string, std::string> header_list_t;
			Response(
				//header_list_t headers,
				unsigned short code,
				std::string body
			);
			//header_list_t get_headers();
			unsigned short get_code();
			std::string get_body();
		protected:
			//header_list_t headers;
			unsigned short code;
			std::string body;
	};
}}}}
