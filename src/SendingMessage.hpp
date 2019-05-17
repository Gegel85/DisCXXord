#ifndef DISCXXORD_SENDINGMESSAGE_HPP
#define DISCXXORD_SENDINGMESSAGE_HPP


#include "Embed.hpp"

namespace DisCXXord
{
	struct SendingMessage {
		std::string content{};
		Embed embed{};
		bool tts{false};
		std::string nonce{};
		std::vector <std::string> files{};
	};
}

#endif //DISCXXORD_SENDINGMESSAGE_HPP
