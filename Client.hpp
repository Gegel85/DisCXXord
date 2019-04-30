//
// Created by Gegel85 on 09/04/2019.
//

#ifndef DISC_ORD_CLIENT_HPP
#define DISC_ORD_CLIENT_HPP


#include <thread>
#include <functional>
#include "Optional.hpp"
#include "SecuredWebSocket.hpp"
#include "Logger.hpp"
#include "User.hpp"
#include "Guild.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class Client {
	public:
		struct clientHandlers {
			void (*ready)(Client &);
			void (*messageCreate)(Client &, Message &);
		};

		explicit Client(const std::string &logpath = "./disc++ord.log", Logger::LogLevel level = Logger::INFO);
		~Client();
		User &me();
		User &getUser(json user);
		User &getUser(const std::string &id);
		Guild &getGuild(const std::string &id);
		Channel &getChannel(json val, Guild &guild);
		Channel &getChannel(const std::string &id);
		const std::vector<std::string> &guilds();
		void setHandlers(clientHandlers handl);
		void disconnect();
		void run(const std::string &username, const std::string &password);
		void run(const std::string &token);
		json makeApiRequest(const std::string &endpt, const std::string &method = "GET", const std::string &body = "");

	private:
		Optional<std::string> _timedGetAnswer(int time);
		Channel *_createChannel(json value);
		Channel *_createChannel(json value, Guild &guild);
		void _connect();
		void _heartbeatLoop();
		void _handleWebSocket();
		void _identify();
		void _heartbeat(bool waitAnswer = false);
		void _treatWebSocketPayloads();
		void _handlePayload(json &);

		//Events functions
		void _ready(json &val);
		void _resumed(json &val);
		void _channelCreate(json &val);
		void _channelUpdate(json &val);
		void _channelDelete(json &val);
		void _channelPinsUpdate(json &val);
		void _guildCreate(json &val);
		void _guildUpdate(json &val);
		void _guildDelete(json &val);
		void _guildBanAdd(json &val);
		void _guildBanRemove(json &val);
		void _guildEmojisUpdate(json &val);
		void _guildIntegrationsUpdate(json &val);
		void _guildMemberAdd(json &val);
		void _guildMemberRemove(json &val);
		void _guildMemberUpdate(json &val);
		void _guildMembersChunk(json &val);
		void _guildRoleCreate(json &val);
		void _guildRoleUpdate(json &val);
		void _guildRoleDelete(json &val);
		void _messageCreate(json &val);
		void _messageUpdate(json &val);
		void _messageDelete(json &val);
		void _messageDeleteBulk(json &val);
		void _messageReactionAdd(json &val);
		void _messageReactionRemove(json &val);
		void _messageReactionRemoveAll(json &val);
		void _presenceUpdate(json &val);
		void _typingStart(json &val);
		void _userUpdate(json &val);
		void _voiceStateUpdate(json &val);
		void _voiceServerUpdate(json &val);
		void _webhooksUpdate(json &val);

		std::map<std::string, std::function<void(json &)>> _dispatchEvents = {
			{"READY",			[this](json &val) { this->_ready(val); }},
			{"RESUMED",			[this](json &val) { this->_resumed(val); }},
			{"CHANNEL_CREATE",		[this](json &val) { this->_channelCreate(val); }},
			{"CHANNEL_UPDATE",		[this](json &val) { this->_channelUpdate(val); }},
			{"CHANNEL_DELETE",		[this](json &val) { this->_channelDelete(val); }},
			{"CHANNEL_PINS_UPDATE",		[this](json &val) { this->_channelPinsUpdate(val); }},
			{"GUILD_CREATE",		[this](json &val) { this->_guildCreate(val); }},
			{"GUILD_UPDATE",		[this](json &val) { this->_guildUpdate(val); }},
			{"GUILD_DELETE",		[this](json &val) { this->_guildDelete(val); }},
			{"GUILD_BAN_ADD",		[this](json &val) { this->_guildBanAdd(val); }},
			{"GUILD_BAN_REMOVE",		[this](json &val) { this->_guildBanRemove(val); }},
			{"GUILD_EMOJIS_UPDATE",		[this](json &val) { this->_guildEmojisUpdate(val); }},
			{"GUILD_INTEGRATIONS_UPDATE",	[this](json &val) { this->_guildIntegrationsUpdate(val); }},
			{"GUILD_MEMBER_ADD",		[this](json &val) { this->_guildMemberAdd(val); }},
			{"GUILD_MEMBER_REMOVE",		[this](json &val) { this->_guildMemberRemove(val); }},
			{"GUILD_MEMBER_UPDATE",		[this](json &val) { this->_guildMemberUpdate(val); }},
			{"GUILD_MEMBERS_CHUNK",		[this](json &val) { this->_guildMembersChunk(val); }},
			{"GUILD_ROLE_CREATE",		[this](json &val) { this->_guildRoleCreate(val); }},
			{"GUILD_ROLE_UPDATE",		[this](json &val) { this->_guildRoleUpdate(val); }},
			{"GUILD_ROLE_DELETE",		[this](json &val) { this->_guildRoleDelete(val); }},
			{"MESSAGE_CREATE",		[this](json &val) { this->_messageCreate(val); }},
			{"MESSAGE_UPDATE",		[this](json &val) { this->_messageUpdate(val); }},
			{"MESSAGE_DELETE",		[this](json &val) { this->_messageDelete(val); }},
			{"MESSAGE_DELETE_BULK",		[this](json &val) { this->_messageDeleteBulk(val); }},
			{"MESSAGE_REACTION_ADD",	[this](json &val) { this->_messageReactionAdd(val); }},
			{"MESSAGE_REACTION_REMOVE",	[this](json &val) { this->_messageReactionRemove(val); }},
			{"MESSAGE_REACTION_REMOVE_ALL",	[this](json &val) { this->_messageReactionRemoveAll(val); }},
			{"PRESENCE_UPDATE",		[this](json &val) { this->_presenceUpdate(val); }},
			{"TYPING_START",		[this](json &val) { this->_typingStart(val); }},
			{"USER_UPDATE",			[this](json &val) { this->_userUpdate(val); }},
			{"VOICE_STATE_UPDATE",		[this](json &val) { this->_voiceStateUpdate(val); }},
			{"VOICE_SERVER_UPDATE",		[this](json &val) { this->_voiceServerUpdate(val); }},
			{"WEBHOOKS_UPDATE",		[this](json &val) { this->_webhooksUpdate(val); }},
			{"PRESENCES_REPLACE",		[] (json &) {}}
		};

		struct HeartbeatInfos {
			size_t						_heartbeatInterval;
			std::thread					_heartbeatThread;
			Optional<int>				_lastSValue;
			std::chrono::_V2::system_clock::time_point	_lastHeartbeat;
			bool						_isAcknoledged;
			int						_nbNotAcknoledge;
		};

		Optional<User>	_me;
		std::vector<User *>	_cachedUsers;
		std::vector<Guild *>	_cachedGuilds;
		std::vector<Channel *>	_cachedChannels;
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
