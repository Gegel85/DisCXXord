#include <JsonObject.hpp>
#include <JsonString.hpp>
#include "Snowflake.hpp"

namespace DisCXXord
{
	Snowflake::Snowflake(Client &client, JsonObject &obj) :
		_parent(client),
		_id(obj["id"]->to<JsonString>().value()),
		_createdAt((std::stol(this->_id) >> 22) + 1420070400000)
	{
	}

	const Date &Snowflake::createdAt() const
	{
		return this->_createdAt;
	}

	std::string Snowflake::timestamp() const
	{
		return this->createdAt().toISO();
	}

	std::string Snowflake::id() const
	{
		return this->_id;
	}
}