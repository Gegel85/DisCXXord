//
// Created by Gegel85 on 09/04/2019.
//

#include <string>
#include <JsonParser.hpp>
#include "defines.hpp"
#include "Client.hpp"

#ifndef _WINDOWS_DISCXXORD
#	include <sys/select.h>
	typedef fd_set FD_SET;
#endif

#ifdef __GNUG__
#include <cxxabi.h>

std::string getLastExceptionName()
{
	int status;

	if (!abi::__cxa_current_exception_type())
		return "No exception";
	return abi::__cxa_demangle(abi::__cxa_current_exception_type()->name(), nullptr, nullptr, &status);
}

#endif

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

		if (!this->_hbInfos._isAcknoledged) {
			this->_logger.warning("Server didn't acknowledged previous heartbeat");
		}
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

	void Client::disconnect()
	{
		this->_logger.info("Disconnecting...");
		this->_webSocket.disconnect();
		this->_disconnected = true;
	}

	void Client::_handlePayload(JsonObject &object)
	{
		switch (static_cast<int>(object["op"]->to<JsonNumber>().value())) {
		case 0:
			this->_hbInfos._lastSValue = object["s"]->to<JsonNumber>().value();
			this->_dispatchEvents[object["t"]->to<JsonString>().value()](
				*object["d"]
			);
			break;
		case 1:
			this->_logger.debug("Server requests heartbeat");
			this->_heartbeat();
			break;
		case 11:
			this->_logger.debug("Server acknowledged heartbeat");
			this->_hbInfos._isAcknoledged = true;
			this->_hbInfos._nbNotAcknoledge = 0;
		}
	}

	void Client::_treatWebSocketPayloads()
	{
		std::string response;

		while (!this->_disconnected) {
			fd_set	set;
			struct timeval	timestruct = {10, 0};

			FD_ZERO(&set);
			FD_SET(this->_webSocket.getSockFd(), &set);
			while (!this->_disconnected && timestruct.tv_sec) {
				this->_logger.debug("Reading server answer");
				std::string str = this->_webSocket.getAnswer();
				auto val = JsonParser::parseString(str);
				auto &object = val->to<JsonObject>();

				val->dump();
				this->_logger.debug("Server sent opcode " + std::to_string(static_cast<int>(object["op"]->to<JsonNumber>().value())));
				this->_handlePayload(object);
			}
		}
	}

	std::optional<std::string> Client::timedGetAnswer(int time)
	{
		FD_SET	set;
		struct timeval	timestruct = {time, 0};

		FD_ZERO(&set);
		FD_SET(this->_webSocket.getSockFd(), &set);
		if (select(FD_SETSIZE, &set, nullptr, nullptr, &timestruct) == 1)
			return this->_webSocket.getAnswer();
		return {};
	}

	void Client::_connect()
	{
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

		this->_logger.debug("Waiting HELLO payload");

		std::optional<std::string> answer = this->timedGetAnswer(30);

		if (!answer)
			throw TimeoutException("Gateway didn't reply to the identify request");
		auto value = JsonParser::parseString(*answer);
		auto &obj = value->to<JsonObject>();

		if (obj["op"]->to<JsonNumber>().value() == 10)
			this->_logger.info("Received HELLO from gateway");
		else
			throw std::invalid_argument("Gateway didn't send Hello");
		this->_hbInfos._heartbeatInterval = static_cast<size_t>(obj["d"]->to<JsonObject>()["heartbeat_interval"]->to<JsonNumber>().value());
		this->_logger.debug("Default heartbeat interval is " + std::to_string(this->_hbInfos._heartbeatInterval));
		this->_logger.debug("Sending Identify event");
		this->_identify();
		this->_logger.debug("Setting heartbeating thread");
		this->_hbInfos._heartbeatThread = std::thread{[this]() { this->_heartbeatLoop(); }};
	}

	void Client::_heartbeatLoop()
	{
		for (int i = 0; this->_webSocket.isOpen() && i < this->_hbInfos._heartbeatInterval; i += 1000) {
			if (this->_hbInfos._heartbeatInterval - i < 1000)
				std::this_thread::sleep_for(std::chrono::milliseconds(this->_hbInfos._heartbeatInterval - i));
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		while (this->_webSocket.isOpen()) {
			this->_heartbeat(true);
			for (int i = 0; this->_webSocket.isOpen() && i < this->_hbInfos._heartbeatInterval; i += 1000) {
				if (this->_hbInfos._heartbeatInterval - i < 1000)
					std::this_thread::sleep_for(std::chrono::milliseconds(this->_hbInfos._heartbeatInterval - i));
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}

	void Client::_handleWebSocket()
	{
		this->_logger.debug("Listening for gatway payloads");
		this->_treatWebSocketPayloads();
	}

	void Client::run(const std::string &token)
	{
		this->_token = token;
		try {
			this->_connect();
			this->_handleWebSocket();
		} catch (std::exception &e) {
			#ifdef __GNUG__
			this->_logger.critical("Caught exception: " + getLastExceptionName());
			#endif
			this->_logger.critical(e.what());
		}
		if (this->_hbInfos._heartbeatThread.joinable())
			this->_hbInfos._heartbeatThread.join();
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

	void Client::_ready(JsonValue &val)
	{

	}

	void Client::_resumed(JsonValue &val)
	{

	}

	void Client::_channelCreate(JsonValue &val)
	{

	}

	void Client::_channelUpdate(JsonValue &val)
	{

	}

	void Client::_channelDelete(JsonValue &val)
	{

	}

	void Client::_channelPinsUpdate(JsonValue &val)
	{

	}

	void Client::_guildCreate(JsonValue &val)
	{

	}

	void Client::_guildUpdate(JsonValue &val)
	{

	}

	void Client::_guildDelete(JsonValue &val)
	{

	}

	void Client::_guildBanAdd(JsonValue &val)
	{

	}

	void Client::_guildBanRemove(JsonValue &val)
	{

	}

	void Client::_guildEmojisUpdate(JsonValue &val)
	{

	}

	void Client::_guildIntegrationsUpdate(JsonValue &val)
	{

	}

	void Client::_guildMemberAdd(JsonValue &val)
	{

	}

	void Client::_guildMemberRemove(JsonValue &val)
	{

	}

	void Client::_guildMemberUpdate(JsonValue &val)
	{

	}

	void Client::_guildMembersChunk(JsonValue &val)
	{

	}

	void Client::_guildRoleCreate(JsonValue &val)
	{

	}

	void Client::_guildRoleUpdate(JsonValue &val)
	{

	}

	void Client::_guildRoleDelete(JsonValue &val)
	{

	}

	void Client::_messageCreate(JsonValue &val)
	{

	}

	void Client::_messageUpdate(JsonValue &val)
	{

	}

	void Client::_messageDelete(JsonValue &val)
	{

	}

	void Client::_messageDeleteBulk(JsonValue &val)
	{

	}

	void Client::_messageReactionAdd(JsonValue &val)
	{

	}

	void Client::_messageReactionRemove(JsonValue &val)
	{

	}

	void Client::_messageReactionRemoveAll(JsonValue &val)
	{

	}

	void Client::_presenceUpdate(JsonValue &val)
	{

	}

	void Client::_typingStart(JsonValue &val)
	{

	}

	void Client::_userUpdate(JsonValue &val)
	{

	}

	void Client::_voiceStateUpdate(JsonValue &val)
	{

	}

	void Client::_voiceServerUpdate(JsonValue &val)
	{

	}

	void Client::_webhooksUpdate(JsonValue &val)
	{

	}
}