#include "Role.hpp"

namespace DisCXXord
{
	Role::Role(Client &client, json obj) :
		Snowflake(client, obj)
	{
		this->name = obj["name"];
		this->color = obj["color"];
		this->hoist = obj["hoist"];
		this->pos = obj["position"];
		this->managed = obj["managed"];
		this->mentionable = obj["mentionable"];
		this->permissions = static_cast<int>(obj["permissions"]);
	}

	bool Role::hasPermission(unsigned perm)
	{
		return this->permissions.has(perm);
	}
}