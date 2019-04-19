#include "Guild.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"

namespace DisCXXord
{
	Guild::Guild(Client &client, JsonObject &obj) :
		Snowflake(client, obj),
		joinedAt(obj["joined_at"]->to<JsonString>().value())
	{
		this->name = obj["name"]->to<JsonString>().value();

		if (!obj["icon"]->is<JsonNull>())
			this->icon = obj["icon"]->to<JsonString>().value();

		if (!obj["splash"]->is<JsonNull>())
			this->splash = obj["splash"]->to<JsonString>().value();

		try {
			this->permissions[client.me().id] = obj["permissions"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {}

		this->region = obj["region"]->to<JsonString>().value();

		//TODO: afk_channel_id	?snowflake	id of afk channel

		this->afkTimeout = obj["afk_timeout"]->to<JsonNumber>().value();

		try {
			this->embedsEnabled = obj["embed_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		//TODO: embed_channel_id?	snowflake	if not null, the channel id that the widget will generate an invite to

		this->verificationLevel = obj["verification_level"]->to<JsonNumber>().value();
		this->defaultMsgNotif = obj["default_message_notifications"]->to<JsonNumber>().value();
		this->exlicitContentFilter = obj["explicit_content_filter"]->to<JsonNumber>().value();

		//TODO: roles	array of role objects	roles in the guild
		//TODO: emojis	array of emoji objects	custom guild emojis

		for (auto &val : obj["features"]->to<JsonArray>().value())
			this->features.emplace_back(val->to<JsonString>().value());

		this->mfaLvl = obj["mfa_level"]->to<JsonNumber>().value();

		//TODO: application_id	?snowflake	application id of the guild creator if it is bot-created

		try {
			this->widgetEnabled = obj["widget_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		this->large = obj["large"]->to<JsonBoolean>().value();
		this->available = !obj["unavailable"]->to<JsonBoolean>().value();
		this->memberCount = obj["member_count"]->to<JsonNumber>().value();

		for (auto &val : obj["members"]->to<JsonArray>().value())
			this->members.emplace_back(
				Member(
					*this,
					client.makeUser(
						val->to<JsonObject>()["user"]->to<JsonObject>()
					),
					val->to<JsonObject>()
				)
			);

		this->owner = &client.getUser(obj["owner_id"]->to<JsonString>().value());

		//TODO: voice_states? *	array of partial voice state objects	(without the guild_id key)
		//TODO: channels? *	array of channel objects	channels in the guild
		//TODO: presences? *	array of partial presence update objects	presences of the users in the guild
		//TODO: max_presences	?integer	the maximum amount of presences for the guild
		//TODO: max_members	integer	the maximum amount of members for the guild
		//TODO: widget_channel_id?	snowflake	the channel id for the server widget
		//TODO: system_channel_id	?snowflake	the id of the channel to which system messages are sent

		if (!obj["vanity_url_code"]->is<JsonNull>())
			this->vanityURLCode = obj["vanity_url_code"]->to<JsonString>().value();

		if (!obj["description"]->is<JsonNull>())
			this->description = obj["description"]->to<JsonString>().value();

		if (!obj["banner"]->is<JsonNull>())
			this->banner = obj["banner"]->to<JsonString>().value();
	}

	const Role &Guild::getRole(std::string id) const
	{
		for (const Role &role : this->roles)
			if (role.id == id)
				return role;
		//TODO: Try to get from API
		throw RoleNotFoundException("Cannot find role " + id);
	}

	const Member &Guild::getMember(std::string id) const
	{
		for (const Member &member : this->members)
			if (member.user.id == id)
				return member;
		//TODO: Try to get from API
		throw MemberNotFoundException("Cannot find member " + id);
	}

	const Channel &Guild::getChannel(std::string id) const
	{
		for (const Channel &channel : this->channels)
			if (channel.id == id)
				return channel;
		//TODO: Try to get from API
		throw ChannelNotFoundException("Connot find channel " + id);
	}

	int Guild::getPermissions(std::string id) const
	{
		try {
			return this->permissions.at(id);
		} catch (std::out_of_range &e) {
			//TODO: Try to get from API
			throw;
		}
	}
}