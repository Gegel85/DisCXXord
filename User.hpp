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

using json = nlohmann::json;

namespace DisCXXord
{
	class Client;

	class User : public PartialUser {
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

		User(Client &client, json obj);
		int defaultAvatar() const;
		std::string tag() const;
		std::string mentionString() const;
		bool hasFlag(Flag flag);
		std::string defaultAvatarURL(int size = 128) const;
		std::string avatarURL(int size = 128, const std::string &format = "png") const;
	};
}


#endif //DISC_ORD_USER_HPP
