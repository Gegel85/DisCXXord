#include "CategoryChannel.hpp"

namespace DisCXXord
{
	CategoryChannel::CategoryChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, GUILD_CATEGORY)
	{
	}

	Message CategoryChannel::send(const DisCXXord::Embed __attribute__((unused)) &embed, const __attribute__((unused)) std::string &content)
	{
		throw InvalidChannelException("Cannot send a message in a CategoryChannel");
	}
}