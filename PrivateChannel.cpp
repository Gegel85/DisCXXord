#include "PrivateChannel.hpp"

namespace DisCXXord
{
	PrivateChannel::PrivateChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, DM)
	{

	}

	Message PrivateChannel::send(const DisCXXord::Embed &embed, const std::string &content)
	{
		throw std::exception();
	}

	Message PrivateChannel::getMessage(const std::string __attribute__((unused)) &id)
	{
		for (Message &msg : this->_cachedMessages)
			if (msg.id == id)
				return msg;
		//TODO: Get from API
		throw MessageNotFoundException("Cannot find any message with id " + id);
	}
}