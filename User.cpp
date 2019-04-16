//
// Created by Gegel85 on 15/04/2019.
//

#include <ctime>
#include "User.hpp"
#include "endpoints.hpp"

namespace DisCXXord
{
	User::User(Client &client, JsonObject &obj) : Snowflake(client, obj)
	{
		this->_username = obj["username"]->to<JsonString>().value();
		this->_discriminator = obj["discriminator"]->to<JsonString>().value();

		try {
			this->_bot = obj["bot"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {
			this->_bot = false;
		}

		try {
			this->_mfaEnabled = obj["mfa_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {
			this->_mfaEnabled = false;
		}

		try {
			this->_verified = obj["verified"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		try {
			this->_premium_type = obj["premium_type"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {}

		try {
			this->_flags = obj["flags"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {
			this->_flags = 0;
		}

		try {
			this->_email = obj["email"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		try {
			this->_locale = obj["locale"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		if (!obj["avatar"]->is<JsonNull>())
			this->_avatarHash = obj["avatar"]->to<JsonString>().value();
	}

	std::string User::discriminator() const
	{
		return this->_discriminator;
	}

	int User::defaultAvatar() const
	{
		return std::stoi(this->_discriminator) % 5;
	}

	bool User::bot() const
	{
		return this->_bot;
	}

	std::string User::tag() const
	{
		return this->_username + "#" + this->_discriminator;
	}

	std::string User::username() const
	{
		return this->_username;
	}

	std::string User::avatarURL() const
	{
		if (this->_avatarHash)
			return getAvatarURL();
		return defaultAvatarURL();
	}

	std::string User::mentionString() const
	{
		return "<@" + this->_id + ">";
	}

	std::string User::defaultAvatarURL() const
	{
		return this->getDefaultAvatarURL();
	}

	std::optional<bool> User::verified() const
	{
		return this->_verified;
	}

	std::optional<int> User::premium_type() const
	{
		return this->_premium_type;
	}

	std::optional<std::string> User::email() const
	{
		return this->_email;
	}

	std::optional<std::string> User::locale() const
	{
		return this->_locale;
	}

	std::optional<std::string> User::avatar() const
	{
		return this->_avatarHash;
	}

	std::string User::getDefaultAvatarURL(int size) const
	{
		return IMAGE_ENDPOINT + ("embed/avatars/" + std::to_string(this->defaultAvatar())) + ".png?size=" + std::to_string(size);
	}

	std::string User::getAvatarURL(int size, const std::string &format) const
	{
		return IMAGE_ENDPOINT + ("avatars/" + this->_id) + "/" + *this->_avatarHash + "." + format + "?size=" + std::to_string(size);
	}

	bool User::mfaEnabled() const
	{
		return this->_mfaEnabled;
	}

	int User::flags() const
	{
		return this->_flags;
	}
}