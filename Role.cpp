#include "Role.hpp"

namespace DisCXXord
{
	Role::Role(Client &client, JsonObject &obj) :
		Snowflake(client, obj)
	{
		this->name = obj["name"]->to<JsonString>().value();
		this->color = obj["color"]->to<JsonNumber>().value();
		this->hoist = obj["hoist"]->to<JsonBoolean>().value();
		this->pos = obj["position"]->to<JsonNumber>().value();
		this->permissions = obj["permissions"]->to<JsonNumber>().value();
		this->managed = obj["managed"]->to<JsonBoolean>().value();
		this->mentionable = obj["mentionable"]->to<JsonBoolean>().value();
	}

	bool Role::hasPermission(unsigned perm)
	{
		return this->permissions.has(perm);
	}
}