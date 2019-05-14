#include "Guild.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"
#include "endpoints.hpp"
#include "PrivateChannel.hpp"

#include <iostream>

namespace DisCXXord
{
	Guild::Guild(Client &client, json obj) :
		Snowflake(client, obj)
	{
		if (!obj["unavailable"].is_null()) {
			this->available = !obj["unavailable"];
			this->complete = true;
		} else {
			this->available = true;
			this->complete = false;
		}

		if (!this->available)
			return;

		//TODO: presences? *	array of partial presence update objects	presences of the users in the guild

		if (!obj["roles"].is_null()) {
			for (auto &val : obj["roles"]) {
				this->roles.emplace_back(client, val);
			}
		} else {
			json array = client.makeApiRequest(GUILDS_ENDPT"/" + this->id + ROLES_ENDPT);

			for (json &val : array)
				this->roles.emplace_back(client, val);
		}

		if (!obj["members"].is_null()) {
			for (auto &val : obj["members"]) {
				this->members.emplace_back(*this, client.getUser(val["user"]), val);
			}
		} else {
			json array = client.makeApiRequest(GUILDS_ENDPT"/" + this->id + MEMBER_ENDPT + "?limit=1000");

			for (auto &val : array)
				this->members.emplace_back(*this, client.getUser(val["user"]), val);
		}

		if (this->complete) {
			this->joinedAt = Date(obj["joined_at"].get<std::string>());

			this->large = obj["large"];
			this->memberCount = obj["member_count"];

			//TODO: voice_states? *	array of partial voice state objects	(without the guild_id key)

			for (auto &val : obj["channels"]) {
				this->channels.emplace_back(&this->_parent.getChannel(val, *this));
			}
		}

		this->name = obj["name"];

		if (!obj["icon"].is_null())
			this->icon = obj["icon"];

		if (!obj["splash"].is_null())
			this->splash = obj["splash"];

		if (!obj["permissions"].is_null())
			this->permissions[client.me().id] = obj["permissions"];

		this->region = obj["region"];

		this->afkTimeout = obj["afk_timeout"];

		if (!obj["embed_enabled"].is_null())
			this->embedsEnabled = obj["embed_enabled"];

		//TODO: embed_channel_id?	snowflake	if not null, the channel id that the widget will generate an invite to

		this->verificationLevel = static_cast<VerificationLvl>(obj["verification_level"]);
		this->defaultMsgNotif = static_cast<MsgNotifLvl>(obj["default_message_notifications"]);
		this->exlicitContentFilter = static_cast<ExplicitContentFilterLvl>(obj["explicit_content_filter"]);
		this->mfaLvl = static_cast<MFALvl>(obj["mfa_level"]);


		//TODO: emojis	array of emoji objects	custom guild emojis

		for (auto &val : obj["features"])
			this->features.emplace_back(val);

		//TODO: application_id	?snowflake	application id of the guild creator if it is bot-created

		if (!obj["widget_enabled"].is_null())
			this->widgetEnabled = obj["widget_enabled"];

		this->owner = &client.getUser(obj["owner_id"]);

		//TODO: max_presences	?integer	the maximum amount of presences for the guild
		//TODO: max_members	integer	the maximum amount of members for the guild
		//TODO: widget_channel_id?	snowflake	the channel id for the server widget
		//TODO: system_channel_id	?snowflake	the id of the channel to which system messages are sent
		//TODO: afk_channel_id		?snowflake	id of afk channel

		if (!obj["vanity_url_code"].is_null())
			this->vanityURLCode = obj["vanity_url_code"];

		if (!obj["description"].is_null())
			this->description = obj["description"];

		if (!obj["banner"].is_null())
			this->banner = obj["banner"];
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
			json val = this->_parent.makeApiRequest(GUILDS_ENDPT + this->id + MEMBER_ENDPT + id);

			this->members.emplace_back(*this, this->_parent.getUser(id), val);
			return this->members.back();
		} catch (APIErrorException &) {
			throw MemberNotFoundException("Cannot find member " + id);
		}
	}

	Channel &Guild::getChannel(const std::string &id)
	{
		for (Channel *channel : this->channels)
			if (channel->id == id)
				return *channel;

		json val = this->_parent.makeApiRequest(GUILDS_ENDPT + this->id + CHANNEL_ENDPT);

		for (auto &value : val) {
			if (value["id"] == id) {
				this->channels.emplace_back(&this->_parent.getChannel(val));
				return *this->channels.back();
			}
		}
		throw ChannelNotFoundException("Cannot find channel " + id);
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