#ifndef ATTRMESSAGE_HPP
#define ATTRMESSAGE_HPP

#include <cstddef>
#include <string>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstraction of messages from sensor nodes that are associated with
	 * their attributes.
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class AttrMessage : public EncryptedMessage {
		public:
			/**
			 * Constructs an abstraction of a message from a sensor node
			 * that is associated with one of its attributes.
			 *
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			AttrMessage(char message[]) : EncryptedMessage(message) {}
			
			/**
			 * Returns the total number of data items expected for a message of
			 * this type.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 5.
			 * 
			 * @return the total number of data items expected
			 * 
             * @see cat/snsrgtwy/radio/messages/DataMessage.hpp
             */
			virtual size_t expected_total_data_items() const;
				
			/**
			 * Retrieves the node ID from the message.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptMessageException if the node ID cannot be
			 * retrieved.
			 * 
			 * @return the node ID extracted from the message
			 */
			virtual unsigned short get_node_id() const;
			
			/**
			 * Retrieves the group ID from the message.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptMessageException if the group ID cannot be
			 * retrieved.
			 * 
			 * @return the group ID extracted from the message
			 */
			virtual unsigned short get_group_id() const;
			
			/**
			 * Retrieves the attribute ID from the message.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptMessageException if the node ID cannot be
			 * retrieved.
			 * 
			 * @return the attribute ID extracted from the message
             */
			virtual unsigned short get_attr_id() const;
			
			/**
			 * Retrieves the attribute number from the message.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptMessageException if the attribute number cannot be
			 * retrieved.
			 * 
			 * @return the attribute number extracted from the message
			 */
			virtual unsigned short get_attr_number() const;
			
			/**
			 * Returns a string representing the message as JSON.
			 * @return a string representing the message as JSON.
			 */
			virtual std::string to_json() const;
			
			std::string str() const;
		protected:
			virtual void before_respond_to(RF24NetworkHeader header);
			/**
			 * The total number of data items expected for a message of this
			 * type.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 5.
			 */
			static const size_t EXPECTED_TOTAL_DATA_ITEMS;
			
			/**
			 * The index of the node ID in reference to the data_items data
			 * member.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 0.
			 */
			static const size_t INDEX_OF_NODE_ID;
			
			/**
			 * The index of the group ID in reference to the data_items data
			 * member.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 1.
			 */
			static const size_t INDEX_OF_GROUP_ID;
			
			/**
			 * The index of the attribute ID in reference to the data_items 
			 * data member.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 2.
			 */
			static const size_t INDEX_OF_ATTR_ID;
			
			/**
			 * The index of the attribute number in reference to the data_items
			 * data member.
			 *  
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 3.
			 */
			static const size_t INDEX_OF_ATTR_NUMBER;
			
			/**
			 * The index of the last data item in reference to the data_items
			 * data member.
			 * 
			 * At the time of writing, the Low Power Radio To Gateway
			 * Communications Specification implies this is 4.
			 */
			static const size_t INDEX_OF_LAST_DATA_ITEM;
			
			/** 
			 * Retrieves the data item that follows the attribute number.
			 * 
			 * It is intended for this member function to throw a
			 * CorruptMessageException if the data item cannot be
			 * retrieved.
			 * 
			 * @return the node ID extracted from the message
			 */
			virtual Data get_last_data_item() const;
			
			/**
			 * Returns the unsigned short value from the data_items data member
			 * at the specified index, but only if the associated Data is of
			 * size U_SHORT.
			 * 
			 * It is intended for this function to act as a data_items wrapper
			 * for the majority of getters associated with this class.
			 * 
			 * It is also intended for this member function to throw a
			 * CorruptMessageException if the index is out of range or
			 * if the associated Data is not an unsigned short.
			 * 
			 * @param index the index to retrieve at from the data_items data
			 * member
			 * @return the unsigned short of Data at the specified index
			 * @throw cat::snsrgtwy::radio::messages::CorruptMessageException
			 */
			virtual unsigned short get_u_short_data_item_at(size_t index) const;
			
			/* Assigns a pointer and a size to the given arguments respectively
			 * for a message to use as part of the response.
			 * 
			 * It is intended for this member function to simply assign the 
			 * same pointer and size for the received message on success.
			 * 
			 * @param message a reference that should be assigned a pointer
			 * to a response message that has been allocated memory
			 * @param size a reference that should be assigned the size of the
			 * response message
			 * @return true if the allocated memory of the message should be
			 * freed immediately after use, false otherwise
			 */
			//virtual bool get_response_message(char* &message, size_t &size);
			virtual DataVector get_response_message_as_data_vector();
	};
	
}}}}

#endif

