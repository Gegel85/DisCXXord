#ifndef DISCXXORD_PRIVATECHANNEL_HPP
#define DISCXXORD_PRIVATECHANNEL_HPP


#include "Channel.hpp"
#include "Message.hpp"

namespace DisCXXord
{
	class PrivateChannel : public Channel {
	private:
		std::vector<SharedPtr<Message>>	_cachedMessages;
		std::string			_lastMsgId;

	public:
		PrivateChannel(Client &client, json val);
		~PrivateChannel() override = default;

		Message &send(const SendingMessage &content) override;
		Message &getMessage(const std::string &id) override;
		Message &getLastMessage() override;
		void cacheMessage(Message *) override;

		User &user;
	};
}

#endif //DISCXXORD_PRIVATECHANNEL_HPP
