#include "Guild.hpp"
#include "Client.hpp"
#include "Exception.hpp"

namespace DisCXXord
{
	Guild::Guild(Client &client, JsonObject &obj) :
		Snowflake(client, obj),
		_joinedAt(obj["joined_at"]->to<JsonString>().value())
	{
		this->_name = obj["name"]->to<JsonString>().value();

		if (!obj["icon"]->is<JsonNull>())
			this->_icon = obj["icon"]->to<JsonString>().value();

		try {
			if (!obj["splash"]->is<JsonNull>())
				this->_splash = obj["splash"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		try {
			this->_permissions[client.me().id()] = obj["permissions"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {}

		this->_region = obj["region"]->to<JsonString>().value();

		//TODO: afk_channel_id	?snowflake	id of afk channel

		this->_afkTimeout = obj["afk_timeout"]->to<JsonNumber>().value();

		try {
			this->_embedsEnabled = obj["embed_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		//TODO: embed_channel_id?	snowflake	if not null, the channel id that the widget will generate an invite to

		this->_verificationLevel = obj["verification_level"]->to<JsonNumber>().value();
		this->_defaultMsgNotif = obj["default_message_notifications"]->to<JsonNumber>().value();
		this->_exlicitContentFilter = obj["explicit_content_filter"]->to<JsonNumber>().value();

		//TODO: roles	array of role objects	roles in the guild
		//TODO: emojis	array of emoji objects	custom guild emojis

		for (auto &val : obj["features"]->to<JsonArray>().value())
			this->_features.emplace_back(val->to<JsonString>().value());

		this->_mfaLvl = obj["mfa_level"]->to<JsonNumber>().value();

		//TODO: application_id	?snowflake	application id of the guild creator if it is bot-created

		try {
			this->_widgetEnabled = obj["widget_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		//TODO: widget_channel_id?	snowflake	the channel id for the server widget
		//TODO: system_channel_id	?snowflake	the id of the channel to which system messages are sent

		this->_large = obj["large"]->to<JsonBoolean>().value();
		this->_available = !obj["unavailable"]->to<JsonBoolean>().value();
		this->_memberCount = obj["member_count"]->to<JsonNumber>().value();

		for (auto &val : obj["members"]->to<JsonArray>().value())
			this->_members.emplace_back(
				Member(
					client,
					client.makeUser(
						val->to<JsonObject>()["user"]->to<JsonObject>()
					),
					val->to<JsonObject>()
				)
			);

		this->_owner = &client.getUser(obj["owner_id"]->to<JsonString>().value());

		//TODO: voice_states? *	array of partial voice state objects	(without the guild_id key)
		//TODO: channels? *	array of channel objects	channels in the guild
		//TODO: presences? *	array of partial presence update objects	presences of the users in the guild
		//TODO: max_presences	?integer	the maximum amount of presences for the guild
		//TODO: max_members	integer	the maximum amount of members for the guild

		try {
			if (obj["vanity_url_code"]->is<JsonNull>())
				this->_vanityURLCode = obj["vanity_url_code"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		try {
			if (obj["description"]->is<JsonNull>())
				this->_description = obj["description"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		try {
			if (obj["banner"]->is<JsonNull>())
				this->_banner = obj["banner"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}
	}

	int Guild::mfaLvl() const
	{
		return this->_mfaLvl;
	}

	int Guild::afkTimeout() const
	{
		return this->_afkTimeout;
	}

	int Guild::memberCount() const
	{
		return this->_memberCount;
	}

	int Guild::defaultMessageNotificationLevel() const
	{
		return this->_defaultMsgNotif;
	}

	int Guild::verificationLevel() const
	{
		return this->_verificationLevel;
	}

	int Guild::exlicitContentFilter() const
	{
		return this->_exlicitContentFilter;
	}

	bool Guild::large() const
	{
		return this->_large;
	}

	bool Guild::embedsEnabled() const
	{
		return this->_embedsEnabled;
	}
	bool Guild::widgetEnabled() const
	{
		return this->_widgetEnabled;
	}

	bool Guild::available() const
	{
		return this->_available;
	}

	const Date &Guild::joinedAt() const
	{
		return this->_joinedAt;
	}

	const User &Guild::owner() const
	{
		return *this->_owner;
	}

	std::string Guild::name() const
	{
		return this->_name;
	}

	std::string Guild::region() const
	{
		return this->_region;
	}

	const std::vector<Role> &Guild::roles() const
	{
		return this->_roles;
	}

	const Role &Guild::getRole(std::string id) const
	{
		for (const Role &role : this->_roles)
			if (role.id() == id)
				return role;
		throw RoleNotFoundException("Cannot find role " + id);
	}

	const std::vector<Member> &Guild::members() const
	{
		return this->_members;
	}

	const std::vector<Channel> &Guild::channels() const
	{
		return this->_channels;
	}

	const Member &Guild::getMember(std::string id) const
	{
		for (const Member &member : this->_members)
			if (member.user().id() == id)
				return member;
		throw MemberNotFoundException("Cannot find member " + id);
	}

	const Channel &Guild::getChannel(std::string id) const
	{
		for (const Channel &channel : this->_channels)
			if (channel.id() == id)
				return channel;
		throw ChannelNotFoundException("Connot find channel " + id);
	}

	const std::optional<Channel> &Guild::afkChannel() const
	{
		return this->_afkChannel;
	}

	const std::optional<Channel> &Guild::embedsChannel() const
	{
		return this->_embedsChannel;
	}

	const std::optional<Channel> &Guild::widgetChannel() const
	{
		return this->_widgetChannel;
	}

	const std::optional<Channel> &Guild::systemChannel() const
	{
		return this->_systemChannel;
	}

	const std::vector<std::string> &Guild::features() const
	{
		return this->_features;
	}

	const std::optional<std::string> &Guild::icon() const
	{
		return this->_icon;
	}

	const std::optional<std::string> &Guild::banner() const
	{
		return this->_banner;
	}

	const std::optional<std::string> &Guild::splash() const
	{
		return this->_splash;
	}

	const std::optional<std::string> &Guild::description() const
	{
		return this->_description;
	}

	const std::optional<std::string> &Guild::vanityURLCode() const
	{
		return this->_vanityURLCode;
	}

	const std::map<std::string, int> Guild::permissions() const
	{
		return this->_permissions;
	}

	int Guild::getPermissions(std::string id) const
	{
		try {
			return this->_permissions.at(id);
		} catch (std::out_of_range &e) {
			throw;
		}
	}
}