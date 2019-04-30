#include "VoiceChannel.hpp"

namespace DisCXXord
{
	VoiceChannel::VoiceChannel(DisCXXord::Client &client, json val, Guild &guild) :
		Channel(client, val, GUILD_VOICE)
	{
	}

	VoiceChannel::VoiceChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, GUILD_VOICE)
	{
	}

	Message VoiceChannel::send(const DisCXXord::Embed __attribute__((unused)) &embed, const std::string __attribute__((unused)) &content)
	{
		throw InvalidChannelException("Cannot send a message in a VoiceChannel");
	}

	Message VoiceChannel::getMessage(const std::string __attribute__((unused)) &id)
	{
		throw InvalidChannelException("Cannot get a message from a VoiceChannel");
	}
}