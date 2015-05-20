#ifndef SERVER_HPP
#define SERVER_HPP

#include <microhttpd.h>
#include <cstddef>
#include <string>
#include <map>
#include <cat/snsrgtwy/radio/Inbox.hpp>
#include <cat/snsrgtwy/radio/Outbox.hpp>
#include <cat/snsrgtwy/web/http/Response.hpp>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	
	class Server {
		public:
			typedef std::map<std::string, std::string> query_t;
			typedef std::pair<radio::Inbox*, radio::Outbox*> mailboxes_t;
			static const unsigned short PORT;
			static uint16_t determine_address(const char* emoncms_id);
			static query_t get_parameters(MHD_Connection* connection);
			static http::Response feed_value(
				query_t query,
				radio::Inbox* inbox,
				radio::Outbox* outbox
			);
			Server(
				radio::Inbox* inbox,
				radio::Outbox* outbox,
				const char* key_filename,
				const char* cert_filename
			);
			void run();
		protected:
			static const size_t DELAY;
			
			radio::Inbox* inbox;
			radio::Outbox* outbox;
			const char* key_filename;
			const char* cert_filename;
			
			static size_t get_file_size(const char* filename);
			static char* load_file(const char* filename);
			static char* load_file_or_throw_exception(const char* filename);
	};
	
}}}}

#endif
