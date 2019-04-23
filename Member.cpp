#include "Member.hpp"

namespace DisCXXord
{
	Member::Member(Guild &guild, User &user, JsonObject &object) :
		guild(guild),
		user(user),
		joinedAt(object["joined_at"]->to<JsonString>().value())
	{
		try {
			if (!object["nick"]->is<JsonNull>())
				this->nick = object["nick"]->to<JsonString>().value();
		} catch (std::out_of_range &) {
			this->nick = this->user.username;
		}
		this->deafen = object["deaf"]->to<JsonBoolean>().value();
		this->muted = object["mute"]->to<JsonBoolean>().value();
	}
}