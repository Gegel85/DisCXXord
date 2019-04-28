//
// Created by Gegel85 on 09/04/2019.
//

#include <string>
#include <JsonParser.hpp>
#include "defines.hpp"
#include "Exceptions.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "endpoints.hpp"

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

namespace DisCXXord
{
//Public
	Client::Client(const std::string &logpath, DisCXXord::Logger::LogLevel level) :
		_handlers(),
		_logger(logpath, level)
	{
	}

	Client::~Client()
	{
		if (this->_webSocket.isOpen())
			this->_webSocket.disconnect();
		for (Guild *guild : this->_cachedGuilds)
			delete guild;
		for (User *user : this->_cachedUsers)
			delete user;
	}

	void Client::disconnect()
	{
		this->_logger.info("Disconnecting...");
		this->_webSocket.disconnect();
		this->_disconnected = true;
	}

	void Client::run(const std::string &token)
	{
		this->_token = token;
		try {
			this->_connect();
			this->_handleWebSocket();
		} catch (std::exception &e) {
			#ifdef __GNUG__
			this->_logger.critical("Caught exception: " + getLastExceptionName() + "(" + e.what() + ")");
			#else
			this->_logger.critical("Caught exception: " + e.what());
			#endif
			this->_disconnected = true;
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

	User &Client::me()
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		return *this->_me;
	}

	User &Client::getUser(const std::string &id)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (User *user : this->_cachedUsers)
			if (user->id == id)
				return *user;
		try {
			std::unique_ptr<JsonValue> val = this->makeApiRequest(
				GET_USER_ENDPT"/" + id);

			this->_cachedUsers.emplace_back(new User(*this, val->to<JsonObject>()));
			return *this->_cachedUsers.back();
		} catch (APIErrorException &) {
			throw UserNotFoundException("Cannot find user " + id);
		}
	}

	User &Client::getUser(JsonObject &obj)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (User *user : this->_cachedUsers)
			if (user->id == obj["id"]->to<JsonString>().value())
				return *user;
		this->_cachedUsers.emplace_back(new User(*this, obj));
		return *this->_cachedUsers.back();
	}

	Guild &Client::getGuild(const std::string &id)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (Guild *guild : this->_cachedGuilds)
			if (guild->id == id)
				return *guild;
		try {
			std::unique_ptr<JsonValue> val = this->makeApiRequest(
				GUILDS_ENDPT"/" + id);

			this->_cachedGuilds.emplace_back(new Guild(*this, val->to<JsonObject>()));
			return *this->_cachedGuilds.back();
		} catch (APIErrorException &) {
			throw UserNotFoundException("Cannot find user " + id);
		}
	}

	const std::vector<std::string> &Client::guilds()
	{
		return this->_guilds;
	}

	std::unique_ptr<JsonValue> Client::makeApiRequest(const std::string &endpt, const std::string &method, const std::string &body)
	{
		Request::HttpRequest	result{
			.method = method,
			.url = API_BASE_URL + endpt,
			.body = body,
			.headers = {
				{"Authorization", this->_token},
				{"User-Agent", "DiscordBot (" LIBLINK ", " VERSION ")"}
			}
		};

		if (!body.empty()) {
			result.headers["Content-Type"] = "application/json";
			result.headers["Content-Length"] = std::to_string(body.size());
		}
		this->_logger.debug(method + ": " + API_BASE_URL + endpt + " \"" + body + "\"");
		result = Request::request(result);
		//TODO: Handle 429 and 502
		//TODO: Handle global rate limit
		if (result.code >= 400) {
			this->_logger.error(API_BASE_URL + endpt + ": " + std::to_string(result.code) + " " + result.codeName);
			throw APIErrorException(API_BASE_URL + endpt + ": " + std::to_string(result.code) + " " + result.codeName);
		}
		this->_logger.debug(std::to_string(result.code) + " (" + result.codeName + "): \"" + result.body + "\"");
		return JsonParser::parseString(result.body);
	}



//Private
	void Client::_heartbeat(bool waitAnswer)
	{
		std::string	payload = "{"
			"\"op\": 1,"
			"\"d\": " + (this->_hbInfos._lastSValue ? std::to_string(*this->_hbInfos._lastSValue) : "null") +
		"}";

		if (!this->_hbInfos._isAcknoledged)
			this->_logger.warning("Server didn't acknowledged previous heartbeat");
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
				try {
					auto val = JsonParser::parseString(str);
					auto &object = val->to<JsonObject>();

					this->_logger.debug("Server sent opcode " + std::to_string(static_cast<int>(object["op"]->to<JsonNumber>().value())));
					this->_handlePayload(object);
				} catch (JsonParser::InvalidJsonStringException &e) {
					throw JsonParser::InvalidJsonStringException("Cannot parse Json received from server\r\n" + str + "\r\n" + e.what());
				}
			}
		}
	}

	Optional<std::string> Client::_timedGetAnswer(int time)
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
		this->_logger.info(LIBNAME " version " VERSION);
		this->_logger.debug("Getting current user");
		this->_me.emplace(*this, this->makeApiRequest(USER_ME_ENDPT)->to<JsonObject>());
		this->_logger.info("Connected on " + this->_me->tag());
		this->_logger.debug("Fetching gateway URL");

		auto val = this->makeApiRequest(GATEWAY_ENDPT);
		auto &object = val->to<JsonObject>();
		std::string url = object["url"]->to<JsonString>().value();

		this->_logger.info("Connecting to Gateway");
		this->_webSocket.connect(url.substr(6, url.size() - 6), 443);

		this->_logger.debug("Waiting HELLO payload");

		Optional<std::string> answer = this->_timedGetAnswer(30);

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
		this->_hbInfos._isAcknoledged = true;
		for (size_t i = 0; !this->_disconnected && this->_webSocket.isOpen() && i < this->_hbInfos._heartbeatInterval; i += 1000) {
			if (this->_hbInfos._heartbeatInterval - i < 1000)
				std::this_thread::sleep_for(std::chrono::milliseconds(this->_hbInfos._heartbeatInterval - i));
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		while (!this->_disconnected && this->_webSocket.isOpen()) {
			this->_heartbeat(true);
			for (size_t i = 0; !this->_disconnected && this->_webSocket.isOpen() && i < this->_hbInfos._heartbeatInterval; i += 1000) {
				if (this->_hbInfos._heartbeatInterval - i < 1000)
					std::this_thread::sleep_for(std::chrono::milliseconds(this->_hbInfos._heartbeatInterval - i));
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}

	void Client::_handleWebSocket()
	{
		this->_logger.debug("Listening for gateway payloads");
		this->_treatWebSocketPayloads();
	}

	void Client::_ready(JsonValue &val)
	{
		this->_guilds = {};
		for (auto &elem : val.to<JsonObject>()["guilds"]->to<JsonArray>().value())
			this->_guilds.emplace_back(elem->to<JsonObject>()["id"]->to<JsonString>().value());
		if (this->_handlers.ready)
			try {
				this->_handlers.ready(*this);
			} catch (std::exception &e) {
				#ifdef __GNUG__
				this->_logger.error("Caught exception in onready function: " + getLastExceptionName() + "(" + e.what() + ")");
				#else
				this->_logger.error("Caught exception in onready function: " + e.what());
				#endif
			}
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
		for (unsigned i = 0; i < this->_cachedGuilds.size(); i++)
			if (this->_cachedGuilds[i]->id == val.to<JsonObject>()["id"]->to<JsonString>().value()) {
				delete this->_cachedGuilds[i];
				this->_cachedGuilds.erase(this->_cachedGuilds.begin() + i);
			}
		this->_cachedGuilds.emplace_back(new Guild(*this, val.to<JsonObject>()));
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