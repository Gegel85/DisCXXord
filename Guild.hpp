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
	private:
		int _mfaLvl;
		int _afkTimeout;
		int _memberCount;
		int _defaultMsgNotif;
		int _verificationLevel;
		int _exlicitContentFilter;
		bool _large;
		bool _embedsEnabled;
		bool _widgetEnabled;
		bool _available;
		Date _joinedAt;
		const User *_owner;
		std::string _name;
		std::string _region;
		std::vector<Role> _roles;
		std::vector<Member> _members;
		std::vector<Channel> _channels;
		std::optional<Channel> _afkChannel;
		std::optional<Channel> _embedsChannel;
		std::optional<Channel> _widgetChannel;
		std::optional<Channel> _systemChannel;
		std::vector<std::string> _features;
		std::optional<std::string> _icon;
		std::optional<std::string> _banner;
		std::optional<std::string> _splash;
		std::optional<std::string> _description;
		std::optional<std::string> _vanityURLCode;
		std::map<std::string, int> _permissions;

	public:
		Guild(Client &client, JsonObject &obj);
		int mfaLvl() const;
		int afkTimeout() const;
		int memberCount() const;
		int defaultMessageNotificationLevel() const;
		int verificationLevel() const;
		int exlicitContentFilter() const;
		bool large() const;
		bool embedsEnabled() const;
		bool widgetEnabled() const;
		bool available() const;
		const Date &joinedAt() const;
		const User &owner() const;
		std::string name() const;
		std::string region() const;
		const std::vector<Role> &roles() const;
		const Role &getRole(std::string id) const;
		const std::vector<Member> &members() const;
		const std::vector<Channel> &channels() const;
		const Member &getMember(std::string id) const;
		const Channel &getChannel(std::string id) const;
		const std::optional<Channel> &afkChannel() const;
		const std::optional<Channel> &embedsChannel() const;
		const std::optional<Channel> &widgetChannel() const;
		const std::optional<Channel> &systemChannel() const;
		const std::vector<std::string> &features() const;
		const std::optional<std::string> &icon() const;
		const std::optional<std::string> &banner() const;
		const std::optional<std::string> &splash() const;
		const std::optional<std::string> &description() const;
		const std::optional<std::string> &vanityURLCode() const;
		const std::map<std::string, int> permissions() const;
		int getPermissions(std::string id) const;
	};
}


#endif //DISCXXORD_GUILD_HPP
