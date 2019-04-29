#include "Member.hpp"

namespace DisCXXord
{
	Member::Member(Guild &guild, User &user, json object) :
		guild(guild),
		user(user),
		joinedAt(object["joined_at"].get<std::string>())
	{
		if (!object["nick"].is_null())
			this->nick = object["nick"];
		else
			this->nick = this->user.username;
		this->deafen = object["deaf"];
		this->muted = object["mute"];
	}
}