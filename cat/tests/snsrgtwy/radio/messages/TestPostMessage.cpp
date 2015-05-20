#include <boost/test/unit_test.hpp>
#include <cat/snsrgtwy/radio/messages/PostMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	class TestPostMessage : public PostMessage {
		public:
			TestPostMessage(char message[]) : PostMessage(message) {}
			void post() {
				this->before_respond_to(HEADER);
			}
			char status_code() {
				return this->get_response_status_code();
			}
	};
	
	BOOST_AUTO_TEST_SUITE(PostMessageTestSuite)
	
	BOOST_AUTO_TEST_CASE(should_post) {
		char msg[] = {
			0x0E, 0x00, /* Size (16 bytes) */
			0x00, /* Status (OK) */
			0x05, /* 5 Items */

			/* Node ID */
			0x05, /* unsigned short */
			0xFF, 0xFF, /* 65535 */

			/* Group ID */
			0x05, /* unsigned short */
			0x01, 0x00, /* 1 */

			/* Attribute ID */
			0x05, /* unsigned short */
			0x0F, 0x0F, /* 3855 */

			/* Attribute Number */
			0x05, /* unsigned short */
			0xF0, 0xF0, /* 61680 */

			/* Attribute Value */
			0x02, /* char */
			0x55 /* 01010101 or 'U' in utf8 */
		};
		size_t size = sizeof(msg);
		char* msg_encrypted = new char[size]();
		memcpy(msg_encrypted, msg, size);
		EncryptedMessage::convert_to_encrypted_message(
			msg_encrypted,
			size,
			SHARED_KEY
		);
		
		TestPostMessage m(msg_encrypted);
		m.post();
		BOOST_CHECK(m.status_code() == 0x00);
	}
	
	BOOST_AUTO_TEST_SUITE_END()

}}}}
