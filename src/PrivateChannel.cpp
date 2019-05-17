#include "PrivateChannel.hpp"
#include "Client.hpp"
#include "endpoints.hpp"

namespace DisCXXord
{
	PrivateChannel::PrivateChannel(DisCXXord::Client &client, json val) :
		Channel(client, val, DM),
		user(client.getUser(val["recipients"][0]["id"]))
	{
		Channel::owner.reset(&this->user);
	}

	Message &PrivateChannel::send(const SendingMessage &msg)
	{
		if (msg.files.empty()) {
			this->_parent.logger.info("Create string");
			std::string request = "{"
				R"("tts":)" + std::string(msg.tts ? "true" : "false") + ","
				R"("embed":)" + msg.embed.dump() + ","
				R"("content":")" + msg.content + '"' +
				(msg.nonce.empty() ? "" : R"("nonce":")" + msg.nonce + '"') +
			"}";

			this->_cachedMessages.emplace_back(new Message{this->_parent, this->_parent.makeApiRequest(CHANNEL_ENDPT"/" + this->id + MSG_ENDPT, request, "POST")});
			return *this->_cachedMessages.back();
		}
		throw NotImplementedException("PrivateChannel::send(file");
	}

	Message &PrivateChannel::getMessage(const std::string &id)
	{
		for (auto &msg : this->_cachedMessages)
			if (msg->id == id)
				return *msg;
		//TODO: Get from API
		throw MessageNotFoundException("Cannot find any message with id " + id);
	}

	Message &PrivateChannel::getLastMessage()
	{
		return this->getMessage(this->_lastMsgId);
	}

	void PrivateChannel::cacheMessage(Message *msg)
	{
		this->_cachedMessages.emplace_back(msg);
	}

	void PrivateChannel::startTyping()
	{
		this->_parent.makeApiRequest(CHANNEL_ENDPT"/" + this->id + TYPING_ENDPT, "", "POST");
	}
}