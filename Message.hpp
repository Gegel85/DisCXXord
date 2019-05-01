#ifndef DISCXXORD_MESSAGE_HPP
#define DISCXXORD_MESSAGE_HPP


#include "Snowflake.hpp"
#include "Embed.hpp"
#include "Member.hpp"
#include "Attachement.hpp"
#include "Reaction.hpp"

namespace DisCXXord
{
	class Channel;

	class Message : public Snowflake {
	private:
		std::vector<Message>	_cachedMessages;

	public:
		enum Type {
			DEFAULT,
			RECIPENT_ADD,
			RECIPENT_REMOVE,
			CALL,
			CHANNEL_NAME_CHANGE,
			CHANNEL_ICON_CHANGE,
			CHANNEL_PINED_MESSAGE,
			GUILD_MEMBER_JOIN
		};

		struct Mention {
			User		&user;
			Optional<Member>member;
		};

		Channel			&channel;
		Optional<Embed>		embed;
		std::string		content;
		PartialUser		author;
		Optional<Member>	member;
		Optional<User>		user;
		Optional<Guild>		guild;
		Optional<Date>		editedAt;
		Type			type;
		bool			tts;
		bool			mentionEveryone;
		std::vector<Mention>	mentions;
		std::vector<Role *>	mentionRoles;
		std::vector<Attachement>attachements;
		std::vector<Embed>	embeds;
		std::vector<Reaction>	reactions;
		Optional<Snowflake>	nonce;
		bool			pinned;
		Optional<Snowflake>	webhookId;

		Message(Client &client, json val, Channel &channel);
		Message(Client &client, json val);
	};
}

#include "Guild.hpp"

#endif //DISCXXORD_MESSAGE_HPP
