//
// Created by Gegel85 on 06/04/2019.
//

#ifndef DISC_ORD_SECUREDWEBSOCKET_HPP
#define DISC_ORD_SECUREDWEBSOCKET_HPP


#include <random>
#include "SecuredSocket.hpp"

namespace DisCXXord
{
#define WEBSOCKET_CODE(code) ((code - 1000 < 0 || code - 1000 > 15) ? ("???") : (codesStrings[code - 1000]))

	class SecuredWebSocket : public SecuredSocket {
	private:
		std::random_device	_rand;
		void	_establishHandshake(const std::string &host);
		void	_pong(const std::string &validator);

	public:
		static const char * const codesStrings[];
		using Socket::connect;

		class InvalidHandshakeException : public Exception {
		public:
			explicit InvalidHandshakeException(const std::string &msg) : Exception(msg) {};
		};

		class InvalidPongException : public Exception {
		public:
			explicit InvalidPongException(const std::string &msg) : Exception(msg) {};
		};

		class ConnectionTerminatedException : public Exception {
		private:
			int _code;
		public:
			explicit ConnectionTerminatedException(const std::string &msg, int code) : Exception(msg), _code(code) {};
			int getCode() { return this->_code; };
		};

		SecuredWebSocket() = default;
		~SecuredWebSocket() = default;
		void		send(const std::string &value) override;
		void		connect(const std::string &host, unsigned short portno) override;
		void		sendHttpRequest(const HttpRequestIn &request);
		std::string	getAnswer();
		std::string	getRawAnswer();
	};
}


#endif //DISC_ORD_SECUREDWEBSOCKET_HPP
