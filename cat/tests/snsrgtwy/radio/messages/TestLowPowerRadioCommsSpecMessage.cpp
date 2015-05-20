#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {

	BOOST_AUTO_TEST_SUITE(LowPowerRadioCommsSpecMessageTestSuite)
	
	const bool little_endian = true;
	
	BOOST_AUTO_TEST_CASE(should_read_successfully) {
		char bytes[] = {0x15, 0xB6, 0x00, 0x3D, 0x00}; /* 1023456789 */
		long value = LowPowerRadioCommsSpecMessage::read(
			bytes, /* char array to read */
			5, /* size of char array */
			5, /* number of bytes to read */
			0, /* start position */
			little_endian
		);
		
		BOOST_CHECK(value == 1023456789);
		
		value = LowPowerRadioCommsSpecMessage::read(
			bytes,
			5,
			1,
			4,
			little_endian
		);
		
		BOOST_CHECK(value == 0);
		
		value = LowPowerRadioCommsSpecMessage::read(
			bytes,
			5,
			0,
			0,
			little_endian
		);
		
		BOOST_CHECK(value == 0);
		
	}
	
	BOOST_AUTO_TEST_CASE(should_throw_out_of_range_error) {
		char bytes[] = {0x01, 0x02, 0x03, 0x04};
		
		BOOST_CHECK_THROW(
			LowPowerRadioCommsSpecMessage::read(
				bytes,
				4,
				4,
				5, /* start position out of range */
				little_endian
			),
			std::out_of_range
		);
		
		BOOST_CHECK_THROW(
			LowPowerRadioCommsSpecMessage::read(
				bytes,
				4,  
				5, /* bytes to read is greater than actual size */
				0,
				little_endian
			),  
			std::out_of_range
		);
		
		BOOST_CHECK_THROW(
			LowPowerRadioCommsSpecMessage::read(
				bytes,
				4,
				3, /* bytes to read is valid... */
				2, /* ...but start position causes out of range */
				little_endian
			),
			std::out_of_range
		);
		
		BOOST_CHECK_THROW(
			LowPowerRadioCommsSpecMessage::read(
				bytes,
				4,
				5, /* bytes to read is invalid... */
				-1, /* ...but invalid start position makes this valid */
				little_endian
			),
			std::out_of_range
		);
	}

	BOOST_AUTO_TEST_SUITE_END()

}}}}
