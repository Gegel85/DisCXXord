#include "nlohmann/json.hpp"
#include "Snowflake.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	Snowflake::Snowflake(Client &client, json obj) :
		_parent(client),
		id(obj["id"]),
		createdAt((std::stoll(this->id) >> 22) + 1420070400000)
	{
	}

	std::string Snowflake::timestamp() const
	{
		return this->createdAt.toISO();
	}

	bool Snowflake::operator==(DisCXXord::Snowflake &comp)
	{
		return this->id == comp.id && typeid(comp) == typeid(*this);
	}
}