#ifndef MESSAGEFACTORY_HPP
#define MESSAGEFACTORY_HPP

#include <cat/snsrgtwy/radio/messages/Message.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * Constructs objects derived from the Message base class according to the
	 * contents of raw messages.
     */
	class MessageFactory {
		public:
			/**
			 * Creates a child of Message on the heap according to the
			 * supplied message and returns a pointer to it.
			 * @param type the message type
			 * @param message the raw message
             * @return a pointer to the newly create Message object
             */
			messages::Message* create_message(char type, char message[]);
	};
	
}}}

#endif
