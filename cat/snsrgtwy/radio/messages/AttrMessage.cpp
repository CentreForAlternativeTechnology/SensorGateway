#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/messages/AttrMessage.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>


namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	const size_t AttrMessage::EXPECTED_TOTAL_DATA_ITEMS = 5;
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	const size_t AttrMessage::INDEX_OF_NODE_ID = 0;
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	const size_t AttrMessage::INDEX_OF_GROUP_ID = 1;
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	const size_t AttrMessage::INDEX_OF_ATTR_ID = 2;
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	const size_t AttrMessage::INDEX_OF_ATTR_NUMBER = 3;
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	const size_t AttrMessage::INDEX_OF_LAST_DATA_ITEM = 4;
	
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	size_t AttrMessage::expected_total_data_items() const {
		return this->EXPECTED_TOTAL_DATA_ITEMS;
	}
	
	/**
	 * Returns the unsigned short value from the data_items data member at the
	 * specified index, but only if the associated Data is of size U_SHORT.
	 * 
	 * This member function throws a DataMessageCorruptionException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function), the associated Data simply does not exist in the raw
	 * message, or the associated Data is not of size U_SHORT.
	 * 
	 * @param index the index to retrieve at from the data_items data member
	 * @return the unsigned short of Data at the specified index
	 * @throw cat::snsrgtwy::radio::messages::DataMessageCorruptionException
 	 */
	unsigned short AttrMessage::get_u_short_data_item_at(size_t index) const {
		return this->get_data_item_at_of_size(
			index, Data::U_SHORT
		).unsigned_short;
	}
	
	/**
	 * Retrieves the node ID from the message.
	 * 
	 * This member function throws a CorruptMessageException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function) or if the node ID simply does not exist in the raw
	 * message.
	 * 
	 * @return the node ID extracted from the message
	 */
	unsigned short AttrMessage::get_node_id() const {
		return this->get_u_short_data_item_at(this->INDEX_OF_NODE_ID);
	}
	
	/**
	 * Retrieves the group ID from the message.
	 * 
	 * This member function throws a CorruptMessageException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function) or if the group ID simply does not exist in the raw
	 * message.
	 * 
	 * @return the group ID extracted from the message
	 */
	unsigned short AttrMessage::get_group_id() const {
		return this->get_u_short_data_item_at(this->INDEX_OF_GROUP_ID);
	}
	
	/**
	 * Retrieves the attribute ID from the message.
	 * 
	 * This member function throws a CorruptMessageException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function) or if the attribute ID simply does not exist in the raw
	 * message.
	 * 
	 * @return the attribute ID extracted from the message
	 */
	unsigned short AttrMessage::get_attr_id() const {
		return this->get_u_short_data_item_at(this->INDEX_OF_ATTR_ID);
	}
	
	/**
	 * Retrieves the attribute number from the message.
	 * 
	 * This member function throws a CorruptMessageException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function) or if the attribute number simply does not exist in the raw
	 * message.
	 * 
	 * @return the attribute number extracted from the message
	 */
	unsigned short AttrMessage::get_attr_number() const {
		return this->get_u_short_data_item_at(this->INDEX_OF_ATTR_NUMBER);
	}
	
	/**
	 * Retrieves the data item following the attribute number from the
	 * message.
	 * 
	 * This member function throws a CorruptMessageException if the 
	 * message has not yet been interpreted (by calling the interpret() member
	 * function) or if the attribute value simply does not exist in the raw
	 * message.
	 * 
	 * @return the attribute value extracted from the message
	 */
	Data AttrMessage::get_last_data_item() const {
		return this->get_data_item_at(this->INDEX_OF_LAST_DATA_ITEM);
	}
	
	/* Assigns a pointer and a size to the given arguments respectively
	 * for a message allocated memory on the heap for use as part of
	 * the response.
	 * 
	 * This member function to simply assigns the same pointer and size for the
	 * received message on success. False is returned in order to preserve the
	 * integrity of this instance until destruction.
	 * 
	 * @param message a reference that should be assigned a pointer
	 * to a response message that has been allocated memory
	 * @param size a reference that should be assigned the size of the
	 * response message
	 * @return false
	 */
	/*
	bool AttrMessage::get_response_message(char* &message, size_t &size) {
		message = this->message;
		size = this->size;
		return false;
	}
	*/
	DataVector AttrMessage::get_response_message_as_data_vector() {
		return this->data_items;
	}
	
	/**
	 * @see cat/snsrgtwy/radio/messages/AttrMessage.hpp
	 */
	std::string AttrMessage::to_json() const {
		
		std::stringstream ss;
		ss << "{"
			<< "\"nodeid\":" << this->get_node_id() << ","
			<< "\"groupid\":" << this->get_group_id() << ","
			<< "\"attrid\":" << this->get_attr_id() << ","
			<< "\"attrno\":" << this->get_attr_number() << ","
			<< "\"value\":" << this->get_last_data_item().str()
			<< "}"
		;
		return ss.str();
	}
	
	std::string AttrMessage::str() const {
		std::stringstream ss;
		ss << " Node ID: " << this->get_node_id() << "\n"
			<< "Group ID: " << this->get_group_id() << "\n"
			<< " Attr ID: " << this->get_attr_id() << "\n"
			<< " Attr No: " << this->get_attr_number() << "\n"
			<< "Attr Val: " << this->get_last_data_item().str() << "\n"
		;
		return ss.str();
	}
	
	void AttrMessage::before_respond_to(RF24NetworkHeader header) {
		EncryptedMessage::before_respond_to(header);
		std::cout << this->str();
	}
	
}}}}
