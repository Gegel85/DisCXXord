#include "Message.hpp"

namespace DisCXXord {
	Message::Message(Client &client, json val) :
		Snowflake(client, val),
		author{client, val["author"]}
	{}
}