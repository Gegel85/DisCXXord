#ifndef DISCXXORD_VOICECHANNEL_HPP
#define DISCXXORD_VOICECHANNEL_HPP

#include "Message.hpp"
#include "Channel.hpp"
#include "Client.hpp"

namespace DisCXXord
{
	class VoiceChannel : public Channel {
	public:
		VoiceChannel(Client &client, json val);
		~VoiceChannel() override = default;

		Message send(const Embed &embed, const std::string &content) override;

	};
}


#endif //DISCXXORD_VOICECHANNEL_HPP
