#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>
#include <cat/snsrgtwy/radio/Inbox.hpp>
#include <cat/snsrgtwy/radio/Outbox.hpp>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/DataVector.hpp>
#include <cat/snsrgtwy/radio/RF24Handler.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseException.hpp>
#include <cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp>
#include <cat/snsrgtwy/web/http/Response.hpp>
#include <cat/snsrgtwy/web/server/FeedValueNodeRequest.hpp>
#include <cat/snsrgtwy/web/server/ServerException.hpp>
#include <cat/snsrgtwy/web/server/Server.hpp>

#include <iostream>

namespace cat { namespace snsrgtwy { namespace web { namespace server {
	using namespace cat::snsrgtwy::radio;
	using namespace cat::snsrgtwy::radio::messages;
	
	const unsigned short Server::PORT = 8888;
	const size_t Server::DELAY = 2;
	
	Server::Server(
		radio::Inbox* inbox,
		radio::Outbox* outbox,
		const char* key_filename,
		const char* cert_filename
	) {
		this->inbox = inbox;
		this->outbox = outbox;
		this->key_filename = key_filename;
		this->cert_filename = cert_filename;
	}
	
	size_t Server::get_file_size (const char *filename) {
		FILE *fp;

		fp = fopen (filename, "rb");
		if (fp) {
			size_t size;
			if ((0 != fseek (fp, 0, SEEK_END)) || (-1 == (size = ftell (fp)))) {
				size = 0;
			}
			fclose (fp);
			return size;
		} else {
			return 0;
		}
	}
	
	char* Server::load_file(const char* filename) {
		FILE *fp;
		char *buffer;
		size_t size;

		size = get_file_size (filename);
		if (size == 0) {
			return NULL;
		}
		
		fp = fopen (filename, "rb");
		if (!fp) {
			return NULL;
		}
		buffer = (char*) malloc(size);
		if (!buffer) {
			fclose (fp);
			return NULL;
		}

		if (size != fread (buffer, 1, size, fp)) {
			free (buffer);
			buffer = NULL;
		}
		fclose (fp);
		return buffer;
	}
	
	char* Server::load_file_or_throw_exception(const char* filename) {
		const char* error_suffix = ": no such file";
		char* contents = load_file(filename);
		if (contents == NULL) {
			if (filename == NULL) {
				filename = "";
			}
			throw ServerException(
				(std::string(filename) + error_suffix).c_str()
			);
		}
		return contents;
	}
	
	uint16_t Server::determine_address(const char* emoncms_id) {
		uint16_t address;
		NodeDatabase* db = NULL;
		char* address_str = NULL;
		try {
			db = NodeDatabaseCreator::create();
			address_str = db->get_address(emoncms_id);
			if (address_str == NULL) {
				address = 0;
			} else {
				address = std::atoi(address_str);
			}
		} catch (NodeDatabaseException ndbe) {
			delete db;
			throw ndbe;
		} catch (std::out_of_range oor) {
			throw NodeDatabaseException(oor.what());
		}
		
		if (db != NULL) {
			delete db;
		}
		
		if (address_str != NULL) {
			delete[] address_str;
		}
		
		return address;
	}
	
	http::Response Server::feed_value(
		query_t query,
		radio::Inbox* inbox,
		radio::Outbox* outbox
	) {
		
		int status = MHD_HTTP_NOT_FOUND;
		std::string body = "";
		
		size_t initial_inbox_size;
		FeedValueNodeRequest* request = NULL;
		
		std::string id;		
		std::string node;
		uint16_t to_node;
		time_t timeout;
		time_t now;
		time_t until;
		bool success = false;
		
		
		try {
			/* Attempt to convert query to usable types. */
			node = query["node"];
			to_node = Server::determine_address(node.c_str());
			timeout = std::stoi(query["timeout"]);
			id = query["id"]; 
			request = new FeedValueNodeRequest(node, id.c_str(), id.size());
			
			if (to_node == 0) {
				throw std::out_of_range("node does not exist");
			}
			
			/* Determine when to stop sending messages to radios. */
			now = time(NULL);
			until = now + timeout;
			
			/* If all parameters are valid, forward the request. */
			
			/* The following code causes a hardware failure on the radio.
			 * This seems to be due to writing to the network on a separate
			 * thread.
			 */
			/*
			initial_inbox_size = inbox->at(to_node)->size();
			
			while ((timeout==0 || now < until) && !success) {
				
				request->send(to_node);
				delay(Server::DELAY);
				
				if (inbox->at(to_node).size() > initial_inbox_size) {
					success = true;
				}
			}
			*/
			/* Empty the contents of the inbox for this response. */
			body = inbox->to_json(to_node);
			inbox->delete_all(to_node);
			
			/* If the node seemed to respond, indicate success. */
			if (success) {
				delete request;
				status = MHD_HTTP_OK;
			/* Otherwise, add the request to the outbox. */
			} else {
				outbox->push_back_at(to_node, request);
				status = MHD_HTTP_PROCESSING;
			}
			
		
		} catch (NodeDatabaseException ndbe) {
			delete request;
			status = MHD_HTTP_INTERNAL_SERVER_ERROR;
		} catch (std::out_of_range oor) {
			delete request;
			status = MHD_HTTP_BAD_REQUEST;
		} catch (std::exception e) {
			delete request;
			status = MHD_HTTP_INTERNAL_SERVER_ERROR;
		} 
		
		return http::Response(status, body);
	}
	
	Server::query_t Server::get_parameters(struct MHD_Connection* connection) {
		
		query_t query;
		MHD_get_connection_values(
			connection,
			MHD_GET_ARGUMENT_KIND,
			[](
				void* cls,
				enum MHD_ValueKind kind,
				const char* key,
				const char* value
			)->int {
				query_t* query = (query_t*) cls;
				/* Sanitize value if unspecified. */
				if (value == NULL) {
					value = "";
				}
				/* Add key/value pair to query. */
				query->insert(
					std::pair<std::string, std::string>(
						key, value
					)
				);
				/* Continue to iterate. */
				return MHD_YES;
			},
			&query
		);
		return query;
	}
	
	int handle_request(
		void* cls,
		struct MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t* upload_data_size,
		void** con_cls
	) {
		
		int value;
		Server::mailboxes_t* mailboxes = (Server::mailboxes_t*) cls;
		radio::Inbox* inbox = mailboxes->first;
		radio::Outbox* outbox = mailboxes->second;
		
		std::string body;
		http::Response response(MHD_HTTP_NOT_FOUND,"");
		
		Server::query_t query;
		struct MHD_Response* mhd_response;
		
		/* Extract the query string from the request. */
		query = Server::get_parameters(connection);
		
		if (strcmp(url, "/feed/value.json") == 0) {
			response = Server::feed_value(query, inbox, outbox);
		}
		/* ==== TEST ==== */
		else if (strcmp(url, "/feed/test.json") == 0) {
			const char id[] = {
				0x04, 0x03,
				0x10, 0x10,
				0x00, 0x00,
				0x00, 0x00
			};
			FeedValueNodeRequest* fvnr = new FeedValueNodeRequest("45", id, sizeof(id));
			outbox->push_back_at(45, fvnr);
			response = http::Response(200, "Added to Inbox.");
		}
		/* ============== */
		
		body = response.get_body();
		mhd_response = MHD_create_response_from_buffer(
			body.size(),
			(void*) body.c_str(),
			MHD_RESPMEM_PERSISTENT
		);
		
		value = MHD_queue_response(
			connection,
			response.get_code(),
			mhd_response
		);
		MHD_destroy_response(mhd_response);
		
		return value;
	}
	
	void Server::run() {
		struct MHD_Daemon* daemon;
		const char* key = load_file_or_throw_exception(this->key_filename);
		const char* cert = load_file_or_throw_exception(this->cert_filename);
		mailboxes_t mailboxes(this->inbox, this->outbox);
		
		daemon = MHD_start_daemon(
			MHD_USE_SELECT_INTERNALLY | MHD_USE_SSL,
			PORT, NULL, NULL,
			/* Callback function to handle requests. */	
			&handle_request,
			&mailboxes,
			MHD_OPTION_HTTPS_MEM_KEY, key,
			MHD_OPTION_HTTPS_MEM_CERT, cert,
			MHD_OPTION_END
		);
		
		if (daemon == NULL) {
			free((void*) key);
			free((void*) cert);
			throw ServerException("could not start daemon");
		}
	}
	
}}}}
/*
int main(int argc, char* argv[]) {
	cat::snsrgtwy::web::Server server(argv[1], argv[2]);
	server.run();
}
*/
