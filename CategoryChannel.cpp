#include "CategoryChannel.hpp"

namespace DisCXXord
{
	CategoryChannel::CategoryChannel(DisCXXord::Client &client, json val, Guild &guild) :
		Channel(client, val, GUILD_CATEGORY)
	{
	}

	CategoryChannel::CategoryChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, GUILD_CATEGORY)
	{
	}

	Message &CategoryChannel::send(const SendingMessage &)
	{
		throw InvalidChannelException("Cannot send a message in a CategoryChannel");
	}

	Message &CategoryChannel::getMessage(const std::string &)
	{
		throw InvalidChannelException("Cannot get a message from a CategoryChannel");
	}

	void CategoryChannel::cacheMessage(Message *)
	{
		throw InvalidChannelException("Cannot cache a message in a CategoryChannel");
	}

	Message& CategoryChannel::getLastMessage()
	{
		throw InvalidChannelException("Cannot get a message from a CategoryChannel");
	}
}