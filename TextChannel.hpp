#ifndef DISCXXORD_TEXTCHANNEL_HPP
#define DISCXXORD_TEXTCHANNEL_HPP


#include "Snowflake.hpp"
#include "Channel.hpp"

namespace DisCXXord
{
	class TextChannel : public Channel {
	private:
		std::vector<SharedPtr<Message>>	_cachedMessages;
		std::string			_lastMsgId;

	public:
		int position;
		int rateLimit;
		bool nsfw;
		Guild &guild;
		std::string name;
		std::string topic;
		std::vector<PermissionOverwrite> permissions;

		TextChannel(Client &client, json val, Guild &guild);
		TextChannel(Client &client, json val);
		~TextChannel() override = default;

		void startTyping() override;
		Message &send(const SendingMessage &content) override;
		Message &getMessage(const std::string &id) override;
		Message &getLastMessage() override;
		void cacheMessage(Message *msg) override;
	};
}


#endif //DISCXXORD_TEXTCHANNEL_HPP
