#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cat/snsrgtwy/radio/Mailbox.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	/*
	template <typename T>
	void Mailbox<T>::delete_all(uint16_t address) {
		messages_t messages = this->at(address);
		for (size_t i = 0; i < messages.size(); i++) {
			delete messages.back();
			messages.pop_back();
		}
	}
	*/
	/*
	template <typename T>
	void Mailbox<T>::push_back_at(uint16_t address, T message) {
		try {
			this->map.at(address);
		} catch (std::out_of_range oor) {
			this->map[address] = std::vector<T>();
		}
		this->map[address].push_back(message);
	}
	*/
}}}

