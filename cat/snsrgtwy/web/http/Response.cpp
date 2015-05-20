#include <string>
#include <cat/snsrgtwy/web/http/Response.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace http {
	
	Response::Response(
		unsigned short code,
		std::string body
	) {
		this->code = code;
		this->body = body;
		//this->headers = headers;
	}
	
	unsigned short Response::get_code() {
		return this->code;
	}
	
	std::string Response::get_body() {
		return this->body;
	}
	
}}}}

