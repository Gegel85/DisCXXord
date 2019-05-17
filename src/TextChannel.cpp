#include "TextChannel.hpp"
#include "Client.hpp"
#include "endpoints.hpp"

#include <iostream>
namespace DisCXXord
{
	TextChannel::TextChannel(DisCXXord::Client &client, json val, Guild &guild) :
		Channel(client, val, GUILD_TEXT),
		rateLimit(0),
		guild(guild)
	{
		this->name = val["name"];
		this->position = val["position"];
		this->nsfw = val["nsfw"].is_null() ? false : val["nsfw"].get<bool>();

		Channel::pos.reset(&this->position);
		Channel::nsfw.reset(&this->nsfw);
		Channel::name.reset(&this->name);
		Channel::topic.reset(&this->topic);
		Channel::guild.reset(&this->guild);
		Channel::rateLimit.reset(&this->rateLimit);
		Channel::permissions.reset(&this->permissions);

		if (!val["last_message_id"].is_null())
			this->_lastMsgId = val["last_message_id"];

		if (!val["topic"].is_null())
			this->topic = val["topic"];

		if (!val["parent_id"].is_null())
			this->parentId = Snowflake(client, {{"id", val["parent_id"]}});

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

		Channel::pos.reset(&this->position);
		Channel::nsfw.reset(&this->nsfw);
		Channel::name.reset(&this->name);
		Channel::topic.reset(&this->topic);
		Channel::guild.reset(&this->guild);
		Channel::rateLimit.reset(&this->rateLimit);
		Channel::permissions.reset(&this->permissions);

		if (!val["last_message_id"].is_null())
			this->_lastMsgId = val["last_message_id"];

		if (!val["parent_id"].is_null())
			this->parentId = Snowflake(client, {{"id", val["parent_id"]}});

		if (!val["topic"].is_null())
			this->topic = val["topic"];

		for (auto &obj : val["permission_overwrites"])
			this->permissions.emplace_back(obj);
	}

	Message &TextChannel::send(const SendingMessage &msg)
	{
		if (msg.files.empty()) {
			this->_parent.logger.info("Create string");
			std::string request = "{"
			 	R"("tts":)" + std::string(msg.tts ? "true" : "false") + ","
				R"("embed":)" + msg.embed.dump() + ","
				R"("content":")" + msg.content + '"' +
				(msg.nonce.empty() ? "" : R"("nonce":")" + msg.nonce + '"') +
			"}";

			this->_parent.logger.info("Make request");
			this->_cachedMessages.emplace_back(new Message{this->_parent, this->_parent.makeApiRequest(CHANNEL_ENDPT"/" + this->id + MSG_ENDPT, request, "POST")});
			return *this->_cachedMessages.back();
		}
		throw NotImplementedException("TextChannel::send(file)"); //TODO: files
	}

	Message &TextChannel::getMessage(const std::string &id)
	{
		for (auto &msg : this->_cachedMessages)
			if (msg->id == id)
				return *msg;
		try {
			json value = this->_parent.makeApiRequest(CHANNEL_ENDPT"/" + this->id + MSG_ENDPT"/" + id);

			this->_cachedMessages.emplace_back(new Message{this->_parent, value, *this});
			return *this->_cachedMessages.back();
		} catch (APIErrorException &) {
			throw MessageNotFoundException("Cannot find any message with id " + id);
		}
	}

	Message &TextChannel::getLastMessage()
	{
		return this->getMessage(this->_lastMsgId);
	}

	void TextChannel::cacheMessage(Message *msg)
	{
		this->_cachedMessages.emplace_back(msg);
	}

	void TextChannel::startTyping()
	{
		this->_parent.makeApiRequest(CHANNEL_ENDPT"/" + this->id + TYPING_ENDPT, "", "POST");
	}
}
