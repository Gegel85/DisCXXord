//
// Created by Gegel85 on 09/04/2019.
//

#ifndef DISC_ORD_CLIENT_HPP
#define DISC_ORD_CLIENT_HPP


#include <thread>
#include <optional>
#include <JsonObject.hpp>
#include "SecuredWebSocket.hpp"
#include "Logger.hpp"

namespace DisCXXord {
	class Client {
	public:
		struct clientHandlers {
			void (*onready)();
		};

		Client(const std::string &logpath = "./disc++ord.log", Logger::LogLevel level = Logger::WARNING);
		~Client();
		void setHandlers(clientHandlers handl);
		void run(const std::string &username, const std::string &password);
		void run(const std::string &token);

	private:
		void _handleWebSocket();
		void _identify();
		void _heartbeat(bool waitAnswer = false);
		void _treatWebSocketPayloads();
		void _handlePayload(JsonObject &);

		struct HeartbeatInfos {
			size_t						_heartbeatInterval;
			std::thread					_heartbeatThread;
			std::optional<int>				_lastSValue;
			std::chrono::_V2::system_clock::time_point	_lastHeartbeat;
			bool						_isAcknoledged;
			int						_nbNotAcknoledge;
		};

		std::string 		_token;
		SecuredWebSocket	_webSocket;
		SecuredSocket		_httpSocket;
		clientHandlers		_handlers;
		Logger			_logger;
		HeartbeatInfos		_hbInfos;
	};
}


#endif //DISC_ORD_CLIENT_HPP
