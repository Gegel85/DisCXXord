//
// Created by Gegel85 on 15/04/2019.
//

#ifndef DISC_ORD_USER_HPP
#define DISC_ORD_USER_HPP


#include <string>
#include "Optional.hpp"
#include "Snowflake.hpp"
#include "nlohmann/json.hpp"
#include "PartialUser.hpp"
#include "Channel.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class Client;
	class PrivateChannel;
	class Message;

	class User : public PartialUser {
	private:
		Optional<PrivateChannel>_privateChannel;

	public:
		enum Flag {
			NONE		= 0,
			DISCORD_EMPLOYEE= 1 << 0,
			DISCORD_PARTNER	= 1 << 1,
			HYPESQUAD_EVENTS= 1 << 2,
			BUG_HUNTER	= 1 << 3,
			HOUSE_BRAVERY	= 1 << 6,
			HOUSE_BRILLIANCE= 1 << 7,
			HOUSE_BALANCE	= 1 << 8,
			EARLY_SUPPORTER	= 1 << 9,
		};

		int flags = 0;
		bool mfaEnabled = false;
		std::string discriminator;
		Optional<bool> verified;
		Optional<int> premium_type;
		Optional<std::string> email;
		Optional<std::string> locale;

		PrivateChannel &getPrivateChannel();
		Message &send(const SendingMessage &);

		User(Client &client, json obj);
		int defaultAvatar() const;
		std::string tag() const;
		std::string mentionString() const;
		bool hasFlag(Flag flag);
		std::string defaultAvatarURL(int size = 128) const;
		std::string avatarURL(int size = 128, const std::string &format = "png") const;
	};
}

#include "PrivateChannel.hpp"


#endif //DISC_ORD_USER_HPP
