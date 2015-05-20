#ifndef NODEDATABASE_HPP
#define NODEDATABASE_HPP

#include <cstdint>
#include <string>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * An interface for persisting shared keys and EMonCMS IDs.
	 */
	class NodeDatabase {
		public:
			/**
			 * Converts an address to a string for use in the database.
			 * @param address the address to convert
			 * @return the address as a string
			 */
			static std::string to_string(uint16_t address);
			/**
			 * Returns the shared key of a given address.
			 * @param address the address to query
			 * @return the shared key of the given address
			 */
			virtual char* get_shared_key(const char* address)=0;
			/**
			 * Returns the address of a given EMonCMS ID.
			 * @param emoncms_id the EMonCMS ID to query
			 * @return the address of the given EMonCMS ID.
			 */
			virtual char* get_address(const char* emoncms_id)=0;
			/**
			 * Update the shared key associated with a given address.
			 * @param address the address to assign the shared key to
			 * @param shared_key the value of the new shared key
			 */
			virtual void set_shared_key(
				const char* address,
				const char* shared_key
			)=0;
			/**
			 * Update the EMonCMS ID associated with the given address.
			 * @param address the address to assign to the EMonCMS ID to
			 * @param emoncms_id the value of the new EMonCMS ID
			 */
			virtual void set_emoncms_id(
				const char* address,
				const char* emoncms_id
			)=0;
	};
}}}

#endif

