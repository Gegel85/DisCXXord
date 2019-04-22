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
		enum MsgNotifLvl {
			ALL_MESSAGE,
			ONLY_MENTIONS
		};

		enum ExplicitContentFilterLvl {
			DISABLED,
			MEMBERS_WITHOUT_ROLES,
			ALL_MEMBERS
		};

		enum MFALvl {
			NO_MFA,
			ELEVATED
		};

		enum VerificationLvl {
			NO_VERIFICATION,
			LOW,
			MEDIUM,
			HIGH,
			VERY_HIGH
		};

		int afkTimeout;
		int memberCount;
		bool large;
		bool embedsEnabled;
		bool widgetEnabled;
		bool available;
		Date joinedAt;
		MFALvl mfaLvl;
		const User *owner;
		MsgNotifLvl defaultMsgNotif;
		std::string name;
		std::string region;
		VerificationLvl verificationLevel;
		std::vector<Role> roles;
		std::vector<Member> members;
		std::vector<Channel> channels;
		std::optional<Channel> afkChannel;
		std::optional<Channel> embedsChannel;
		std::optional<Channel> widgetChannel;
		std::optional<Channel> systemChannel;
		std::vector<std::string> features;
		ExplicitContentFilterLvl exlicitContentFilter;
		std::optional<std::string> icon;
		std::optional<std::string> banner;
		std::optional<std::string> splash;
		std::optional<std::string> description;
		std::optional<std::string> vanityURLCode;
		std::map<std::string, int> permissions;

		Guild(Client &client, JsonObject &obj);
		const Role &getRole(const std::string &id) const;
		const Role &getRoleByName(const std::string &name) const;
		const Member &getMember(const std::string &id) const;
		const Channel &getChannel(const std::string &id) const;
		int getPermissions(const std::string &id) const;
	};
}


#endif //DISCXXORD_GUILD_HPP
