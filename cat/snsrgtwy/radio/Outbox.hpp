#ifndef OUTBOX_HPP
#define OUTBOX_HPP

#include <cstdint>
#include <cat/snsrgtwy/web/server/NodeRequest.hpp>
#include <cat/snsrgtwy/radio/Mailbox.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * Lists of messages to be transmitted to the radio network, grouped by
	 * recipient.
	 */
	class Outbox : public Mailbox<web::server::NodeRequest*> {
		public:
			void delete_all(uint16_t to_node);
			void push_back_at(uint16_t to_node, web::server::NodeRequest* message);
			void send_all();
	};

}}}

#endif
