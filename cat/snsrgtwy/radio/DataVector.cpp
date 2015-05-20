#include <cstddef>
#include <cstring>
#include <vector>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/DataVector.hpp>

#include <cstdio>

namespace cat { namespace snsrgtwy { namespace radio {
	
	void DataVector::push_back(Data data) {
		parent::push_back(data);
		this->bytes += data.total_bytes() + 1;
	}
	
	Data DataVector::at(size_t n) const {	
		return parent::at(n);
	}
	
	size_t DataVector::get_bytes() {
		return this->bytes;
	}
	
	size_t DataVector::to_data_message(char* &message, char status) {
		const size_t size_of_header = 4;
		
		Data data;
		size_t size = size_of_header;
		size_t current_bytes = 0;
		size_t size_of_data_item_list = 0;
		
		message[2] = status; /* Status code */
		message[3] = (char) this->size(); /* Total data items. */
		
		/* List each data item in the message. */
		for (DataVector::iterator i = this->begin(); i != this->end(); i++) {
			data = *i;
			
			/* Determine the number of bytes required for this data item. */
			current_bytes = data.total_bytes();
			/* Add the data size identifier to the message. */
			message[size++] = data.size;
			/* Using the data union, copy the data value to the message. */
			memcpy(&message[size], data.string, current_bytes);
			/* Increase the size of the message accordingly. */
			size += current_bytes;
		}
		
		/* Indicate the length of the data at the start of the message. */
		size_of_data_item_list = size - size_of_header;
		message[0] = size_of_data_item_list & 0xFF;
		message[1] = (size_of_data_item_list & 0xFF00) >> 8;
		
		return size;
	}
}}}

