//
// Created by Gegel85 on 09/04/2019.
//

#ifndef DISC_ORD_CLIENT_HPP
#define DISC_ORD_CLIENT_HPP


#include <thread>
#include <optional>
#include <functional>
#include <JsonObject.hpp>
#include "SecuredWebSocket.hpp"
#include "Logger.hpp"
#include "User.hpp"
#include "Guild.hpp"

namespace DisCXXord
{
	class Client {
	public:
		struct clientHandlers {
			void (*ready)(Client &);
		};

		explicit Client(const std::string &logpath = "./disc++ord.log", Logger::LogLevel level = Logger::INFO);
		~Client();
		const User &me();
		const std::vector<std::string> &guilds();
		const User &getUser(const std::string &id);
		void setHandlers(clientHandlers handl);
		void disconnect();
		void run(const std::string &username, const std::string &password);
		void run(const std::string &token);
		User &makeUser(JsonObject &obj);

	private:
		std::unique_ptr<JsonValue> _makeApiRequest(const std::string &endpt, const std::string &method = "GET", const std::string &body = "");
		std::optional<std::string> _timedGetAnswer(int time);
		void _connect();
		void _heartbeatLoop();
		void _handleWebSocket();
		void _identify();
		void _heartbeat(bool waitAnswer = false);
		void _treatWebSocketPayloads();
		void _handlePayload(JsonObject &);

		//Events functions
		void _ready(JsonValue &val);
		void _resumed(JsonValue &val);
		void _channelCreate(JsonValue &val);
		void _channelUpdate(JsonValue &val);
		void _channelDelete(JsonValue &val);
		void _channelPinsUpdate(JsonValue &val);
		void _guildCreate(JsonValue &val);
		void _guildUpdate(JsonValue &val);
		void _guildDelete(JsonValue &val);
		void _guildBanAdd(JsonValue &val);
		void _guildBanRemove(JsonValue &val);
		void _guildEmojisUpdate(JsonValue &val);
		void _guildIntegrationsUpdate(JsonValue &val);
		void _guildMemberAdd(JsonValue &val);
		void _guildMemberRemove(JsonValue &val);
		void _guildMemberUpdate(JsonValue &val);
		void _guildMembersChunk(JsonValue &val);
		void _guildRoleCreate(JsonValue &val);
		void _guildRoleUpdate(JsonValue &val);
		void _guildRoleDelete(JsonValue &val);
		void _messageCreate(JsonValue &val);
		void _messageUpdate(JsonValue &val);
		void _messageDelete(JsonValue &val);
		void _messageDeleteBulk(JsonValue &val);
		void _messageReactionAdd(JsonValue &val);
		void _messageReactionRemove(JsonValue &val);
		void _messageReactionRemoveAll(JsonValue &val);
		void _presenceUpdate(JsonValue &val);
		void _typingStart(JsonValue &val);
		void _userUpdate(JsonValue &val);
		void _voiceStateUpdate(JsonValue &val);
		void _voiceServerUpdate(JsonValue &val);
		void _webhooksUpdate(JsonValue &val);

		std::map<std::string, std::function<void(JsonValue &)>> _dispatchEvents = {
			{"READY",			[this](JsonValue &val) { this->_ready(val); }},
			{"RESUMED",			[this](JsonValue &val) { this->_resumed(val); }},
			{"CHANNEL_CREATE",		[this](JsonValue &val) { this->_channelCreate(val); }},
			{"CHANNEL_UPDATE",		[this](JsonValue &val) { this->_channelUpdate(val); }},
			{"CHANNEL_DELETE",		[this](JsonValue &val) { this->_channelDelete(val); }},
			{"CHANNEL_PINS_UPDATE",		[this](JsonValue &val) { this->_channelPinsUpdate(val); }},
			{"GUILD_CREATE",		[this](JsonValue &val) { this->_guildCreate(val); }},
			{"GUILD_UPDATE",		[this](JsonValue &val) { this->_guildUpdate(val); }},
			{"GUILD_DELETE",		[this](JsonValue &val) { this->_guildDelete(val); }},
			{"GUILD_BAN_ADD",		[this](JsonValue &val) { this->_guildBanAdd(val); }},
			{"GUILD_BAN_REMOVE",		[this](JsonValue &val) { this->_guildBanRemove(val); }},
			{"GUILD_EMOJIS_UPDATE",		[this](JsonValue &val) { this->_guildEmojisUpdate(val); }},
			{"GUILD_INTEGRATIONS_UPDATE",	[this](JsonValue &val) { this->_guildIntegrationsUpdate(val); }},
			{"GUILD_MEMBER_ADD",		[this](JsonValue &val) { this->_guildMemberAdd(val); }},
			{"GUILD_MEMBER_REMOVE",		[this](JsonValue &val) { this->_guildMemberRemove(val); }},
			{"GUILD_MEMBER_UPDATE",		[this](JsonValue &val) { this->_guildMemberUpdate(val); }},
			{"GUILD_MEMBERS_CHUNK",		[this](JsonValue &val) { this->_guildMembersChunk(val); }},
			{"GUILD_ROLE_CREATE",		[this](JsonValue &val) { this->_guildRoleCreate(val); }},
			{"GUILD_ROLE_UPDATE",		[this](JsonValue &val) { this->_guildRoleUpdate(val); }},
			{"GUILD_ROLE_DELETE",		[this](JsonValue &val) { this->_guildRoleDelete(val); }},
			{"MESSAGE_CREATE",		[this](JsonValue &val) { this->_messageCreate(val); }},
			{"MESSAGE_UPDATE",		[this](JsonValue &val) { this->_messageUpdate(val); }},
			{"MESSAGE_DELETE",		[this](JsonValue &val) { this->_messageDelete(val); }},
			{"MESSAGE_DELETE_BULK",		[this](JsonValue &val) { this->_messageDeleteBulk(val); }},
			{"MESSAGE_REACTION_ADD",	[this](JsonValue &val) { this->_messageReactionAdd(val); }},
			{"MESSAGE_REACTION_REMOVE",	[this](JsonValue &val) { this->_messageReactionRemove(val); }},
			{"MESSAGE_REACTION_REMOVE_ALL",	[this](JsonValue &val) { this->_messageReactionRemoveAll(val); }},
			{"PRESENCE_UPDATE",		[this](JsonValue &val) { this->_presenceUpdate(val); }},
			{"TYPING_START",		[this](JsonValue &val) { this->_typingStart(val); }},
			{"USER_UPDATE",			[this](JsonValue &val) { this->_userUpdate(val); }},
			{"VOICE_STATE_UPDATE",		[this](JsonValue &val) { this->_voiceStateUpdate(val); }},
			{"VOICE_SERVER_UPDATE",		[this](JsonValue &val) { this->_voiceServerUpdate(val); }},
			{"WEBHOOKS_UPDATE",		[this](JsonValue &val) { this->_webhooksUpdate(val); }},
			{"PRESENCES_REPLACE",		[] (JsonValue &) {}}
		};

		struct HeartbeatInfos {
			size_t						_heartbeatInterval;
			std::thread					_heartbeatThread;
			std::optional<int>				_lastSValue;
			std::chrono::_V2::system_clock::time_point	_lastHeartbeat;
			bool						_isAcknoledged;
			int						_nbNotAcknoledge;
		};

		std::optional<User>	_me;
		std::vector<User>	_cachedUsers;
		std::vector<Guild>	_cachedGuilds;
		std::vector<std::string>_guilds;
		std::string 		_token;
		SecuredWebSocket	_webSocket;
		clientHandlers		_handlers;
		Logger			_logger;
		HeartbeatInfos		_hbInfos;
		bool			_disconnected = false;
	};
}


#endif //DISC_ORD_CLIENT_HPP
