#ifndef DATAMESSAGE_HPP
#define	DATAMESSAGE_HPP

#include <cstddef>
#include <vector>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/DataVector.hpp>
#include <cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstraction of radio messages received from sensor nodes as defined
	 * in the Low Power Radio To Gateway Communications Specification.
	 * 
	 * @param message the raw content of the message represented as an array of
	 * chars
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class DataMessage : public LowPowerRadioCommsSpecMessage {
		public:
			/**
			 * Constructs an abstraction of a radio message received from a
			 * sensor node as defined in the Low Power Radio To Gateway
			 * Communications Specification.
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			DataMessage(char message[]) : LowPowerRadioCommsSpecMessage(
				message
			) {
				
			}
			/**
			 * Returns the total number of data items expected in the message.
			 * 
			 * It is intended for the result of this member function to be
			 * verified by the interpret() member function. If the actual
			 * number of data items is not equivalent to the number that was
			 * expected, interpret should throw a CorruptMessageException.
			 * 
			 * @return the total number of data items expected in the message 
			 */
			virtual size_t expected_total_data_items() const =0;
			/**
			 * Reads the raw message that was previously specified in the
			 * constructor in order to possibly modify data members
			 * accordingly.
			 * 
			 * It is intended for this function to verify the instance of the
			 * message with the raw content of the message. If the raw content
			 * is invalid, a CorruptMessageException should be thrown.
			 * 
			 * It is also intended for this member function to be called after
			 * instantiation in order to construct the list of data (also
			 * referred to as the data items) associated with the message.
			 *  
			 * @throw cat::snsrgtwy::radio::messages::CorruptMessageException
			 */
			virtual void interpret();
			
			/**
			 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
			 */
			virtual bool respond_to(RF24NetworkHeader header);
		protected:
			/**
			 * The index of the byte in the raw message that defines the total
			 * number of data items in the message.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification defines this as index 3.
			 */
			static const size_t INDEX_OF_TOTAL_DATA_ITEMS;
			/**
			 * The index of the byte in the raw message that defines the size
			 * of the first data item in the message.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification defines this as index 4.
			 */
			static const size_t INDEX_OF_FIRST_DATA_ITEM;
			/**
			 * Converts a data size identifier to the associated number of
			 * bytes as defined by the Low Power Radio To Gateway
			 * Communications Specification
			 * 
			 * @param size a data size identifier as defined in the Low Power
			 * Radio To Gateway Communications Specification 
			 * @return the number of bytes associated with the given data size
			 * identifier as defined in the Low Power Radio To Gateway
			 * Communications Specification 
			 */
			static size_t to_bytes(char data_size);
			
			/**
			 * The data items associated with the message.
			 * 
			 * It is intended for this list to be empty until the interpret()
			 * member function is called.
			 */
			DataVector data_items;
			
			/**
			 * Returns the Data from the data_items data member at the
			 * specified index.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptMessageException if the message has not yet been
			 * interpreted (by calling the interpret() member function) or if
			 * the associated Data simply does not exist in the raw message.
			 * 
			 * @param index the index to retrieve at from the data items
			 * @return the Data at the specified index
			 * @throw cat::snsrgtwy::radio::messages::CorruptMessageException
			 */
			Data get_data_item_at(size_t index) const;
			
			/**
			 * Returns the Data from the data_items data member at the
			 * specified index, but only if it is of the specified data size.
			 * 
			 * It is intended for this member function th throw a
			 * CorruptMessageException if the message has not yet been
			 * interpreted (by calling the interpret() member function), if 
			 * the associated Data simply does not exist in the raw message or 
			 * if the associated Data is not of the specifed data size.
			 * 
			 * @param index the index to retrieve at from the data items
			 * @param data_size the expected data size
			 * @return the Data at the specified index
			 * @throw cat::snsrgtwy::radio::messages::CorruptMessageException 
			 */
			Data get_data_item_at_of_size(size_t index, char data_size) const;
			
			virtual char get_response_status_code();
			virtual DataVector get_response_message_as_data_vector();
			
			virtual bool get_response_message(char* &message, size_t &size);
			
			/**
			 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
			 */
			void before_respond_to(RF24NetworkHeader header);
	};

}}}}

#endif

