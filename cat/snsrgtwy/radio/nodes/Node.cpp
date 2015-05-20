#include <iostream>
#include <string>
#include <cat/snsrgtwy/radio/nodes/Node.h>

namespace cat { namespace snsrgtwy { namespace radio { namespace nodes {

	const char Node::MESSAGE_START_BYTE = 0xFF;
	const char Node::MESSAGE_EMPTY_BYTE = 0x00;
	const int Node::MESSAGE_MAX_LENGTH = 1024;
	
	Node::Node(char id) {
		this->id = id;
	}
	
	char Node::get_id() {
		return this->id;
	}
	
	char Node::read() {
		/* Emulate receiving a message from the radio. */
		static unsigned int index = 0;
		static char buffer[] = {
			
			/* Some garble from the end of a previous message */
			MESSAGE_EMPTY_BYTE, 0xBC, 0xDE,
			/* Start of register message */
			/*TODO Consider whether or not nodes need to register */
			MESSAGE_START_BYTE, 0x0C, 'R',
			/* Using IPv4 address (for prototype) */
			'1', '9', '2', '.', '1', '6', '8', '.', '0', '.', '1',
			/* IPv6 equivalent of 192.168.0.1 */
			/*':',':','C','0','3','A',':','1',*/
			
			/* Start of a attribute registration message */
			MESSAGE_START_BYTE, 0x07, 'A',
			/* Group ID (from example)*/
			0x02, 0x01,
			/* Attribute ID (from example) */
			0x00, 0x01,
			/* Attribute Number (from example) */
			/* Note that this number is 12-bits, so there is some padding. */
			0x0F, 0xA0,
			
			/* Start of a post message */
			MESSAGE_START_BYTE, 0x07, 'P',
			/* Group ID (from example)*/
			0x02, 0x01,
			/* Attribute ID (from example) */
			0x00, 0x01,
			/* Attribute Number (from example) */
			/* Note that this number is 12-bits, so there is some padding. */
			0x0F, 0xA0,
			
			/* Emulate the radio stopping transmission */
			MESSAGE_EMPTY_BYTE, MESSAGE_EMPTY_BYTE
		};
		
		char read = buffer[index];
		if (index < 34) {
			index++;
		}
		return read;
	}
	
	char* Node::read_message(int timeout = 0) {
		char buffer[MESSAGE_MAX_LENGTH]; /* Temporary storage for the incoming message. */
		
		char* message;
		char length = 0; /* The current length of the message. */
		int time = 0;
		
		/* While we are unaware of the length of the incoming message. */
		while (time < timeout && !length) {
			if (this->read() == MESSAGE_START_BYTE) {
				length = this->read();
			} else {
				time++;
			}
		}
		
		if (time < timeout) {
			for (int i = 0; i < length; i++) {
				buffer[i] = this->read();
			}
		}
		
		if (length) {
			/* ...allocate it memory on the heap. */
			message = new char[length+1];
			for (int i = 0; i < length; i++) {
				message[i] = buffer[i];
			}
		/* Otherwise, return an empty array */
		} else {
			message = NULL;
		}
		return message;
	}	
}}}}

