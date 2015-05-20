#include <cstddef>
#include <string>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfigException.hpp>

namespace cat { namespace snsrgtwy { namespace web {
	
	std::string EmoncmsApiConfig::destination = "";
	std::string EmoncmsApiConfig::api_key = "";
	
	void EmoncmsApiConfig::set_destination(const char destination[]) {
		EmoncmsApiConfig::destination = std::string(destination);
	}
	
	void EmoncmsApiConfig::set_api_key(const char api_key[]) {
		EmoncmsApiConfig::api_key = std::string(api_key);
	}
	
	EmoncmsApi EmoncmsApiConfig::get_copy() {
		if (
			EmoncmsApiConfig::destination == ""
			|| EmoncmsApiConfig::api_key == ""
		) {
			throw EmoncmsApiConfigException();
		}
		EmoncmsApi api(EmoncmsApiConfig::destination.c_str(), EmoncmsApiConfig::api_key.c_str());
		return api;
	}
	
}}}

