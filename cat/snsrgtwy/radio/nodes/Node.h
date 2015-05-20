namespace cat { namespace snsrgtwy { namespace radio { namespace nodes {

	class Node {
		public:
			static const char MESSAGE_START_BYTE;
			static const char MESSAGE_EMPTY_BYTE;
			static const int MESSAGE_MAX_LENGTH;
			Node(char id);
			char get_id();
			char read();
			char* read_message(int timeout);
		protected:
			char id;
	};
}}}}

