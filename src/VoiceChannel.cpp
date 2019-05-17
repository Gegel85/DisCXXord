#include "VoiceChannel.hpp"

namespace DisCXXord
{
	VoiceChannel::VoiceChannel(Client &client, json val, Guild &guild) :
		Channel(client, val, GUILD_VOICE)
	{
	}

	VoiceChannel::VoiceChannel(Client &client, json val) :
		Channel(client, val, GUILD_VOICE)
	{
	}

	Message &VoiceChannel::send(const SendingMessage &)
	{
		throw InvalidChannelException("Cannot send a message in a VoiceChannel");
	}

	Message &VoiceChannel::getMessage(const std::string &)
	{
		throw InvalidChannelException("Cannot get a message from a VoiceChannel");
	}

	void VoiceChannel::cacheMessage(Message *)
	{
		throw InvalidChannelException("Cannot cache a message in a VoiceChannel");
	}

	Message &VoiceChannel::getLastMessage()
	{
		throw InvalidChannelException("Cannot get a message from a VoiceChannel");
	}

	void VoiceChannel::startTyping()
	{
		throw InvalidChannelException("Cannot type in VoiceChannel");
	}
}