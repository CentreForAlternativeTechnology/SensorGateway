#include <cstddef>
#include <string>
#include <cat/snsrgtwy/web/http/RequestSslConfigException.hpp>
#include <cat/snsrgtwy/web/http/RequestSslConfig.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace http {

	std::string RequestSslConfig::cert_filename = "";
	
	void RequestSslConfig::set_certificate_filename(const char filename[]) {
		cert_filename = std::string(filename);
	}
	
	std::string RequestSslConfig::get_certificate_filename() {
		if (cert_filename == "") {
			throw RequestSslConfigException();
		}
		return cert_filename;
	}
	
}}}}

