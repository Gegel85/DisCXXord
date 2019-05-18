//
// Created by Gegel85 on 09/04/2019.
//

#include <string>
#include "nlohmann/json.hpp"
#include "defines.hpp"
#include "Exceptions.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "endpoints.hpp"
#include "TextChannel.hpp"
#include "VoiceChannel.hpp"
#include "PrivateChannel.hpp"

#ifndef _WINDOWS_DISCXXORD
#	include <sys/select.h>
	typedef fd_set FD_SET;
#endif

using json = nlohmann::json;

#ifdef __GNUG__
#include <cxxabi.h>
#endif

#include <iostream>

namespace DisCXXord
{
	#ifdef __GNUG__
	std::string getLastExceptionName()
	{
		int status;
		char *value;
		std::string name;

		if (!abi::__cxa_current_exception_type())
			return "No exception";
		auto val = abi::__cxa_current_exception_type();

		if (!val)
			return "No exception";
		value = abi::__cxa_demangle(val->name(), nullptr, nullptr, &status);
		name = value;
		free(value);
		return name;
	}
	#endif

//Public
	Client::Client(const std::string &logpath, DisCXXord::Logger::LogLevel level) :
		logger(logpath, level),
		_handlers()
	{
	}

	Client::~Client()
	{
		if (this->_webSocket.isOpen())
			this->_webSocket.disconnect();
	}

	void Client::disconnect()
	{
		this->logger.info("Disconnecting...");
		this->_webSocket.disconnect();
		this->_disconnected = true;
	}

	void Client::run(const std::string &token)
	{
		if (this->_running)
			throw RunningException("This client is already running");
		this->_running = true;
		this->_token = token;
		try {
			this->_connect();
			this->_handleWebSocket();
		} catch (std::exception &e) {
			#ifdef __GNUG__
			this->logger.critical("Caught exception: " + getLastExceptionName() + ": " + e.what());
			#else
			this->logger.critical("Caught exception: " + e.what());
			#endif
			try {
				this->disconnect();
			} catch (std::exception &) {}
			this->_disconnected = true;
		}
		if (this->_hbInfos._heartbeatThread.joinable())
			this->_hbInfos._heartbeatThread.join();
		this->_running = false;
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

	User &Client::getUser(json user)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		if (user.is_string())
			return this->getUser(user.get<std::string>());
		for (auto &user_it : this->_cachedUsers)
			if (user_it->id == user["id"])
				return *user_it;
		try {
			this->_cachedUsers.emplace_back(new User(*this, user));
			return *this->_cachedUsers.back();
		} catch (APIErrorException &) {
			throw UserNotFoundException("Cannot find user " + user["id"].get<std::string>());
		}
	}

	User &Client::getUser(const char *id)
	{
		return this->getUser(std::string(id));
	}

	User &Client::getUser(const std::string &id)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (auto &user : this->_cachedUsers)
			if (user->id == id)
				return *user;
		try {
			this->_cachedUsers.emplace_back(new User(*this, this->makeApiRequest(GET_USER_ENDPT"/" + id)));
			return *this->_cachedUsers.back();
		} catch (APIErrorException &) {
			throw UserNotFoundException("Cannot find user " + id);
		}
	}

	PrivateChannel &Client::getPrivateChannel(const std::string &user_id)
	{
		for (auto &channel : this->_cachedChannels)
			if (channel->is(Channel::DM) && channel->owner->id == user_id)
				return channel->to<PrivateChannel>();

		json val = this->makeApiRequest(USER_ME_ENDPT CHANNEL_ENDPT, R"({"recipient_id":")" + user_id + "\"}", "POST");
		PrivateChannel *channel = new PrivateChannel(*this, val);

		this->_cachedChannels.emplace_back(channel);
		return *channel;
	}

	PrivateChannel &Client::getPrivateChannel(const DisCXXord::User &user)
	{
		return this->getPrivateChannel(user.id);
	}

	Guild &Client::getGuild(const std::string &id)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (auto &guild : this->_cachedGuilds)
			if (guild->id == id)
				return *guild;
		try {
			this->_cachedGuilds.emplace_back(new Guild(*this, this->makeApiRequest(GUILDS_ENDPT"/" + id)));
			return *this->_cachedGuilds.back();
		} catch (APIErrorException &) {
			throw GuildNotFoundException("Cannot find guild " + id);
		}
	}

	Channel &Client::getChannel(json val, Guild &guild)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (auto &channel : this->_cachedChannels)
			if (channel->id == val["id"])
				return *channel;
		try {
			this->_cachedChannels.emplace_back(this->_createChannel(val, guild));
			return *this->_cachedChannels.back();
		} catch (APIErrorException &) {
			throw ChannelNotFoundException("Cannot find channel " + val["id"].get<std::string>());
		}
	}

	Channel &Client::getChannel(const std::string &id)
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		for (auto &channel : this->_cachedChannels)
			if (channel->id == id)
				return *channel;
		try {
			this->_cachedChannels.emplace_back(this->_createChannel(this->makeApiRequest(CHANNEL_ENDPT"/" + id)));
			return *this->_cachedChannels.back();
		} catch (APIErrorException &) {
			throw ChannelNotFoundException("Cannot find channel " + id);
		}
	}

	const std::vector<std::string> &Client::guilds()
	{
		if (this->_disconnected)
			throw DisconnectedException("You need to be connected to use this");
		return this->_guilds;
	}

	json Client::makeApiRequest(const std::string &endpt, const std::string &body, const std::string &method)
	{
		Request::HttpRequest	result;

	        result.method = method;
		result.url = API_BASE_URL + endpt;
		result.body = body;
		result.headers = {
			{"Authorization", this->_token},
			{"User-Agent", "DiscordBot (" LIBLINK ", " VERSION ")"}
		};

		if (!body.empty())
			result.headers["Content-Type"] = "application/json";
		result.headers["Content-Length"] = std::to_string(body.size());
		this->logger.debug(method + ": " + API_BASE_URL + endpt + " \"" + body + "\"");
		result = Request::request(result);
		//TODO: Handle 429 and 502
		//TODO: Handle global rate limit
		if (result.code >= 400) {
			this->logger.error(method + ": " API_BASE_URL + endpt + ": " + std::to_string(result.code) + " " + result.codeName);
			throw APIErrorException(method + ": " API_BASE_URL + endpt + ": " + std::to_string(result.code) + " " + result.codeName, result.body, result.code);
		}
		this->logger.debug(std::to_string(result.code) + " (" + result.codeName + "): \"" + result.body + "\"");
		return result.code == 204 ? "null"_json : json::parse(result.body);
	}




//Private
	Channel *Client::_createChannel(json value, Guild &guild)
	{
		auto type = static_cast<Channel::Type>(value["type"]);

		switch (type) {
			case Channel::GUILD_TEXT:
				return new TextChannel(*this, value, guild);
			case Channel::GUILD_CATEGORY:
				return new CategoryChannel(*this, value, guild);
			case Channel::GUILD_VOICE:
				return new VoiceChannel(*this, value, guild);
			default:
				throw InvalidChannelException("Cannot create a channel of type " + std::to_string(type) + " (" + Channel::typeToString(type) + ") in a guild");
		}
	}

	Channel *Client::_createChannel(json value)
	{
		auto type = static_cast<Channel::Type>(value["type"]);

		switch (type) {
		case Channel::GUILD_TEXT:
			return new TextChannel(*this, value);
		case Channel::GUILD_CATEGORY:
			return new CategoryChannel(*this, value);
		case Channel::GUILD_VOICE:
			return new VoiceChannel(*this, value);
		case Channel::DM:
			return new PrivateChannel(*this, value);
		default:
			throw InvalidChannelException("Cannot create a channel of type " + std::to_string(type) + " (" + Channel::typeToString(type) + ")");
		}
	}

	void Client::_heartbeat(bool waitAnswer)
	{
		std::string	payload = "{"
			"\"op\": 1,"
			"\"d\": " + (this->_hbInfos._lastSValue ? std::to_string(*this->_hbInfos._lastSValue) : "null") +
		"}";

		if (!this->_hbInfos._isAcknoledged)
			this->logger.warning("Server didn't acknowledged previous heartbeat");
		this->logger.debug("Sending heartbeat");
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

	void Client::_handlePayload(json &object)
	{
		switch (object["op"].get<int>()) {
		case 0:
			this->logger.debug("Got payload " + object["t"].get<std::string>());
			this->_hbInfos._lastSValue = object["s"];
			this->_dispatchEvents[object["t"]](
				object["d"]
			);
			break;
		case 1:
			this->logger.debug("Server requests heartbeat");
			this->_heartbeat();
			break;
		case 11:
			this->logger.debug("Server acknowledged heartbeat");
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
				this->logger.debug("Reading server answer");
				json object = json::parse(this->_webSocket.getAnswer());

				this->logger.debug("Server sent opcode " + std::to_string(static_cast<int>(object["op"])));
				this->_handlePayload(object);
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
			return {this->_webSocket.getAnswer()};
		return {};
	}

	void Client::_connect()
	{
		this->logger.info(LIBNAME " version " VERSION);
		this->logger.debug("Getting current user");
		this->_me.emplace(*this, this->makeApiRequest(USER_ME_ENDPT));
		this->logger.info("Connected on " + this->_me->tag());
		this->logger.debug("Fetching gateway URL");

		json object = this->makeApiRequest(GATEWAY_ENDPT);
		std::string url = object["url"];

		this->logger.info("Connecting to Gateway");
		this->_webSocket.connect(url.substr(6, url.size() - 6), 443);

		this->logger.debug("Waiting HELLO payload");

		try {
			object = json::parse(*this->_timedGetAnswer(30));
		} catch (EmptyValueException &) {
			throw TimeoutException("Gateway didn't reply to the identify request");
		}

		if (object["op"] == 10)
			this->logger.info("Received HELLO from gateway");
		else
			throw std::invalid_argument("Gateway didn't send Hello");
		this->_hbInfos._heartbeatInterval = static_cast<size_t>(object["d"]["heartbeat_interval"]);
		this->logger.debug("Default heartbeat interval is " + std::to_string(this->_hbInfos._heartbeatInterval));
		this->logger.debug("Sending Identify event");
		this->_identify();
		this->logger.debug("Setting heartbeating thread");
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
		this->logger.debug("Listening for gateway payloads");
		this->_treatWebSocketPayloads();
	}

	void Client::_ready(json &val)
	{
		this->_guilds = {};
		for (auto &elem : val["guilds"])
			this->_guilds.emplace_back(elem["id"]);
		if (this->_handlers.ready)
			try {
				this->_handlers.ready(*this);
			} catch (CorruptedCacheException &) {
				throw;
			} catch (std::exception &e) {
				#ifdef __GNUG__
				this->logger.error("Caught exception in onReady function: " + getLastExceptionName() + ": " + e.what());
				#else
				this->logger.error("Caught exception in onready function: " + e.what());
				#endif
			}
	}

	void Client::_resumed(json &val)
	{

	}

	void Client::_channelCreate(json &val)
	{

	}

	void Client::_channelUpdate(json &val)
	{

	}

	void Client::_channelDelete(json &val)
	{

	}

	void Client::_channelPinsUpdate(json &val)
	{

	}

	void Client::_guildCreate(json &val)
	{
		for (unsigned i = 0; i < this->_cachedGuilds.size(); i++)
			if (this->_cachedGuilds[i]->id == val["id"])
				this->_cachedGuilds.erase(this->_cachedGuilds.begin() + i);
		this->_cachedGuilds.emplace_back(new Guild(*this, val));
	}

	void Client::_guildUpdate(json &val)
	{

	}

	void Client::_guildDelete(json &val)
	{

	}

	void Client::_guildBanAdd(json &val)
	{

	}

	void Client::_guildBanRemove(json &val)
	{

	}

	void Client::_guildEmojisUpdate(json &val)
	{

	}

	void Client::_guildIntegrationsUpdate(json &val)
	{

	}

	void Client::_guildMemberAdd(json &val)
	{

	}

	void Client::_guildMemberRemove(json &val)
	{

	}

	void Client::_guildMemberUpdate(json &val)
	{

	}

	void Client::_guildMembersChunk(json &val)
	{

	}

	void Client::_guildRoleCreate(json &val)
	{

	}

	void Client::_guildRoleUpdate(json &val)
	{

	}

	void Client::_guildRoleDelete(json &val)
	{

	}

	void Client::_messageCreate(json &val)
	{
		Message *msg = new Message{*this, val};

		msg->channel.cacheMessage(msg);
		if (this->_handlers.messageCreate)
			try {
				this->_handlers.messageCreate(*this, *msg);
			} catch (CorruptedCacheException &) {
				throw;
			} catch (std::exception &e) {
				#ifdef __GNUG__
				this->logger.error("Caught exception in onMessageCreate function: " + getLastExceptionName() + ": " + e.what());
				#else
				this->logger.error("Caught exception in onready function: " + e.what());
				#endif
			}
	}

	void Client::_messageUpdate(json &val)
	{

	}

	void Client::_messageDelete(json &val)
	{

	}

	void Client::_messageDeleteBulk(json &val)
	{

	}

	void Client::_messageReactionAdd(json &val)
	{

	}

	void Client::_messageReactionRemove(json &val)
	{

	}

	void Client::_messageReactionRemoveAll(json &val)
	{

	}

	void Client::_presenceUpdate(json &val)
	{

	}

	void Client::_typingStart(json &val)
	{

	}

	void Client::_userUpdate(json &val)
	{

	}

	void Client::_voiceStateUpdate(json &val)
	{

	}

	void Client::_voiceServerUpdate(json &val)
	{

	}

	void Client::_webhooksUpdate(json &val)
	{

	}
}
