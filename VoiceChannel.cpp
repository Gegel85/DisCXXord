#include "VoiceChannel.hpp"

namespace DisCXXord
{
	VoiceChannel::VoiceChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, GUILD_VOICE)
	{
	}

	Message VoiceChannel::send(const DisCXXord::Embed &embed, const std::string &content)
	{
		throw InvalidChannelException("Cannot send a message in a VoiceChannel");
	}
}