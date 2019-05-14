#include "Message.hpp"
#include "Client.hpp"
#include "PrivateChannel.hpp"

namespace DisCXXord
{
	Message::Message(Client &client, json val, Channel &channel) :
		Snowflake(client, val),
		channel{channel},
		content{val["content"]},
		author{client, val["author"]},
		type{val["type"]},
		tts{val["tts"]},
		mentionEveryone{val["mention_everyone"]},
		pinned{val["pinned"]}
	{
		if (!val["guild_id"].is_null())
			this->guild = &client.getGuild(val["guild_id"]);

		if (!val["member"].is_null())
			this->member = &this->guild->getMember(this->author.id);

		if (!this->webhookId)
			this->user = &client.getUser(this->author.id);

		for (auto &mention : val["mention"]) {
			try {
				this->mentions.emplace_back(Mention{client.getUser(mention["id"]), &this->guild->getMember(mention["id"])});
			} catch (EmptyValueException &) {
				this->mentions.emplace_back(Mention{client.getUser(mention["id"]), nullptr});
			} catch (MemberNotFoundException &) {
				this->mentions.emplace_back(Mention{client.getUser(mention["id"]), nullptr});
			}
		}

		if (!this->guild && this->channel.guild)
			this->guild = &*this->channel.guild;

		for (auto &id : val["mention_roles"])
			this->mentionRoles.emplace_back(&this->guild->getRole(id));

		for (json &elem : val["embed"]) {
			Embed emb{elem};

			this->embeds.emplace_back(elem);
		}

		if (!this->embeds.empty())
			this->embed = &this->embeds.front();

		if (!val["webhook_id"].is_null())
			this->webhookId = Snowflake(client, val["webhook_id"]);

		if (!val["nonce"].is_null())
			this->webhookId = Snowflake(client, val["nonce"]);

		if (!val["edited_timestamp"].is_null())
			this->editedAt = Date(static_cast<time_t>(val["edited_timestamp"]));
	}

	Message::Message(Client &client, json val) :
		Snowflake(client, val),
		channel{client.getChannel(val["channel_id"])},
		content{val["content"]},
		author{client, val["author"]},
		type{val["type"]},
		tts{val["tts"]},
		mentionEveryone{val["mention_everyone"]},
		pinned{val["pinned"]}
	{
		if (!val["guild_id"].is_null())
			this->guild = &client.getGuild(val["guild_id"]);

		if (!val["member"].is_null())
			this->member = &this->guild->getMember(this->author.id);

		if (!this->webhookId)
			this->user = &client.getUser(this->author.id);

		for (auto &mention : val["mention"]) {
			try {
				this->mentions.emplace_back(Mention{client.getUser(mention["id"]), &this->guild->getMember(mention["id"])});
			} catch (EmptyValueException &) {
				this->mentions.emplace_back(Mention{client.getUser(mention["id"]), nullptr});
			} catch (MemberNotFoundException &) {
				this->mentions.emplace_back(Mention{client.getUser(mention["id"]), nullptr});
			}
		}

		if (!this->guild && this->channel.guild)
			this->guild = &*this->channel.guild;

		for (auto &id : val["mention_roles"])
			this->mentionRoles.emplace_back(&this->guild->getRole(id));

		//for (auto &elem : val["embed"])
		//	this->embeds.push_back(Embed{elem});

		if (!this->embeds.empty())
			this->embed = &this->embeds.front();

		if (!val["webhook_id"].is_null())
			this->webhookId = Snowflake(client, val["webhook_id"]);

		if (!val["nonce"].is_null())
			this->webhookId = Snowflake(client, {{"id", val["nonce"]}});

		if (!val["edited_timestamp"].is_null())
			this->editedAt = Date(static_cast<time_t>(val["edited_timestamp"]));
	}
}