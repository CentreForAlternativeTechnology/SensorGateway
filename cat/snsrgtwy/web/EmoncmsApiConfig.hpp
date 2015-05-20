#ifndef EMONCMSAPICONFIG_HPP
#define EMONCMSAPICONFIG_HPP

#include <cstdint>
#include <map>
#include <cat/snsrgtwy/web/Api.hpp>
#include <cat/snsrgtwy/web/EmoncmsApi.hpp>

namespace cat { namespace snsrgtwy { namespace web {
	
	/**
	 * An abstraction of the EMonCMS API.
	 */
	class EmoncmsApiConfig : protected Api {
		public:
			/**
			 * Set the location of the EMonCMS API (e.g. 127.0.0.1/emoncms/)
			 * for all copies of EmoncmsApi.
			 * @param destination the location of the EMonCMS API.
			 */
			static void set_destination(const char destination[]);
			/**
			 * Set the API key for all requests to the EMonCMS API.
			 * @param api_key the API key for all requests to the EMonCMS API
			 */
			static void set_api_key(const char api_key[]);
			/**
			 * Returns a standardised instance of EmoncmsApi.
			 * @return a standardised instance of EmoncmsApi
			 */
			static EmoncmsApi get_copy();
		protected:
			/**
			 * The destination to use for all EmoncmsAPI copies.
			 */
			static std::string destination;
			/**
			 * The API key to use for all EmoncmsAPI copies.
			 */
			static std::string api_key;
	};
}}}

#endif
