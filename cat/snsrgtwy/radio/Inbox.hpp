#ifndef INBOX_HPP
#define INBOX_HPP

#include <cstdint>
#include <string>
#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/Mailbox.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	class Inbox : public Mailbox<messages::Message*> {
		public:
			void delete_all(uint16_t from_node);
			void push_back_at(uint16_t from_node, messages::Message*);
			/**
			 * Returns all messages from a particular address as a string
			 * representing them as an array JSON objects.
			 */
			std::string to_json(uint16_t from_node) const;
	};

}}}

#endif
