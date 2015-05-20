#include <string>
#include <curl/curl.h>
#include <iostream>

#include <cat/snsrgtwy/web/http/RequestSslConfig.hpp>
#include <cat/snsrgtwy/web/http/RequestException.hpp>
#include <cat/snsrgtwy/web/http/Request.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace http {

	const unsigned char Request::METHOD_HEAD = 0x00;
	const unsigned char Request::METHOD_OPTIONS = 0x2A;
    const unsigned char Request::METHOD_GET = 0x54;
    const unsigned char Request::METHOD_POST = 0x7E;
    const unsigned char Request::METHOD_PUT = 0xA8;
    const unsigned char Request::METHOD_PATCH = 0xD2;
    const unsigned char Request::METHOD_DELETE = 0xFC;
    
	Request::Request(
		std::string destination/*,
		char method = METHOD_GET,
		param_list_t params = param_list_t()*/
	) {
		this->destination = destination;
		/*
		this->method = this->normalize_method(method);
		this->params = params;
		*/
	}
	
	char Request::normalize_method(char method) {
		
		if (method < METHOD_OPTIONS) {
			method = METHOD_HEAD;
		} else if (method < METHOD_GET) {
			method = METHOD_OPTIONS;
		} else if ( method < METHOD_POST) {
			method = METHOD_GET;
		} else if ( method < METHOD_PUT) {
			method = METHOD_POST;
		} else if ( method < METHOD_PATCH) {
			method = METHOD_PUT;
		} else if ( method < METHOD_DELETE ) {
			method = METHOD_PATCH;
		} else {
			method = METHOD_DELETE;
		}
		
		return method;
	}
	
	size_t _curl_callback(char* ptr, size_t size, size_t nmemb, std::string** data) {
		*data = new std::string(ptr);
		return (*data)->length();
	}
	
	Response Request::send(bool ssl) {
		CURL* curl;
		CURLcode curl_result;
		
		long code;
		std::string* body = NULL;
		
		curl = curl_easy_init();
		if(curl) {
			/* The following line is useful to uncomment for debugging. */
			/* curl_easy_setopt(curl, CURLOPT_VERBOSE, true); */
			
			/* Initialise transfer */
			curl_easy_setopt(curl, CURLOPT_URL, this->destination.c_str());
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _curl_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
			if (ssl) {
				curl_easy_setopt(
					curl,
					CURLOPT_CAINFO,
					RequestSslConfig::get_certificate_filename().c_str()
				);
			}
			
			/* Perform transfer */
			curl_result = curl_easy_perform(curl);
			if(curl_result != CURLE_OK) {
				/*TODO Handle error */
				throw RequestException(curl_easy_strerror(curl_result));
			}
			
			/* Retrieve detailed information */
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

		}
		curl_easy_cleanup(curl);
		
		Response response((unsigned short) code, *body);
		delete body;
		return response;
	}
}}}}

