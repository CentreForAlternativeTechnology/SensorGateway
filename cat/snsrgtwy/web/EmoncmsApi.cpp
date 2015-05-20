#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiException.hpp>
#include <cat/snsrgtwy/web/http/Response.hpp>

namespace cat { namespace snsrgtwy { namespace web {
	
	unsigned short EmoncmsApi::register_node(
		uint16_t node,
		unsigned int timeout = 0
	) {
		unsigned short id;
		std::string response_body;
		query_param_list_t params;
		
		params["apikey"] = this->api_key;
		params["nodeip"] = "" + node;
		
		if (timeout > 0) {
			params["timeout"] = timeout;
		}
		
		http::Response response = this->request(
			"register/register.json",
			params
		);
		response_body = response.get_body();
		EmoncmsApiException eae(response_body.c_str());
		
		switch (response.get_code()) {
			case 200:
				try {
					id = std::stoi(response_body);
				} catch (std::invalid_argument ia) {
					throw eae;
				} catch (std::out_of_range oor) {
					throw eae;
				}
				break;
			default:
				throw eae;
				break;
		}
		
		return id;
	}
	
	void EmoncmsApi::attr(
		const char resource[],
		unsigned short node,
		unsigned short groupid,
		unsigned short attrid,
		unsigned short attr_number,
		const char attr_value[],
		unsigned int timeout = 0
	) {
		std::string response_body;
		query_param_list_t params;
		std::stringstream json;
		
		params["apikey"] = this->api_key;
		params["node"] = node;
		json << "{"
			<< groupid << attrid << attr_number << ","
			<< attr_value
			<< "}";
		params["json"] = json.str();
		
		if (timeout > 0) {
			params["timeout"] = timeout;
		}
		
		http::Response response = this->request(resource, params);
		response_body = response.get_body();
		EmoncmsApiException eae(response_body.c_str());
		switch (response.get_code()) {
			case 200:
				if (response_body != "ok") {
					throw eae;
				}
				break;
			default:
				throw eae;
				break;
		}
	}
	
	void EmoncmsApi::register_attr(
		unsigned short node,
		unsigned short groupid,
		unsigned short attrid,
		unsigned short attr_number,
		const char attr_default_value[],
		unsigned int timeout = 0
	) {
		this->attr(
			"register/setup.json",
			node,
			groupid,
			attrid,
			attr_number,
			attr_default_value,
			timeout
		);
	}
	
	void EmoncmsApi::post(
		unsigned short node,
		unsigned short groupid,
		unsigned short attrid,
		unsigned short attr_number,
		const char attr_value[],
		unsigned int timeout = 0
	) {
		
		this->attr(
			"input/post.json",
			node,
			groupid,
			attrid,
			attr_number,
			attr_value,
			timeout
		);
	}
	
	std::string EmoncmsApi::to_query_string(query_param_list_t params) {
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

