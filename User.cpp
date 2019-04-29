//
// Created by Gegel85 on 15/04/2019.
//

#include <ctime>
#include "User.hpp"
#include "endpoints.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	User::User(Client &client, json obj) : Snowflake(client, obj)
	{
		this->username = obj["username"];
		this->discriminator = obj["discriminator"];

		if (!obj["bot"].is_null())
			this->bot = obj["bot"];

		if (!obj["mfa_enabled"].is_null())
			this->mfaEnabled = obj["mfa_enabled"];

		if (!obj["verified"].is_null())
			this->verified = obj["verified"];

		if (!obj["premium_type"].is_null())
			this->premium_type = obj["premium_type"];

		if (!obj["flags"].is_null())
			this->flags = obj["flags"];

		if (!obj["email"].is_null())
			this->email = obj["email"];

		if (!obj["locale"].is_null())
			this->locale = obj["locale"];

		if (!obj["avatar"].is_null())
			this->avatarHash = obj["avatar"];
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
		return IMAGE_BASE_URL + ("/embed/avatars/" + std::to_string(this->defaultAvatar())) + ".png?size=" + std::to_string(size);
	}

	std::string User::avatarURL(int size, const std::string &format) const
	{
		if (!this->avatarHash)
			return defaultAvatarURL(size);
		return IMAGE_BASE_URL + ("/avatars/" + this->id) + "/" + *this->avatarHash + "." + format + "?size=" + std::to_string(size);
	}

	bool User::hasFlag(DisCXXord::User::Flag flag)
	{
		return (this->flags & flag) != 0;
	}
}