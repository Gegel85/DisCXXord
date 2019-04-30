#ifndef DISCXXORD_TEXTCHANNEL_HPP
#define DISCXXORD_TEXTCHANNEL_HPP


#include "Snowflake.hpp"
#include "Channel.hpp"

namespace DisCXXord
{
	class TextChannel : public Channel {
	public:
		TextChannel(Client &client, json val);
		~TextChannel() override = default;

		Message send(const Embed &embed, const std::string &content) override;
	};
}


#endif //DISCXXORD_TEXTCHANNEL_HPP
