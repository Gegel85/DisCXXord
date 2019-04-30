#include "Attachement.hpp"

namespace DisCXXord
{
	Attachement::Attachement(Client &client, json obj) :
		Snowflake(client, obj),
		filename{obj["filename"]},
		size{obj["size"]},
		url{obj["url"]},
		proxyUrl{obj["proxy_url"]},
		height{obj["height"]},
		width{obj["width"]}
	{
	}
}