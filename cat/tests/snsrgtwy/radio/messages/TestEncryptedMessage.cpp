#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/messages/KeyExchangeMessage.hpp>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>

#include <cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	static const size_t INDEX_OF_CIPHER = 1;
	
	class TestEncryptedMessage : public EncryptedMessage {
		public:
			
			TestEncryptedMessage(char message[]) : EncryptedMessage(
				message
			) {
				
			}
			
			void decrypt_and_interpret(RF24NetworkHeader header) {
				EncryptedMessage::before_respond_to(header);
			}
			
			bool encrypt(char* &message, size_t &size) {
				 return this->get_response_message(message, size);
			}
			
			size_t expected_total_data_items() const {
				return 2;
			}
			
			Data get(size_t index) {
				return this->get_data_item_at(index);
			}
	};
	
	BOOST_AUTO_TEST_SUITE(EncryptedMessageTestSuite)
	
	BOOST_AUTO_TEST_CASE(should_decrypt_and_interpret) {
		DiffieHellman dh;
		RF24NetworkHeader header;
		std::string address;
		char* plain_msg_copy;
		char shared_key[EncryptedMessage::SIZE_OF_KEY] = {};
		NodeDatabase* db = NodeDatabaseCreator::create();
		char plain_msg[] = {
			0x13, 0x00, /* Size (19 bytes) */
			0x00, /* Status (OK) */
			0x02, /* 2 Items */
			Data::U_LONG_LONG,
				0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			Data::U_CHAR, 'X'
		};
		size_t size_of_cipher = sizeof(plain_msg);
		char* cipher_msg = new char[size_of_cipher]();
		char shared_key_string[] = "1023456789ABCDEF";
		
		/* Use an arbitrary node ID. */
		header.from_node = 2;
		
		/* Associate arbitrary shared key with node. */
		address = NodeDatabase::to_string(header.from_node);
		db->set_shared_key(address.c_str(), shared_key_string);
		
		/* Convert shared key to byte array. */
		Data::to_byte_array(shared_key, shared_key_string);
		
		/* Encrypt the plain message. */
		memcpy(cipher_msg, plain_msg, size_of_cipher);
		EncryptedMessage::convert_to_encrypted_message(
			cipher_msg,
			size_of_cipher,
			shared_key
		);
		
		/* Check that the size of the padded plain text is a multiple of 16. */
		BOOST_CHECK(size_of_cipher >= sizeof(plain_msg));
		BOOST_CHECK((size_of_cipher - INDEX_OF_CIPHER) % 16 == 0);
		
		TestEncryptedMessage* m = new TestEncryptedMessage(cipher_msg);
		/* Ensure message was decrypted and interpreted successfully. */
		m->decrypt_and_interpret(header);
		BOOST_CHECK(m->get(0).string[0] == 0x01);
		BOOST_CHECK(m->get(1).unsigned_char == 'X');
		
		/* Ensure that the message can be re-encrypted appropriately. */
		size_of_cipher = sizeof(plain_msg);
		plain_msg_copy = new char[size_of_cipher]();
		memcpy(plain_msg_copy, plain_msg, size_of_cipher);
		
		m->encrypt(plain_msg_copy, size_of_cipher);
		BOOST_CHECK(size_of_cipher >= sizeof(plain_msg));
		BOOST_CHECK((size_of_cipher - 1) % EncryptedMessage::SIZE_OF_BLOCK == 0);
		BOOST_CHECK(memcmp(plain_msg_copy, cipher_msg, size_of_cipher) == 0);
		
		delete[] cipher_msg;
		delete[] plain_msg_copy;
		delete db;
		delete m;
	}

	BOOST_AUTO_TEST_SUITE_END()
}}}}
			
