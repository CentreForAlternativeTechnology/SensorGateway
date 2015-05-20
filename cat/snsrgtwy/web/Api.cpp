#include <cstdint>
#include <sstream>
#include <cat/snsrgtwy/web/Api.hpp>
#include <cat/snsrgtwy/web/http/Request.hpp>

namespace cat { namespace snsrgtwy { namespace web {
	
	Api::Api(const char destination[], bool ssl = false) {
		this->destination = destination;
		this->ssl = ssl;
	}
	
	http::Response Api::request(
		const char* resource,
		query_param_list_t params = query_param_list_t()
	) {
		std::stringstream url;
		
		url << "http";
		/* If a secure connection is required, change the protocol to HTTPS. */
		if (this->ssl) {
			url << "s";
		}
		url << "://" << this->destination
			<< "/" << resource
			<< "?" << this->to_query_string(params);
		
		http::Request request(url.str());
		return request.send(this->ssl);
	}
	
	
	std::string Api::to_query_string(query_param_list_t params) {
		query_param_list_t::iterator iter;
		std::string query_string = "";
		for (iter = params.begin(); iter != params.end(); iter++) {
			if (iter != params.begin()) {
				query_string += "&";
			}
			query_string += iter->first + "=" + iter->second;
		}
		return query_string;
	}
}}}

