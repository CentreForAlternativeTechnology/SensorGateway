#ifndef REQUESTSSLCONFIG_HPP
#define REQUESTSSLCONFIG_HPP

#include <string>

namespace cat { namespace snsrgtwy { namespace web { namespace http {

	class RequestSslConfig {
		public:
			static void set_certificate_filename(const char filename[]);
			static std::string get_certificate_filename();
		protected:
			static std::string cert_filename;
	};
}}}}

#endif

