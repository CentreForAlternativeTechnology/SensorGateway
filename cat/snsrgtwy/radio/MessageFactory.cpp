#include <cat/snsrgtwy/radio/MessageFactory.hpp>
#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/messages/KeyExchangeMessage.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterMessage.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterAttrMessage.hpp>
#include <cat/snsrgtwy/radio/messages/PostMessage.hpp>
#include <cat/snsrgtwy/radio/messages/FeedValueMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/MessageFactory.hpp
     */
	messages::Message* MessageFactory::create_message(char type, char message[]) {
		using namespace cat::snsrgtwy::radio::messages;
		Message* m = 0;
		switch (type) {
			case KeyExchangeMessage::TYPE:
				m = new KeyExchangeMessage(message);
				break;
			case RegisterMessage::TYPE:
				m = new RegisterMessage(message);
				break;
			case RegisterAttrMessage::TYPE:
				m = new RegisterAttrMessage(message);
				break;
			case PostMessage::TYPE:
				m = new PostMessage(message);
				break;
			case FeedValueMessage::TYPE:
				m = new FeedValueMessage(message);
				break;
		}
		return m;
	}

}}}
