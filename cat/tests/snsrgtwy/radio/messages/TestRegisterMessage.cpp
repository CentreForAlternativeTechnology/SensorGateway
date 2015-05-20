#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	class TestRegisterMessage : public RegisterMessage {
		public:
			TestRegisterMessage(char message[]) : RegisterMessage(message) {}
			void register_node() {
				this->before_respond_to(HEADER);
			}
			
			char status_code() {
				return this->get_response_status_code();
			}
			
			unsigned short expected_id() {
				return 45;
			}
			unsigned short assigned_id() {
				return this->get_response_message_as_data_vector()
					.at(0).unsigned_short;
			}
	};
	
	BOOST_AUTO_TEST_SUITE(RegisterMessageTestSuite)
	
	BOOST_AUTO_TEST_CASE(should_assign_correct_node_id) {
		
		char msg[] = {0x00, 0x00, 0x00, 0x00};
		size_t size = sizeof(msg);
		char* msg_encrypted = new char[size]();
		memcpy(msg_encrypted, msg, size);
		EncryptedMessage::convert_to_encrypted_message(
			msg_encrypted,
			size,
			SHARED_KEY
		);
		TestRegisterMessage m(msg_encrypted);
		
		m.register_node();
		BOOST_CHECK(m.status_code() == 0x00);
		BOOST_CHECK(m.expected_id() == m.assigned_id());
	}
	
	BOOST_AUTO_TEST_SUITE_END()

}}}}
