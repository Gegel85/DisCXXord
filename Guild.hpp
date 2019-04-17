#ifndef DISCXXORD_GUILD_HPP
#define DISCXXORD_GUILD_HPP


#include <optional>
#include "User.hpp"
#include "Snowflake.hpp"
#include "Role.hpp"
#include "Channel.hpp"
#include "Member.hpp"

namespace DisCXXord
{
	class Client;

	class Guild : public Snowflake {
	public:
		int mfaLvl;
		int afkTimeout;
		int memberCount;
		int defaultMsgNotif;
		int verificationLevel;
		int exlicitContentFilter;
		bool large;
		bool embedsEnabled;
		bool widgetEnabled;
		bool available;
		Date joinedAt;
		const User *owner;
		std::string name;
		std::string region;
		std::vector<Role> roles;
		std::vector<Member> members;
		std::vector<Channel> channels;
		std::optional<Channel> afkChannel;
		std::optional<Channel> embedsChannel;
		std::optional<Channel> widgetChannel;
		std::optional<Channel> systemChannel;
		std::vector<std::string> features;
		std::optional<std::string> icon;
		std::optional<std::string> banner;
		std::optional<std::string> splash;
		std::optional<std::string> description;
		std::optional<std::string> vanityURLCode;
		std::map<std::string, int> permissions;

		Guild(Client &client, JsonObject &obj);
		const Role &getRole(std::string id) const;
		const Member &getMember(std::string id) const;
		const Channel &getChannel(std::string id) const;
		int getPermissions(std::string id) const;
	};
}


#endif //DISCXXORD_GUILD_HPP
