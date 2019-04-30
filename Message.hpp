#ifndef DISCXXORD_MESSAGE_HPP
#define DISCXXORD_MESSAGE_HPP


#include "Snowflake.hpp"
#include "Embed.hpp"
#include "Member.hpp"
#include "Attachement.hpp"

namespace DisCXXord
{
	class Message : public Snowflake {
	private:
		std::vector<Message>	_cachedMessages;

	public:
		Optional<Embed>		embed;
		std::string		content;
		PartialUser		author;
		Optional<Member>	member;
		Optional<Guild>		guild;
		Optional<Date>		editedAt;
		bool			tts;
		bool			mentionEveryone;
		std::vector<User>	mentions;
		std::vector<Role>	mentionRoles;
		std::vector<Attachement>attachements;
		std::vector<Embed>	embeds;


		Message(Client &client, json val);
	};
}

#include "Guild.hpp"

#endif //DISCXXORD_MESSAGE_HPP
