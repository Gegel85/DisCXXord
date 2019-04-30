#ifndef DISCXXORD_CATEGORYCHANNEL_HPP
#define DISCXXORD_CATEGORYCHANNEL_HPP


#include "Channel.hpp"

namespace DisCXXord
{
	class CategoryChannel : public Channel {
	public:
		CategoryChannel(Client &client, json val, Guild &guild);
		CategoryChannel(Client &client, json val);
		~CategoryChannel() override = default;

		Message send(const Embed &embed, const std::string &content) override;
		Message getMessage(const std::string &id) override;
	};
}


#endif //DISCXXORD_CATEGORYCHANNEL_HPP