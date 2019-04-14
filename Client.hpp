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

namespace DisCXXord {
	class Client {
	public:
		struct clientHandlers {
			void (*onready)();
		};

		Client(const std::string &logpath = "./disc++ord.log", Logger::LogLevel level = Logger::WARNING);
		~Client();
		void setHandlers(clientHandlers handl);
		void disconnect();
		void run(const std::string &username, const std::string &password);
		void run(const std::string &token);

	private:
		void _handleWebSocket();
		void _identify();
		void _heartbeat(bool waitAnswer = false);
		void _treatWebSocketPayloads();
		void _handlePayload(JsonObject &);

		//Events functions
		void _ready(JsonObject &val);
		void _resumed(JsonObject &val);
		void _channelCreate(JsonObject &val);
		void _channelUpdate(JsonObject &val);
		void _channelDelete(JsonObject &val);
		void _channelPinsUpdate(JsonObject &val);
		void _guildCreate(JsonObject &val);
		void _guildUpdate(JsonObject &val);
		void _guildDelete(JsonObject &val);
		void _guildBanAdd(JsonObject &val);
		void _guildBanRemove(JsonObject &val);
		void _guildEmojisUpdate(JsonObject &val);
		void _guildIntegrationsUpdate(JsonObject &val);
		void _guildMemberAdd(JsonObject &val);
		void _guildMemberRemove(JsonObject &val);
		void _guildMemberUpdate(JsonObject &val);
		void _guildMembersChunk(JsonObject &val);
		void _guildRoleCreate(JsonObject &val);
		void _guildRoleUpdate(JsonObject &val);
		void _guildRoleDelete(JsonObject &val);
		void _messageCreate(JsonObject &val);
		void _messageUpdate(JsonObject &val);
		void _messageDelete(JsonObject &val);
		void _messageDeleteBulk(JsonObject &val);
		void _messageReactionAdd(JsonObject &val);
		void _messageReactionRemove(JsonObject &val);
		void _messageReactionRemoveAll(JsonObject &val);
		void _presenceUpdate(JsonObject &val);
		void _typingStart(JsonObject &val);
		void _userUpdate(JsonObject &val);
		void _voiceStateUpdate(JsonObject &val);
		void _voiceServerUpdate(JsonObject &val);
		void _webhooksUpdate(JsonObject &val);

		std::map<std::string, std::function<void(JsonObject &)>> _dispatchEvents = {
			{"READY",			[this](JsonObject &val) { this->_ready(val); }},
			{"RESUMED",			[this](JsonObject &val) { this->_resumed(val); }},
			{"CHANNEL_CREATE",		[this](JsonObject &val) { this->_channelCreate(val); }},
			{"CHANNEL_UPDATE",		[this](JsonObject &val) { this->_channelUpdate(val); }},
			{"CHANNEL_DELETE",		[this](JsonObject &val) { this->_channelDelete(val); }},
			{"CHANNEL_PINS_UPDATE",		[this](JsonObject &val) { this->_channelPinsUpdate(val); }},
			{"GUILD_CREATE",		[this](JsonObject &val) { this->_guildCreate(val); }},
			{"GUILD_UPDATE",		[this](JsonObject &val) { this->_guildUpdate(val); }},
			{"GUILD_DELETE",		[this](JsonObject &val) { this->_guildDelete(val); }},
			{"GUILD_BAN_ADD",		[this](JsonObject &val) { this->_guildBanAdd(val); }},
			{"GUILD_BAN_REMOVE",		[this](JsonObject &val) { this->_guildBanRemove(val); }},
			{"GUILD_EMOJIS_UPDATE",		[this](JsonObject &val) { this->_guildEmojisUpdate(val); }},
			{"GUILD_INTEGRATIONS_UPDATE",	[this](JsonObject &val) { this->_guildIntegrationsUpdate(val); }},
			{"GUILD_MEMBER_ADD",		[this](JsonObject &val) { this->_guildMemberAdd(val); }},
			{"GUILD_MEMBER_REMOVE",		[this](JsonObject &val) { this->_guildMemberRemove(val); }},
			{"GUILD_MEMBER_UPDATE",		[this](JsonObject &val) { this->_guildMemberUpdate(val); }},
			{"GUILD_MEMBERS_CHUNK",		[this](JsonObject &val) { this->_guildMembersChunk(val); }},
			{"GUILD_ROLE_CREATE",		[this](JsonObject &val) { this->_guildRoleCreate(val); }},
			{"GUILD_ROLE_UPDATE",		[this](JsonObject &val) { this->_guildRoleUpdate(val); }},
			{"GUILD_ROLE_DELETE",		[this](JsonObject &val) { this->_guildRoleDelete(val); }},
			{"MESSAGE_CREATE",		[this](JsonObject &val) { this->_messageCreate(val); }},
			{"MESSAGE_UPDATE",		[this](JsonObject &val) { this->_messageUpdate(val); }},
			{"MESSAGE_DELETE",		[this](JsonObject &val) { this->_messageDelete(val); }},
			{"MESSAGE_DELETE_BULK",		[this](JsonObject &val) { this->_messageDeleteBulk(val); }},
			{"MESSAGE_REACTION_ADD",	[this](JsonObject &val) { this->_messageReactionAdd(val); }},
			{"MESSAGE_REACTION_REMOVE",	[this](JsonObject &val) { this->_messageReactionRemove(val); }},
			{"MESSAGE_REACTION_REMOVE_ALL",	[this](JsonObject &val) { this->_messageReactionRemoveAll(val); }},
			{"PRESENCE_UPDATE",		[this](JsonObject &val) { this->_presenceUpdate(val); }},
			{"TYPING_START",		[this](JsonObject &val) { this->_typingStart(val); }},
			{"USER_UPDATE",			[this](JsonObject &val) { this->_userUpdate(val); }},
			{"VOICE_STATE_UPDATE",		[this](JsonObject &val) { this->_voiceStateUpdate(val); }},
			{"VOICE_SERVER_UPDATE",		[this](JsonObject &val) { this->_voiceServerUpdate(val); }},
			{"WEBHOOKS_UPDATE",		[this](JsonObject &val) { this->_webhooksUpdate(val); }},
		};

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
		bool			_disconnected = false;
	};
}


#endif //DISC_ORD_CLIENT_HPP
