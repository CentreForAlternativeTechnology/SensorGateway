#include <string>
#include <boost/test/unit_test.hpp>

#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterMessage.hpp>
#include <cat/snsrgtwy/radio/messages/RegisterAttrMessage.hpp>
#include <cat/snsrgtwy/radio/messages/PostMessage.hpp>
#include <cat/snsrgtwy/radio/MessageFactory.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {

	BOOST_AUTO_TEST_SUITE(MessageFactoryTestSuite)

	Message* create_message(char type, char message[]) {
		MessageFactory mf;
		return mf.create_message(type, message);
	}

	BOOST_AUTO_TEST_CASE(should_not_create_message) {
		char invalid_type = '_';
		char invalid_msg[] = {0x00, 0x00};
		BOOST_CHECK(create_message(invalid_type, invalid_msg) == NULL);
	}

	BOOST_AUTO_TEST_CASE(should_create_post_message) {
		char raw_post_msg[] = {0x00, 0x00};
		Message* m = create_message(PostMessage::TYPE, raw_post_msg);
		BOOST_CHECK(dynamic_cast<PostMessage*>(m));
	}
	
	BOOST_AUTO_TEST_CASE(should_create_register_message) {
		char raw_reg_msg[] = {0x00, 0x00};
		Message* m = create_message(RegisterMessage::TYPE, raw_reg_msg);
		BOOST_CHECK(dynamic_cast<RegisterMessage*>(m));
	}
	
	BOOST_AUTO_TEST_CASE(should_create_register_attribute_message) {
		char raw_reg_attr_msg[] = {0x00, 0x00};
		Message* m = create_message(RegisterAttrMessage::TYPE, raw_reg_attr_msg);
		BOOST_CHECK(dynamic_cast<RegisterAttrMessage*>(m));
	}

	BOOST_AUTO_TEST_SUITE_END()

}}}}
