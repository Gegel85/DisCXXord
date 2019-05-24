#include "PartialUser.hpp"
#include "Client.hpp"

namespace DisCXXord
{
	PartialUser::PartialUser(Client &client, json obj) :
		Snowflake(client, obj)
	{
		this->username = obj["username"];

		if (!obj["avatar"].is_null())
			this->avatarHash = obj["avatar"];
	}

	User& PartialUser::user()
	{
		User &obj = this->_parent.getUser(this->id);

		this->username = obj.username;
		this->avatarHash = obj.avatarHash;
		return obj;
	}
}