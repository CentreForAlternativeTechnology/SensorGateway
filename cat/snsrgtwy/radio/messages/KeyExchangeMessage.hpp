#ifndef KEYEXCHANGEMESSAGE_HPP
#define KEYEXCHANGEMESSAGE_HPP

#include <cstddef>
#include <cstdint>

#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/messages/DataMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {

	class KeyExchangeMessage : public DataMessage {
		public:
			static const char TYPE = 'X';
			static const size_t SIZE_OF_KEY = Data::MAX_SIZE;
			KeyExchangeMessage(char message[]) : DataMessage(message) {
				this->public_number = NULL;
				this->node_db = NodeDatabaseCreator::create();
			}
			virtual size_t expected_total_data_items() const;
			virtual DataVector get_response_message_as_data_vector();
			virtual void before_respond_to(RF24NetworkHeader header);
			virtual ~KeyExchangeMessage();
		protected:
			NodeDatabase* node_db;
			std::string address;
			const char* public_number;
	};
}}}}

#endif

