#ifndef API_HPP
#define API_HPP

#include <cstdint>
#include <map>
#include <cat/snsrgtwy/web/http/Response.hpp>

namespace cat { namespace snsrgtwy { namespace web {
	
	/**
	 * An abstraction of a web API.
	 */
	class Api {
		public:
			typedef std::map<std::string, std::string> query_param_list_t;
			Api(const char destination[], bool ssl);
			/**
			 * Converts a list of query parameters to a query string.
			 * @param params the list of parameters to convert
			 * @return the equivalent query string
			 */
			static std::string to_query_string(query_param_list_t params);
			/**
			 * Sends a HTTP request to the given resource with the given query
			 * string parameters.
			 * @param resource the url of the destination
			 * @param params the query string parameters to append to the url
			 * @return the response to the request
			 */
			http::Response request(const char resource[], query_param_list_t params);
		protected:
			/**
			 * The root of the API (e.g. "127.0.0.1/api")
			 */
			const char* destination;
			/**
			 * Whether or not SSL is enabled.
			 */
			bool ssl;
	};
}}}

#endif
