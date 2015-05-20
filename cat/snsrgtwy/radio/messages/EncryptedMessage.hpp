#ifndef ENCRYPTEDMESSAGE_HPP
#define ENCRYPTEDMESSAGE_HPP

#include <cstddef>
#include <cstdint>
#include <cat/snsrgtwy/DiffieHellman.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/messages/Message.hpp>
#include <cat/snsrgtwy/radio/messages/DataMessage.hpp>
#include <cat/snsrgtwy/radio/messages/KeyExchangeMessage.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	/**
	 * An abstraction of messages from sensor nodes that have been encrypted
	 * with their respective private key via AES-128.
	 * 
	 * @see Low Power Radio To Gateway Communications Specification
	 */
	class EncryptedMessage : public DataMessage {
		public:
			static const size_t SIZE_OF_BLOCK = 16;
			static const size_t SIZE_OF_KEY = KeyExchangeMessage::SIZE_OF_KEY;
			/**
			 * Constructs an abstraction of a message from a sensor node that
			 * has been encrypted with their respective private key via
			 * AES-128.
			 * 
			 * @param message the raw content of the message represented as an
			 * array of chars
			 */
			EncryptedMessage(char message[]) : DataMessage(NULL) {
				this->has_determined_shared_key = false;
				/* Determine the length of the encrypted message. */
#ifdef ENCRYPTION
				this->size = (
					this->read(
						message,
						1,
						1,
						0,
						true
					) * SIZE_OF_BLOCK
				) + 1;
#else
				this->size = this->read(
					message,
					2,
					2,
					0,
					true
				) + 4;
#endif
				this->message = Message::copy(message, this->size);
			}
			
			static void convert_to_encrypted_message(
				char* &message,
				size_t &size,
				const char shared_key[SIZE_OF_KEY]
			);
			
			static size_t encrypt(
				char* &cipher,
				const char* plain,
				size_t plain_size,
				const char key[SIZE_OF_KEY]
			);
			
			static void decrypt(
				char* &plain,
				const char* cipher,
				size_t cipher_size,
				const char key[SIZE_OF_KEY]
			);
			
			static void determine_shared_key_for(
				char (&shared_key)[SIZE_OF_KEY],
				uint16_t address
			);
		protected:
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
			virtual void before_respond_to(RF24NetworkHeader header);
			
			static const size_t INDEX_OF_TOTAL_BLOCKS;
			static const size_t INDEX_OF_CIPHER;
			/**
			 * Whether or not the shared key has been determined.
			 * 
			 * This data member is intended to inform the
			 * get_response_message() of the state of the shared_key data 
			 * member.
			 */
			bool has_determined_shared_key;
			/**
			 * The key to use in order to decrypt the message and encrypt the
			 * response.
			 * 
			 * It is intended for this data member to be null until the
			 * perform_response() member function is called so that the 
			 * public key can be extracted from the PublicKeyDatabase using
			 * the address of the node.
			 */
			char shared_key[SIZE_OF_KEY];
			/**
			 * Assigns a pointer and a size to the given arguments respectively
			 * for an encrypted message to use as part of the response.
			 * 
			 * It is intended for this member function to simply encrypt the
			 * given message using the originator public key determined by the
			 * previous call to perform_response(). If the public key has not
			 * yet been determined, an EncryptedMessageException should be
			 * thrown.
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
			virtual bool get_response_message(char* &message, size_t &size);
	};
	
}}}}

#endif

