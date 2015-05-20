#ifndef RF24HANDLER_HPP
#define RF24HANDLER_HPP

#include <cstddef>
#include <cstdint>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>
#include <cat/snsrgtwy/radio/messages/Message.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * A singleton handling the receipt and transmission of radio messages
	 * using nRF24L01+ modules.
	 */
	class RF24Handler {
		public:
			static const uint8_t CE_PIN;
			static const uint8_t CS_PIN;
			static const uint32_t SPI_SPEED;
			
			/**
			 * Returns the instance of RF24Handler.
			 * @return the instance of RF24Handler.
			 */
			static RF24Handler* get_instance();
			
			/**
			 * Reads and returns a single message from the network.
			 * @param header a reference to be assigned the request header
			 * @return the incoming message from the network
			 */
			messages::Message* read(RF24NetworkHeader &header);
			/**
			 * Writes a message to the radio network.
			 * @param to_node the destination fo the message
			 * @param message the contents of the message to be sent
			 * @param type the type of the message being sent
			 * @param the size of the message being sent
			 * @param the number of attempts to undertake when sending the
			 * message
			 * @return whether or not the message was transmitted successfully
			 */
			bool write(
				uint16_t to_node,
				char message[],
				char type,
				size_t size,
				size_t attempts
			);
		protected:
			static RF24Handler* instance;
			
			RF24* radio;
			RF24Network* network;
			RF24Mesh* mesh;
			
			RF24Handler();
			void mesh_update_and_dhcp();
			~RF24Handler();
	};
}}}

#endif

