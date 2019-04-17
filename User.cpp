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
		this->username = obj["username"]->to<JsonString>().value();
		this->discriminator = obj["discriminator"]->to<JsonString>().value();

		try {
			this->bot = obj["bot"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		try {
			this->mfaEnabled = obj["mfa_enabled"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		try {
			this->verified = obj["verified"]->to<JsonBoolean>().value();
		} catch (std::out_of_range &) {}

		try {
			this->premium_type = obj["premium_type"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {}

		try {
			this->flags = obj["flags"]->to<JsonNumber>().value();
		} catch (std::out_of_range &) {}

		try {
			if (!obj["email"]->is<JsonNull>())
				this->email = obj["email"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		try {
			this->locale = obj["locale"]->to<JsonString>().value();
		} catch (std::out_of_range &) {}

		if (!obj["avatar"]->is<JsonNull>())
			this->avatarHash = obj["avatar"]->to<JsonString>().value();
	}

	int User::defaultAvatar() const
	{
		return std::stoi(this->discriminator) % 5;
	}

	std::string User::tag() const
	{
		return this->username + "#" + this->discriminator;
	}

	std::string User::mentionString() const
	{
		return "<@" + this->id + ">";
	}

	std::string User::defaultAvatarURL(int size) const
	{
		return IMAGE_ENDPOINT + ("embed/avatars/" + std::to_string(this->defaultAvatar())) + ".png?size=" + std::to_string(size);
	}

	std::string User::avatarURL(int size, const std::string &format) const
	{
		if (!this->avatarHash)
			return defaultAvatarURL(size);
		return IMAGE_ENDPOINT + ("avatars/" + this->id) + "/" + *this->avatarHash + "." + format + "?size=" + std::to_string(size);
	}
}