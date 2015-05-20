#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <RF24Network/RF24Network.h>
#include <tiny-AES128-C/aes.h>

#include <cat/snsrgtwy/DiffieHellman.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/messages/KeyExchangeMessage.hpp>
#include <cat/snsrgtwy/radio/messages/EncryptedMessageException.hpp>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	const size_t EncryptedMessage::INDEX_OF_TOTAL_BLOCKS = 0;
	const size_t EncryptedMessage::INDEX_OF_CIPHER = 1;
	
	void EncryptedMessage::convert_to_encrypted_message(
		char* &message,
		size_t &size,
		const char shared_key[SIZE_OF_KEY]
	) {
		size_t cipher_size;
		char* cipher = NULL;
		
		cipher_size = encrypt(
			cipher,
			message,
			size,
			shared_key
		);
		
		size = cipher_size + INDEX_OF_CIPHER;
		message = new char[size]();
		message[INDEX_OF_TOTAL_BLOCKS] = (char) size/SIZE_OF_BLOCK;
		memcpy(message + INDEX_OF_CIPHER, cipher, cipher_size);
		
		if (cipher != NULL) {
			delete[] cipher;
		}
	}
	
	void EncryptedMessage::determine_shared_key_for(
		char (&shared_key)[SIZE_OF_KEY],
		uint16_t address
	) {
		const char* shared_key_string;
		DiffieHellman dh;
		NodeDatabase* db = NodeDatabaseCreator::create();
		std::string address_str = NodeDatabase::to_string(address);
		
		/* Clear array before conversion. */
		memset(shared_key, 0, sizeof(shared_key));
		
		/* Extract the shared key for the node from the database. */
		shared_key_string = db->get_shared_key(address_str.c_str());
		if (shared_key_string == NULL) {
			delete db;
			throw EncryptedMessageException("could not determine shared key");
		}
		
		/* Convert the shared key from a string to a byte array. */
		Data::to_byte_array(shared_key, shared_key_string);
		
		delete db;
		delete[] shared_key_string;
	}
	
	size_t EncryptedMessage::encrypt(
		char* &cipher,
		const char* plain,
		size_t plain_size,
		const char key[SIZE_OF_KEY]
	) {
		const size_t divisor = SIZE_OF_BLOCK;
		char* plain_copy;
		size_t total_iterations;
		/* Determine the size of padded plain text for AES encryption. */
		size_t cipher_size = plain_size;
		size_t remainder = plain_size % divisor;
		if (remainder > 0) {
			cipher_size += divisor-remainder;
		}
		
		/* Initialize a padded plain text copy and cipher. */
		plain_copy = new char[cipher_size]();
		cipher = new char[cipher_size]();
		/* Copy the plain text to the padded copy. */
		memcpy(plain_copy, plain, plain_size);
#ifdef ENCRYPTION
		/* Determine the number of iterations required. */
		total_iterations = cipher_size / divisor;
		/* Encrypt the message 16 bytes at a time. */
		for(size_t i = 0; i < total_iterations; i++) {
			AES128_ECB_encrypt(
				(uint8_t*) plain_copy+(i*divisor),
				(uint8_t*) key,
				(uint8_t*) cipher+(i*divisor)
			);
		}
		
		delete[] plain_copy;
#else
		delete[] cipher;
		cipher = plain_copy;
#endif
		return cipher_size;
	}
	
	void EncryptedMessage::decrypt(
		char* &plain,
		const char* cipher,
		size_t cipher_size,
		const char key[SIZE_OF_KEY]
	) {
		const size_t divisor = SIZE_OF_BLOCK;
		size_t total_iterations = cipher_size / divisor;
		char* cipher_copy = new char[cipher_size]();
		plain = new char[cipher_size]();
		memcpy(cipher_copy, cipher, cipher_size);
#ifdef ENCRYPTION
		for(size_t i = 0; i < total_iterations; i++) {
			AES128_ECB_decrypt(
				(uint8_t*) cipher_copy + (i*divisor),
				(uint8_t*) key,
				(uint8_t*) plain + (i*divisor)
			);
		}
		delete[] cipher_copy;
#else
		delete[] plain;
		plain = cipher_copy;
#endif
	}
	
	/**
	 * Decrypts and interprets the received message before performing
	 * the appropriate response.
	 * 
	 * It is intended for this member function to determine the public
	 * key of the originator and use this in combination with the
	 * sensor gateway's private key to decrypt the message. It should
	 * then call the parent implementation of this member function in
	 * order for the message to be interpreted.
	 */
	void EncryptedMessage::before_respond_to(RF24NetworkHeader header) {
		std::cout << "Decrypting... " << std::endl;
#ifdef ENCRYPTION
		char* decrypted;
		
		/* Determine the shared key. */
		this->determine_shared_key_for(this->shared_key, header.from_node);
		this->has_determined_shared_key = true;
		
		/* Decrypt the message. */
		this->decrypt(
			decrypted,
			this->message + INDEX_OF_CIPHER,
			this->size - INDEX_OF_CIPHER,
			this->shared_key
		);
		/* Replace message with decrypted message. */
		delete[] this->message;
		this->message = decrypted;
#else
		this->size = this->read(this->message, MAX_SIZE, 2, 0, true) + 4;
#endif
		this->print(this->message, this->size);
		
		/* Interpret the message. */
		return DataMessage::before_respond_to(header);
	}
	
	/**
	 * Assigns a pointer and a size to the given arguments respectively
	 * for an encrypted message to use as part of the response.
	 * 
	 * This member function simply encrypts the given message using the
	 * shared key determined by the previous call to
	 * respond_to(). If the shared key has not been yet determined, an
	 * EncryptedMessageException is thrown.
	 * 
	 * Derived classes should call this member function in their
	 * respective get_response_message() implementations in order to
	 * encrypt the response message.
	 * 
	 * @param message a reference that should be assigned a pointer
	 * to a response message that has been allocated memory
	 * @param size a reference that should be assigned the size of the
	 * response message
	 * @return true if the allocated memory of the message should be
	 * freed immediately after use, false otherwise
	 * @throw cat::snsrgtwy::radio::message::EncryptedMessageException
	 */
	bool EncryptedMessage::get_response_message(char* &message, size_t &size) {
		DataMessage::get_response_message(message, size);
		std::cout << "Encrypting... " << std::endl;
#ifdef ENCRYPTION
		if (!this->has_determined_shared_key) {
			throw EncryptedMessageException(
				"shared key has not yet been determined"
			);
		}
		this->convert_to_encrypted_message(message, size, this->shared_key);
#endif
		return true;
	}
}}}}

