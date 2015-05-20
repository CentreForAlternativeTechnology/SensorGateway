#include <cstddef>
#include <cstdint>

#include <RF24Network/RF24Network.h>

#include <cat/snsrgtwy/DiffieHellman.hpp>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>
#include <cat/snsrgtwy/radio/messages/KeyExchangeMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	size_t KeyExchangeMessage::expected_total_data_items() const {
		return 1;
	}
	
	DataVector KeyExchangeMessage::get_response_message_as_data_vector() {
		
		char public_key_bytes[Data::MAX_SIZE];
		DiffieHellman dh;
		Data data;
		DataVector dv;
			
		if (this->public_number == NULL) {
			throw CorruptMessageException("public number not yet determined");
		}
		
		/* Convert the public number from a string to a byte array. */
		Data::to_byte_array(public_key_bytes, this->public_number);
		/* Convert byte array to Data object. */
		data = Data::to_data_big(public_key_bytes, Data::U_LONG_LONG);
		/* Determine the response message and size via a DataVector. */
		dv.push_back(data);
		
		return dv;
	}
	
	void KeyExchangeMessage::before_respond_to(RF24NetworkHeader header) {
		
		Data data;
		DiffieHellman dh;
		char node_public_number[(Data::MAX_SIZE*2)+1];
		const char* random;
		const char* shared_key;
		
		/* Extract the address for future use. */
		this->address = NodeDatabase::to_string(header.from_node);
		
		/* Call parent implementation (this should interpret the message). */
		DataMessage::before_respond_to(header);
		
		/* Ensure that a key was sent in the correct format. */
		data = this->get_data_item_at_of_size(0, Data::U_LONG_LONG);
		
		/* Convert the node's public number to string for use in DH. */
		Data::to_hexadecimal_string(
			node_public_number,
			data.string,
			data.total_bytes(),
			true
		); 
		
		/* Calculate the public number to send as part of the key exchange. */
		random = dh.generate_random();
		this->public_number = dh.calculate_with_default_all(
			random
		);
		
		/* Calculate and store the shared key in the database for AES-128. */
		shared_key = dh.calculate_with_default_modulus(
			node_public_number,
			random
		);
		
		this->node_db->set_shared_key(
			this->address.c_str(),
			shared_key
		);
		
		/* Cleanup results from DiffieHellman. */
		delete[] random;
		delete[] shared_key;
	}
	
	KeyExchangeMessage::~KeyExchangeMessage() {
		delete this->node_db;
		if (this->public_number != NULL) {
			delete[] this->public_number;
		}
	}
}}}}

