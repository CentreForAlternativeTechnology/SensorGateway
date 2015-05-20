#ifndef MAILBOX_HPP
#define MAILBOX_HPP

#include <cstdint>
#include <map>
#include <vector>

namespace cat { namespace snsrgtwy { namespace radio {
	
	template <typename T>
	class Mailbox {
		protected:
			typedef std::vector<T> messages_t;
			std::map<uint16_t, std::vector<T> > map;
		public:
			void push_back_at(uint16_t address, T message);
	};

}}}

#endif
