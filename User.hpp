//
// Created by Gegel85 on 15/04/2019.
//

#ifndef DISC_ORD_USER_HPP
#define DISC_ORD_USER_HPP


#include <string>
#include <optional>
#include <JsonParser.hpp>
#include "Snowflake.hpp"

namespace DisCXXord
{
	class Client;

	class User : public Snowflake {
	public:
		int flags = 0;
		bool bot = false;
		bool mfaEnabled = false;
		std::string username;
		std::string discriminator;
		std::optional<bool> verified;
		std::optional<int> premium_type;
		std::optional<std::string> email;
		std::optional<std::string> locale;
		std::optional<std::string> avatarHash;

		User(Client &client, JsonObject &obj);
		int defaultAvatar() const;
		std::string tag() const;
		std::string mentionString() const;
		std::string defaultAvatarURL(int size = 128) const;
		std::string avatarURL(int size = 128, const std::string &format = "png") const;
	};
}


#endif //DISC_ORD_USER_HPP
