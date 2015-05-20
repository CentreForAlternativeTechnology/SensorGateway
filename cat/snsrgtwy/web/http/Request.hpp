#pragma once

#include <string>
#include <map>

#include <cat/snsrgtwy/web/http/Response.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace http {

	class Request {
		public:
			typedef std::map<std::string, std::string> param_list_t;
			
			static const unsigned char METHOD_HEAD;
			static const unsigned char METHOD_OPTIONS;
			static const unsigned char METHOD_GET;
			static const unsigned char METHOD_POST;
			static const unsigned char METHOD_PUT;
		    static const unsigned char METHOD_PATCH;
			static const unsigned char METHOD_DELETE;
			
			Request(std::string destination);
			/*
			Request(
				std::string destination,
				char method,
				param_list_t params
			);
			*/
			Response send(bool ssl);
		
		protected:
			std::string destination;
			char method;
			param_list_t params;
			static char normalize_method(char method);
	};
}}}}
