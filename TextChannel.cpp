#include "TextChannel.hpp"

namespace DisCXXord
{
	TextChannel::TextChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, GUILD_TEXT)
	{
	}

	Message TextChannel::send(const DisCXXord::Embed &embed, const std::string &content)
	{
		//TODO: Envoyer des messages
		throw std::exception();
	}
}
