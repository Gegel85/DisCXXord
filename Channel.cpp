#include "Channel.hpp"

namespace DisCXXord
{
	Channel::Channel(Client &client, JsonObject &obj, Type type) :
		Snowflake(client, obj),
		type(type)
	{
	}
}