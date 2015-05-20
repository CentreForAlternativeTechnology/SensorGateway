#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

#include <cstddef>
#include <exception>
#include <iostream>

#include <cat/snsrgtwy/SensorGateway.hpp>
#include <cat/snsrgtwy/radio/RF24Handler.hpp>
#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/messages/MessageException.hpp>

namespace cat { namespace snsrgtwy {
	using namespace cat::snsrgtwy::radio;
	
	/**
	 * @see cat/snsrgtwy/SensorGateway.hpp
	 */
	SensorGateway::SensorGateway() {
		
	}
	
	Inbox* SensorGateway::get_inbox() {
		return &(this->inbox);
	}
	
	Outbox* SensorGateway::get_outbox() {
		return &(this->outbox);
	}
	
	/**
	 * @see cat/snsrgtwy/SensorGateway.hpp
	 */
	void SensorGateway::handle_incoming_message() {
		using namespace cat::snsrgtwy::radio::messages;
		
		RF24NetworkHeader header;
		Message* message = NULL;
		bool keep_message_alive = false;
			
		try {
			/* Create an abstraction of the message type */
			message = RF24Handler::get_instance()->read(header);
			
			/* Respond to the message if the type was determined. */
			if (message) {
				/* If the message is a response, add it to the inbox. */
				if (!message->respond_to(header)) {
					this->inbox.push_back_at(header.from_node, message);
					keep_message_alive = true;
				}	
			}
		} catch (MessageException me) {
			std::cout << "error: " << me.what() << std::endl;
		}
			
		/* Once we are finished with the message, delete it. */
		if (message != NULL && !keep_message_alive) {
			delete message;
		}
	}
	
	/**
	 * @see cat/snsrgtwy/SensorGateway.hpp
	 */
	void SensorGateway::send_outgoing_messages() {
		this->outbox.send_all();
	}
	
	/**
	 * Runs the service.
	 * 
	 * This member function catches MessageException errors thrown by the
	 * factory and individual messages and report them before continuing to
	 * respond to messages.
	 */
	void SensorGateway::run() {
		
		while(true) {
			this->handle_incoming_message();
			this->send_outgoing_messages();
		}
	}
	
	/**
	 * @see cat/snsrgtwy/SensorGateway.hpp
	 */
	SensorGateway::~SensorGateway() {
		
	}
}}

#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/web/http/RequestSslConfig.hpp>
#include <cat/snsrgtwy/web/server/Server.hpp>

int main(int argc, char* argv[]) {
	if (argc < 6) {
		std::cout << "Usage: sensor-gateway"
			<< " [EMONCMS DESTINATION]"
			<< " [EMONCMS APIKEY]"
			<< " [EMONCMS CERTIFICATE PATH]"
			<< " [GATEWAY KEY PATH]"
			<< " [GATEWAY CERTIFICATE PATH]"
			<< std::endl
		;
		std::cout << "Example: sensor-gateway"
			<< " 127.0.0.1:433/emoncms"
			<< " 265239"
			<< " ./emoncms/127.0.0.2.cert"
			<< " ./127.0.0.1.cert"
			<< " ./127.0.0.1.key"
			<< std::endl;
		;
	} else {
		cat::snsrgtwy::web::EmoncmsApiConfig::set_destination(argv[1]); // 127.0.0.1:443/emoncms
		cat::snsrgtwy::web::EmoncmsApiConfig::set_api_key(argv[2]); 
		cat::snsrgtwy::web::http::RequestSslConfig::set_certificate_filename(argv[3]); ///home/pi/projects/emoncms/127.0.0.1.cert
		cat::snsrgtwy::radio::RF24Handler::get_instance();
		cat::snsrgtwy::SensorGateway sg;
		cat::snsrgtwy::web::server::Server server(
			sg.get_inbox(),
			sg.get_outbox(),
			argv[4],
			argv[5]
		);
		//server.run();
		sg.run();
	}
	return 0;
}

