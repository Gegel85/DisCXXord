#include "Guild.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"
#include "endpoints.hpp"

namespace DisCXXord
{
	Guild::Guild(Client &client, JsonObject &obj) :
		Snowflake(client, obj)
	{
		try {
			this->available = !obj["unavailable"]->to<JsonBoolean>().value();

			if (!this->available)
				return;
			this->joinedAt = Date(obj["joined_at"]->to<JsonString>().value());

			this->large = obj["large"]->to<JsonBoolean>().value();
			this->memberCount = obj["member_count"]->to<JsonNumber>().value();

			//TODO: voice_states? *	array of partial voice state objects	(without the guild_id key)
			//TODO: channels? *	array of channel objects	channels in the guild
			//TODO: presences? *	array of partial presence update objects	presences of the users in the guild

		} catch (std::out_of_range &) {
			this->available = true;
			this->complete = false;
		}

		try {
			for (auto &val : obj["roles"]->to<JsonArray>().value()) {
				try {
					this->roles.emplace_back(
						Role(client, val->to<JsonObject>())
					);
				} catch (std::out_of_range &e) {
					throw std::out_of_range('\r' + std::string(e.what()));
				}
			}
		} catch (std::out_of_range &e) {
			if (e.what()[0] == '\r')
				throw std::out_of_range(&e.what()[1]);
			auto value = client.makeApiRequest(GUILDS_ENDPT"/" + this->id + ROLES_ENDPT);
			auto &array = value->to<JsonArray>();

			for (auto &val : array.value())
				this->roles.emplace_back(
					Role(client, val->to<JsonObject>())
				);
		}

		try {
			auto &array = obj["members"]->to<JsonArray>();

			for (auto &val : array.value()) {
				try {
					this->members.emplace_back(
						Member(
							*this,
							client.getUser(
								val->to<JsonObject>()["user"]->to<JsonObject>()
							),
							val->to<JsonObject>()
						)
					);
				} catch (std::out_of_range &e) {
					throw std::out_of_range('\r' + std::string(e.what()));
				}
			}
		} catch (std::out_of_range &e) {
			if (e.what()[0] == '\r')
				throw std::out_of_range(&e.what()[1]);
			auto value = client.makeApiRequest(GUILDS_ENDPT"/" + this->id + MEMBER_ENDPT + "?limit=1000");
			auto &array = value->to<JsonArray>();

			for (auto &val : array.value())
				this->members.emplace_back(
					Member(
						*this,
						client.getUser(
							val->to<JsonObject>()["user"]->to<JsonObject>()
						),
						val->to<JsonObject>()
					)
				);
		}

		this->name = obj["name"]->to<JsonString>().value();

		if (!obj["icon"]->is<JsonNull>())
			this->icon = obj["icon"]->to<JsonString>().value();

		if (!obj["splash"]->is<JsonNull>())
			this->splash = obj["splash"]->to<JsonString>().value();

		try {
			this->permissions[client.me().id] = obj["permissions"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {}

		this->region = obj["region"]->to<JsonString>().value();

		this->afkTimeout = obj["afk_timeout"]->to<JsonNumber>().value();

		try {
			this->embedsEnabled = obj["embed_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		//TODO: embed_channel_id?	snowflake	if not null, the channel id that the widget will generate an invite to

		this->verificationLevel = static_cast<VerificationLvl>(obj["verification_level"]->to<JsonNumber>().value());
		this->defaultMsgNotif = static_cast<MsgNotifLvl>(obj["default_message_notifications"]->to<JsonNumber>().value());
		this->exlicitContentFilter = static_cast<ExplicitContentFilterLvl>(obj["explicit_content_filter"]->to<JsonNumber>().value());
		this->mfaLvl = static_cast<MFALvl>(obj["mfa_level"]->to<JsonNumber>().value());


		//TODO: emojis	array of emoji objects	custom guild emojis

		for (auto &val : obj["features"]->to<JsonArray>().value())
			this->features.emplace_back(val->to<JsonString>().value());

		//TODO: application_id	?snowflake	application id of the guild creator if it is bot-created

		try {
			this->widgetEnabled = obj["widget_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		this->owner = &client.getUser(obj["owner_id"]->to<JsonString>().value());

		//TODO: max_presences	?integer	the maximum amount of presences for the guild
		//TODO: max_members	integer	the maximum amount of members for the guild
		//TODO: widget_channel_id?	snowflake	the channel id for the server widget
		//TODO: system_channel_id	?snowflake	the id of the channel to which system messages are sent
		//TODO: afk_channel_id		?snowflake	id of afk channel

		if (!obj["vanity_url_code"]->is<JsonNull>())
			this->vanityURLCode = obj["vanity_url_code"]->to<JsonString>().value();

		if (!obj["description"]->is<JsonNull>())
			this->description = obj["description"]->to<JsonString>().value();

		if (!obj["banner"]->is<JsonNull>())
			this->banner = obj["banner"]->to<JsonString>().value();
	}

	Role &Guild::getRole(const std::string &id)
	{
		for (Role &role : this->roles)
			if (role.id == id)
				return role;
		//TODO: Try to get from API
		throw RoleNotFoundException("Cannot find role " + id);
	}

	Role &Guild::getRoleByName(const std::string &name)
	{
		for (Role &role : this->roles)
			if (role.name == name)
				return role;
		throw RoleNotFoundException("Cannot find role " + name);
	}

	Member &Guild::getMember(const std::string &id)
	{
		for (Member &member : this->members)
			if (member.user.id == id)
				return member;
		try {
			auto val = this->_parent.makeApiRequest(GUILDS_ENDPT + this->id + MEMBER_ENDPT + id);

			this->members.emplace_back(Member(*this, this->_parent.getUser(id), val->to<JsonObject>()));
			return this->members.back();
		} catch (APIErrorException &) {
			throw MemberNotFoundException("Cannot find member " + id);
		}
	}

	Channel &Guild::getChannel(const std::string &id)
	{
		for (Channel &channel : this->channels)
			if (channel.id == id)
				return channel;
		//TODO: Try to get from API
		throw ChannelNotFoundException("Connot find channel " + id);
	}

	int Guild::getPermissions(const std::string &id) const
	{
		try {
			return this->permissions.at(id);
		} catch (std::out_of_range &e) {
			//TODO: Try to get from API
			throw;
		}
	}
}