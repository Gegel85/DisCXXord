//
// Created by Gegel85 on 15/04/2019.
//

#include <ctime>
#include "User.hpp"
#include "endpoints.hpp"

namespace DisCXXord
{
	User::User(JsonObject &obj)
	{
		this->_id = obj["id"]->to<JsonString>().value();
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
		} catch (std::out_of_range &) {
			this->_verified = {};
		}

		try {
			this->_premium_type = obj["premium_type"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {
			this->_premium_type = {};
		}

		try {
			this->_flags = obj["flags"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {
			this->_flags = 0;
		}

		try {
			this->_email = obj["email"]->to<JsonString>().value();
		} catch (std::out_of_range &) {
			this->_email = {};
		}

		try {
			this->_locale = obj["locale"]->to<JsonString>().value();
		} catch (std::out_of_range &) {
			this->_locale = {};
		}

		if (obj["avatar"]->is<JsonNull>())
			this->_avatarHash = {};
		else
			this->_avatarHash = obj["avatar"]->to<JsonString>().value();
	}

	time_t User::createdAt()
	{
		return (std::stol(this->_id) >> 22) + 1420070400000;
	}

	std::string User::discriminator()
	{
		return this->_discriminator;
	}

	int User::defaultAvatar()
	{
		return std::stoi(this->_discriminator) % 5;
	}

	bool User::bot()
	{
		return this->_bot;
	}

	std::string User::id()
	{
		return this->_id;
	}

	std::string User::tag()
	{
		return this->_username + "#" + this->_discriminator;
	}

	std::string User::username()
	{
		return this->_username;
	}

	std::string User::avatarURL()
	{
		if (this->_avatarHash)
			return getAvatarURL();
		return defaultAvatarURL();
	}

	std::string User::timestamp()
	{
		char buffer[22];
		time_t time = this->createdAt();

		strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%SZ", gmtime(&time));
		return buffer;
	}

	std::string User::mentionString()
	{
		return "<@" + this->_id + ">";
	}

	std::string User::defaultAvatarURL()
	{
		return this->getDefaultAvatarURL();
	}

	std::optional<bool> User::verified()
	{
		return this->_verified;
	}

	std::optional<int> User::premium_type()
	{
		return this->_premium_type;
	}

	std::optional<std::string> User::email()
	{
		return this->_email;
	}

	std::optional<std::string> User::locale()
	{
		return this->_locale;
	}

	std::optional<std::string> User::avatar()
	{
		return this->_avatarHash;
	}

	std::string User::getDefaultAvatarURL(int size)
	{
		return IMAGE_ENDPOINT + ("embed/avatars/" + std::to_string(this->defaultAvatar())) + ".png?size=" + std::to_string(size);
	}

	std::string User::getAvatarURL(int size, const std::string &format)
	{
		return IMAGE_ENDPOINT + ("avatars/" + this->_id) + "/" + *this->_avatarHash + "." + format + "?size=" + std::to_string(size);
	}

	bool User::mfaEnabled()
	{
		return this->_mfaEnabled;
	}

	int User::flags()
	{
		return this->_flags;
	}
}