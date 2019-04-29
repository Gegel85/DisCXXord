#include "Channel.hpp"

namespace DisCXXord
{
	Channel::Channel(Client &client, json obj, Type type) :
		Snowflake(client, obj),
		type(type)
	{
	}

	bool Channel::is(DisCXXord::Channel::Type expected) const
	{
		return this->type == expected;
	}
}