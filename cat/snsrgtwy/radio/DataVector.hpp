#ifndef DATAVECTOR_HPP
#define	DATAVECTOR_HPP

#include <cstddef>
#include <vector>
#include <cat/snsrgtwy/radio/Data.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * A std::vector representing a list of data items, often referred to
	 * simply as the data items.
	 */
	class DataVector : protected std::vector<Data> {
		public:
			DataVector() : std::vector<Data>() {
				this->bytes = 4;
			}
			
			void push_back(Data data);
			
			Data at(size_t n) const;
			
			size_t get_bytes();
			
			size_t to_data_message(char* &message, char status);
		protected:
			size_t bytes;
		private:
			typedef std::vector<Data> parent;
	};

}}}

#endif

