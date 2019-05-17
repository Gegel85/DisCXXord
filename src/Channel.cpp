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

	CategoryChannel &Channel::parent()
	{
		if (!this->guild)
			throw GuildNotFoundException("This channel is not in a guild");
		try {
			return dynamic_cast<CategoryChannel &>(this->guild->getChannel(this->parentId->id));
		} catch (std::bad_cast &) {
			throw CorruptedCacheException("The channel " + id + " is a parent but a category channel");
		}
	}
}