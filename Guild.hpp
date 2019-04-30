#ifndef DISCXXORD_GUILD_HPP
#define DISCXXORD_GUILD_HPP


#include "User.hpp"
#include "Snowflake.hpp"
#include "Role.hpp"
#include "Channel.hpp"
#include "Member.hpp"
#include "Optional.hpp"

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
		bool complete;
		bool channelsCached = false;
		Date joinedAt;
		User *owner;
		MFALvl mfaLvl;
		MsgNotifLvl defaultMsgNotif;
		std::string name;
		std::string region;
		VerificationLvl verificationLevel;
		std::vector<Role> roles;
		std::vector<Member> members;
		std::vector<Channel *> channels;
		Optional<Channel> afkChannel;
		Optional<Channel> embedsChannel;
		Optional<Channel> widgetChannel;
		Optional<Channel> systemChannel;
		std::vector<std::string> features;
		ExplicitContentFilterLvl exlicitContentFilter;
		Optional<std::string> icon;
		Optional<std::string> banner;
		Optional<std::string> splash;
		Optional<std::string> description;
		Optional<std::string> vanityURLCode;
		std::map<std::string, int> permissions;

		Guild(Client &client, json obj);
		Role &getRole(const std::string &id);
		Role &getRoleByName(const std::string &name);
		Member &getMember(const std::string &id);
		Channel &getChannel(const std::string &id);
		int getPermissions(const std::string &id) const;
	};
}


#endif //DISCXXORD_GUILD_HPP
