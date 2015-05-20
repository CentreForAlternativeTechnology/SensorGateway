#include <cstddef>
#include <cstdint>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>
#include <cat/snsrgtwy/radio/MessageFactory.hpp>
#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/radio/RF24Handler.hpp>


namespace cat { namespace snsrgtwy { namespace radio {
	
	const uint8_t RF24Handler::CE_PIN = RPI_V2_GPIO_P1_15;
	const uint8_t RF24Handler::CS_PIN = BCM2835_SPI_CS0;
	const uint32_t RF24Handler::SPI_SPEED = BCM2835_SPI_SPEED_8MHZ;
	
	RF24Handler* RF24Handler::instance = NULL;
	
	RF24Handler::RF24Handler() {
		this->radio = new RF24(CE_PIN, CS_PIN, SPI_SPEED);
		this->network = new RF24Network(*this->radio);
		this->mesh = new RF24Mesh(*this->radio, *this->network);
		
		this->mesh->setNodeID(0); /* Master node. */
		this->mesh->begin();
		this->radio->setPALevel(RF24_PA_HIGH);
		this->radio->printDetails();
	}
	
	RF24Handler* RF24Handler::get_instance() {
		if (RF24Handler::instance == 0) {
			RF24Handler::instance = new RF24Handler();
		}
		return RF24Handler::instance;
	}
	
	void RF24Handler::mesh_update_and_dhcp() {
		this->mesh->update(); /* Keep network updated. */
		this->mesh->DHCP(); /* Assign addresses to sensor nodes. */
	}
	
	
	messages::Message* RF24Handler::read(RF24NetworkHeader &header) {
		using namespace cat::snsrgtwy::radio::messages;
		MessageFactory factory;
		Message* message = NULL;
		
		/* == FINAL DEMONSTRATION == */
		/*
		static const char key[] = {
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF
		};
		static size_t i = 0;
		static char types[] = {'R', 'A', 'P', 'p'};
		char msg_reg[] = {
			0x00, 0x00,
			0x00,
			0x00
		};
		char msg_attr[] = { 
			0x0E, 0x00,
			0x00,
			0x05,
        
			0x05,
			0xFF, 0xFF,
        
			0x05,
			0x01, 0x00,
			
			0x05,
			0x0F, 0x0F,
			
			0x05,
			0xF0, 0xF0,
			
			0x02,
			0x55
		};
		char* raw_message;
		size_t size;
		if (i >= sizeof(types)) {
			i = 0;
		}
		switch (types[i]) {
			case 'R':
				size = sizeof(msg_reg);
				raw_message = new char[size]();
				memcpy(raw_message, &(msg_reg[0]), size);
				break;
			case 'A':
			case 'P':
			case 'p':
			default:
				size = sizeof(msg_attr);
				raw_message = new char[size]();
				memcpy(raw_message, &(msg_attr[0]), size);
				break;
		}
#ifdef ENCRYPTION
		EncryptedMessage::convert_to_encrypted_message(raw_message, size, key);
#endif
		message = factory.create_message(
			types[i],
			raw_message
		);
		header.from_node = 1;
		header.to_node = 0;
		header.type = types[i];
		i++;
		sleep(7);
		*/
		/* ========================= */
		
		char raw_message[Message::MAX_SIZE] = {};
		
		this->mesh_update_and_dhcp();
		
		/* Check for incoming data from the sensors. */
		while(this->network->available() && message == NULL) {
			
			/* Determine the contents of the header. */
			this->network->peek(header);
			/* Determine the contents of the body. */
			this->network->read(
				header,
				raw_message,
				Message::MAX_SIZE
			);
			/* Create an abstraction of the message type. */
			message = factory.create_message(
				header.type,
				raw_message
			);
		}
		return message;
	}
	
	bool RF24Handler::write(
		uint16_t to_node,
		char message[],
		char type,
		size_t size,
		size_t attempts
	) {
		bool success = false;
		size_t attempt = 0;
		
		/* == FINAL DEMONSTRATION == */
		//success = true;
		/* ========================= */
		
		this->mesh_update_and_dhcp();
		
		while ((attempts == 0 || attempt < attempts) && !success) {
			success = this->mesh->write(to_node, message, type, size);
			if (!success && !this->mesh->checkConnection()) {
					this->mesh->renewAddress();
			}
			attempt++;
		}
		
		return success;
	}
			
	RF24Handler::~RF24Handler() {
		delete this->mesh;
		delete this->network;
		delete this->radio;
	}
}}}

