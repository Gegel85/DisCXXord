#ifndef DISCXXORD_VOICECHANNEL_HPP
#define DISCXXORD_VOICECHANNEL_HPP

#include "Message.hpp"
#include "Channel.hpp"
#include "Client.hpp"

namespace DisCXXord
{
	class VoiceChannel : public Channel {
	public:
		VoiceChannel(Client &client, json val, Guild &guild);
		VoiceChannel(Client &client, json val);
		~VoiceChannel() override = default;

		Message &send(const SendingMessage &msg) override;
		Message &getMessage(const std::string &id) override;
		Message &getLastMessage() override;
		void startTyping() override;
		void cacheMessage(Message *) override;
	};
}


#endif //DISCXXORD_VOICECHANNEL_HPP
