#include "nlohmann/json.hpp"
#include "Snowflake.hpp"

#if INTPTR_MAX != INT64_MAX
#	error You need to compile the library in 64 bits
#endif

using json = nlohmann::json;

namespace DisCXXord
{
	Snowflake::Snowflake(Client &client, json obj) :
		_parent(client),
		id(obj["id"]),
		createdAt{static_cast<time_t>((std::stoll(this->id) >> 22) + 1420070400000)}
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