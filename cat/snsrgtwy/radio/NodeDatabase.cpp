#include <string>
#include <sstream>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	std::string NodeDatabase::to_string(uint16_t address) {
		std::stringstream ss;
		ss << address;
		return ss.str();
	}
}}}

