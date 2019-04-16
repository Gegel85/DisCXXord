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
	private:
		bool _bot;
		int _flags;
		bool _mfaEnabled;
		std::string _username;
		std::string _discriminator;
		std::optional<bool> _verified;
		std::optional<int> _premium_type;
		std::optional<std::string> _email;
		std::optional<std::string> _locale;
		std::optional<std::string> _avatarHash;

	public:
		User(Client &client, JsonObject &obj);
		int defaultAvatar() const;
		bool mfaEnabled() const;
		bool bot() const;
		int flags() const;
		std::string tag() const;
		std::string username() const;
		std::string avatarURL() const;
		std::string discriminator() const;
		std::string mentionString() const;
		std::string defaultAvatarURL() const;
		std::optional<bool> verified() const;
		std::optional<int> premium_type() const;
		std::optional<std::string> email() const;
		std::optional<std::string> locale() const;
		std::optional<std::string> avatar() const;
		std::string getDefaultAvatarURL(int size = 128) const;
		std::string getAvatarURL(int size = 128, const std::string &format = "png") const;
	};
}


#endif //DISC_ORD_USER_HPP
