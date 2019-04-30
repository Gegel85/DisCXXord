#ifndef DISCXXORD_PRIVATECHANNEL_HPP
#define DISCXXORD_PRIVATECHANNEL_HPP


#include "Channel.hpp"

namespace DisCXXord
{
	class PrivateChannel : public Channel {
	private:
		std::vector<Message>	_cachedMessages;

	public:
		PrivateChannel(Client &client, json val);
		~PrivateChannel() override = default;

		Message send(const Embed &embed, const std::string &content) override;
		Message getMessage(const std::string &id) override;
	};
}

#endif //DISCXXORD_PRIVATECHANNEL_HPP
