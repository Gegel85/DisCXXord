#include "TextChannel.hpp"
#include "Client.hpp"
#include "endpoints.hpp"

namespace DisCXXord
{
	TextChannel::TextChannel(DisCXXord::Client &client, json val, Guild &guild) :
		Channel(client, val, GUILD_TEXT),
		rateLimit(0),
		guild(guild)
	{
		this->name = val["name"];
		this->position = val["position"];
		this->nsfw = val["nsfw"];

		Channel::pos = &this->position;
		Channel::nsfw = &this->nsfw;
		Channel::name = &this->name;
		Channel::topic = &this->topic;
		Channel::guild = &this->guild;
		Channel::rateLimit = &this->rateLimit;
		Channel::permissions = &this->permissions;

		if (!val["last_message_id"].is_null())
			try {
				this->lastMessage = this->getMessage(val["last_message_id"]);
			} catch (MessageNotFoundException &) {}

		if (!val["topic"].is_null())
			this->topic = val["topic"];

		if (!val["parent_id"].is_null())
			this->parent = &dynamic_cast<CategoryChannel &>(client.getChannel(val["parent_id"]));

		if (!val["rate_limit_per_user"].is_null())
			this->rateLimit = val["rate_limit_per_user"];

		for (auto &obj : val["permission_overwrites"])
			this->permissions.emplace_back(obj);
	}

	TextChannel::TextChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, GUILD_TEXT),
		guild(client.getGuild(val["guild_id"]))
	{
		this->name = val["name"];
		this->position = val["position"];
		this->rateLimit = val["rate_limit_per_user"];
		this->nsfw = val["nsfw"];
		this->topic = val["topic"];

		Channel::pos = &this->position;
		Channel::nsfw = &this->nsfw;
		Channel::name = &this->name;
		Channel::topic = &this->topic;
		Channel::guild = &this->guild;
		Channel::rateLimit = &this->rateLimit;
		Channel::permissions = &this->permissions;

		if (!val["last_message_id"].is_null())
			try {
				this->lastMessage = this->getMessage(val["last_message_id"]);
			} catch (MessageNotFoundException &) {}

		if (!val["parent_id"].is_null())
			this->parent = &dynamic_cast<CategoryChannel &>(client.getChannel(val["parent_id"]));

		for (auto &obj : val["permission_overwrites"])
			this->permissions.emplace_back(obj);
	}

	Message TextChannel::send(const DisCXXord::Embed &embed, const std::string &content)
	{
		//TODO: Envoyer des messages
		throw std::exception();
	}

	Message TextChannel::getMessage(const std::string &id)
	{
		for (Message &msg : this->_cachedMessages)
			if (msg.id == id)
				return msg;
		try {
			json value = this->_parent.makeApiRequest(CHANNEL_ENDPT"/" + this->id + MSG_ENDPT"/" + id);

			this->_cachedMessages.emplace_back(this->_parent, value, *this);
			return this->_cachedMessages.back();
		} catch (APIErrorException &) {
			throw MessageNotFoundException("Cannot find any message with id " + id);
		}
	}
}
