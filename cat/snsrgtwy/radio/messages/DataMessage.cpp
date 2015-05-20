#include <cstddef>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/messages/DataMessage.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/DataMessage.hpp
	 */
	const size_t DataMessage::INDEX_OF_TOTAL_DATA_ITEMS = 3;
	/**
	 * @see cat/snsrgtwy/radio/messages/DataMessage.hpp
	 */
	const size_t DataMessage::INDEX_OF_FIRST_DATA_ITEM = 4;
	
	/**
	 * Returns the Data from the data_items data member at the specified index.
	 * 
	 * This member function throws a CorruptMessageException if the message has
	 * not yet been interpreted (by calling the interpret() member function) or
	 * if the associated Data simply does not exist.
	 * 
	 * @param index the index to retrieve at from the data items
	 * @return the Data at the specified index
	 * @throw cat::snsrgtwy::radio::messages::CorruptMessageException
	 */
	Data DataMessage::get_data_item_at(size_t index) const {
		Data data;
		try {
			data = this->data_items.at(index);
		} catch (std::out_of_range oor) {
			throw CorruptMessageException(
				"data item does not exist at the specified index"
			);
		}
		return data;
	}
	
	Data DataMessage::get_data_item_at_of_size(
		size_t index,
		char data_size
	) const {
		Data data = this->get_data_item_at(index);
		if (data.size != data_size) {
			throw CorruptMessageException("data item is of unexpected size");
		}
		return data;
	}
	
	char DataMessage::get_response_status_code() {
		return 0x00;
	}
	
	DataVector DataMessage::get_response_message_as_data_vector() {
		DataVector dv;
		return dv;
	}
	
	bool DataMessage::get_response_message(char* &message, size_t &size) {
		DataVector dv = this->get_response_message_as_data_vector();
		size = dv.get_bytes();
		message = new char[size]();
		dv.to_data_message(
			message,
			this->get_response_status_code()
		);
		this->print(message, size);
		return true;
	}
	
	/**
	 * Reads the raw message in order to verify the message type and extract
	 * the associated data items.
	 * 
	 * @throws cat::snsrgtwy::radio::messages::DataMessageTypeException 
	 * @throws cat::snsrgtwy::radio::messages::DataMessageCorruptionException
	 */
	void DataMessage::interpret() {
		
		const size_t SIZE_OF_LONG_LONG = Data::to_total_bytes(Data::LONG_LONG);
		
		/* The number of data items to extract from the message (assume 0). */
		char items_remaining = 0;
		/* The data type that needs to be read in the current iteration. */
		size_t current_data_size = 0;
		/* The value of the data that was read in the current iteration. */
		long current_std = 0;
		char current_big[Data::MAX_SIZE];
		/* The number of bytes that need to be read in the current iteration. */
		size_t bytes_to_read = 0;
		/* Start at the location of the first data item in the message. */
		size_t current_position = this->INDEX_OF_FIRST_DATA_ITEM;
		
		try {
			/* Attempt to retrieve the number of data items. */
			items_remaining = this->get_char_at(
				this->INDEX_OF_TOTAL_DATA_ITEMS
			);
		} catch(std::out_of_range oor) {
			/* Do nothing. */
		}

		/* Ensure that the message claims the expected number of data items. */
		if (this->expected_total_data_items() != items_remaining) {
			throw CorruptMessageException("unexpected number of data items");
		}
		
		try {
			while (items_remaining > 0) {
				Data data;
				/* Determine the current data size and move position. */
				current_data_size = this->get_char_at(current_position++);
				/* Determine the number of bytes to read. */
				bytes_to_read = Data::to_total_bytes(current_data_size);
				
				/* If the data is no larger than a standard type, read it. */
				if (bytes_to_read != 0 && bytes_to_read < SIZE_OF_LONG_LONG) {
					current_std = LowPowerRadioCommsSpecMessage::read(
						this->message,
						this->size,
						bytes_to_read,
						current_position,
						true
					);
					data = Data::to_data_std(
						current_std,
						current_data_size
					);
				/* Otherwise, extract the substring from the message. */
				} else {
					/* If the data is a string, determine its length. */
					if (bytes_to_read == 0) {
						current_data_size = this->get_char_at(
							current_position++
						);
						bytes_to_read = Data::to_total_bytes(current_data_size);
					}
					/* If overflow will occur, prevent it. */
					if (current_position + bytes_to_read > this->size) {
						throw std::out_of_range(
							"data item value extends beyond message size"
						);
					}
					memcpy(
						current_big,
						&this->message[current_position],
						bytes_to_read
					);
					data = Data::to_data_big(
						current_big,
						current_data_size
					);
				}
				/* Assuming a successful read, move position appropriately. */
				current_position += bytes_to_read;
				/* Add the new Data to the list of data items. */
				this->data_items.push_back(data);
				items_remaining--;
			}
		} catch (std::out_of_range oor) {
			throw CorruptMessageException(
				"size of message cannot contain all data items"
			);
		}
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.hpp
	 */
	void DataMessage::before_respond_to(RF24NetworkHeader header) {
		/* Before the message is sent, interpret the one received. */
		this->interpret();
	}
	
	/**
	 * @see cat/snsrgtwy/radio/message/LowPowerRadioCommsMessage.hpp
	 */
	bool DataMessage::respond_to(RF24NetworkHeader header) {
		return LowPowerRadioCommsSpecMessage::respond_to(header);
	}
	
}}}}
