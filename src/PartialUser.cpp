#include "PartialUser.hpp"

namespace DisCXXord
{
	PartialUser::PartialUser(Client &client, json obj) :
		Snowflake(client, obj)
	{
		this->username = obj["username"];

		if (!obj["avatar"].is_null())
			this->avatarHash = obj["avatar"];
	}
}