#ifndef SENSORGATEWAY_HPP
#define SENSORGATEWAY_HPP

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

#include <cstddef>
#include <vector>
#include <cat/snsrgtwy/radio/Inbox.hpp>
#include <cat/snsrgtwy/radio/Outbox.hpp>

namespace cat { namespace snsrgtwy {
	
	/**
	 * Provides the service of forwarding messages from sensor nodes to
	 * Emoncms. 
	 */
	class SensorGateway {
		public:
			/**
			 * Constructs a service for forwarding messages from sensor nodes
			 * to Emoncms. 
			 */
			SensorGateway();

			/**
			 * Runs the service.
			 * 
			 * It is intended for this member function to catch
			 * MessageException errors thrown by either the factory or
			 * individual messages and report them before continuing to respond
			 * to messages.
			 */
			void run();
			
			radio::Inbox* get_inbox();
			radio::Outbox* get_outbox();
			
			/**
			 * Destructs the service.
			 */
			~SensorGateway();
		protected:
			/**
			 * Messages that have been received by the gateway and
			 * did not require a response (implying that they are responses to
			 * messages previously sent from EMonCMS via the gateway).
			 */
			radio::Inbox inbox;
			
			/**
			 * Messages that should be sent to the appropriate sensors whenever
			 * possible (implying that they a messages being buffered on
			 * behalf of EMonCMS).
			 */
			radio::Outbox outbox;
			
			/**
			 * Reads a single message from the single and performs the
			 * appropriate response (if any).
			 */
			void handle_incoming_message();
			
			/**
			 * Writes all messages in the outbox of the gateway to the network.
			 */
			void send_outgoing_messages();
	};
}}

#endif

