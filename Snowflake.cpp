#include <JsonObject.hpp>
#include <JsonString.hpp>
#include "Snowflake.hpp"

namespace DisCXXord
{
	Snowflake::Snowflake(Client &client, JsonObject &obj) :
		_parent(client),
		id(obj["id"]->to<JsonString>().value()),
		createdAt((std::stoll(this->id) >> 22) + 1420070400000)
	{
	}

	std::string Snowflake::timestamp() const
	{
		return this->createdAt.toISO();
	}

	bool Snowflake::operator==(DisCXXord::Snowflake &comp)
	{
		return this->id == comp.id;
	}
}