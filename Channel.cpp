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

	Message Channel::send(const DisCXXord::Embed &embed)
	{
		return this->send(embed, "");
	}

	Message Channel::send(const std::string &content)
	{
		return this->send({}, content);
	}
}