//
// Created by Gegel85 on 09/04/2019.
//

#include <string>
#include <JsonParser.hpp>
#include "defines.hpp"
#include "Client.hpp"

namespace DisCXXord {

	Client::Client(const std::string &logpath, DisCXXord::Logger::LogLevel level) :
		_handlers(),
		_logger(logpath, level)
	{
	}

	Client::~Client()
	{
		if (this->_webSocket.isOpen())
			this->_webSocket.disconnect();
	}

	void Client::_heartbeat(bool waitAnswer)
	{
		std::string	payload = "{"
			"\"op\": 1,"
			"\"d\": " + (this->_hbInfos._lastSValue ? std::to_string(*this->_hbInfos._lastSValue) : "null") +
		"}";

		this->_logger.debug("Sending heartbeat");
		this->_webSocket.send(payload);
		this->_hbInfos._lastHeartbeat = std::chrono::system_clock::now();
		this->_hbInfos._isAcknoledged = !waitAnswer;
	}

	void Client::_identify()
	{
		std::string	payload = "{"
			"\"op\": 2,"
			"\"d\": {"
				"\"token\": \"" + this->_token + "\","
  				"\"properties\": {"
					"\"$os\": \"" PLATFORM_NAME "\","
					"\"$browser\": \"" LIBNAME "\","
					"\"$device\": \"" LIBNAME "\""
				"}"
			"}"
		"}";

		this->_webSocket.send(payload);
	}

	void Client::_handlePayload(JsonObject &object)
	{
		switch (static_cast<int>(object["op"]->to<JsonNumber>().value())) {
		case 1:
			this->_logger.debug("Server requests heartbeat");
			this->_heartbeat();
		case 11:
			this->_logger.debug("Server acknowledged heartbeat");
			this->_hbInfos._isAcknoledged = true;
			this->_hbInfos._nbNotAcknoledge = 0;
		}
	}

	void Client::_treatWebSocketPayloads()
	{
		std::string response;

		while (true) {
			fd_set	set;
			struct timeval	timestruct = {10, 0};

			FD_ZERO(&set);
			FD_SET(this->_webSocket.getSockFd(), &set);
			while (timestruct.tv_sec) {
				if (!select(this->_webSocket.getSockFd(), &set, nullptr, nullptr, &timestruct) && !this->_hbInfos._isAcknoledged) {
					this->_logger.warning("Server didn't acknowledged previous heartbeat after 10 seconds");
					this->_hbInfos._nbNotAcknoledge++;
				} else {
					std::string str = this->_webSocket.getAnswer();
					auto val = JsonParser::parseString(str);
					auto &object = val->to<JsonObject>();

					this->_logger.debug("Server sent opcode " + std::to_string(static_cast<int>(object["op"]->to<JsonNumber>().value())));
					this->_handlePayload(object);
				}
			}
		}
	}

	void Client::_handleWebSocket()
	{
		std::string answer = this->_webSocket.getAnswer();
		auto val = JsonParser::parseString(answer);
		auto &object = val->to<JsonObject>();

		if (object["op"]->to<JsonNumber>().value() == 10)
			this->_logger.info("Received HELLO from gateway");
		else
			throw std::invalid_argument("Gateway didn't send Hello");
		this->_hbInfos._heartbeatInterval = object["d"]->to<JsonObject>()["heartbeat_interval"]->to<JsonNumber>().value();
		if (object.value().find("s") != object.value().end())
			this->_hbInfos._lastSValue = object["s"]->to<JsonNumber>().value();
		this->_logger.debug("Default heartbeat interval is " + std::to_string(this->_hbInfos._heartbeatInterval));
		this->_logger.debug("Sending Identify event");
		this->_identify();
		this->_hbInfos._heartbeatThread = std::thread{[this]() {
			this->_hbInfos._isAcknoledged = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(this->_hbInfos._heartbeatInterval));
			while (this->_webSocket.isOpen()) {
				this->_heartbeat(true);
				std::this_thread::sleep_for(std::chrono::milliseconds(this->_hbInfos._heartbeatInterval));
			}
		}};
		this->_treatWebSocketPayloads();
	}

	void Client::run(const std::string &token)
	{
		this->_token = token;
		try {
			Socket::HttpRequestIn request = {
				.method = "GET",
				.host = "discordapp.com",
				.portno = 443,
				.header = {},
				.path = "/api/gateway",
			};

			this->_logger.debug("Fetching gateway URL");

			std::string url = this->_httpSocket.makeHttpRequest(request).body;
			auto val = JsonParser::parseString(url);
			auto &object = val->to<JsonObject>();

			url = object["url"]->to<JsonString>().value();
			this->_logger.info("Connecting to Gateway");
			this->_webSocket.connect(url.substr(6, url.size() - 6), 443);
			this->_handleWebSocket();
		} catch (std::exception &e) {
			this->_logger.critical(e.what());
		}
	}

	void Client::run(const std::string &username, const std::string &password)
	{
		(void)username;
		(void)password;
	}

	void Client::setHandlers(DisCXXord::Client::clientHandlers handl)
	{
		this->_handlers = handl;
	}
}