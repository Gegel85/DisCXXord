#include "Embed.hpp"

namespace DisCXXord
{
	EmbedAuthor::EmbedAuthor(json obj) :
		iconUrl{obj["icon_url"]},
		proxyIconUrl{obj["proxy_icon_url"]},
		url{obj["url"]},
		name{obj["name"]}
	{
	}

	EmbedField::EmbedField(json obj) :
		name(obj["name"]),
		value(obj["value"]),
		isInline{obj["inline"]}
	{
	}

	EmbedFooter::EmbedFooter(json obj) :
		text(obj["text"]),
		iconUrl{obj["icon_url"]},
		proxyIconUrl{obj["proxy_icon_url"]}
	{
	}

	EmbedImage::EmbedImage(json obj) :
		url{obj["url"]},
		proxyUrl{obj["proxy_url"]},
		height{obj["height"]},
		width{obj["width"]}
	{
	}

	EmbedProvider::EmbedProvider(json obj) :
		url{obj["url"]},
		name{obj["name"]}
	{
	}

	EmbedVideo::EmbedVideo(json obj) :
		url{obj["url"]},
		height{obj["height"]},
		width{obj["width"]}
	{
	}

	Embed::Embed(json obj) :
		title{obj["title"]},
		type{obj["type"]},
		description{obj["description"]},
		url{obj["url"]},
		color{obj["color"]},
		footer{obj["footer"]},
		image{obj["image"]},
		thumbnail{obj["thumbnail"]},
		video{obj["video"]},
		provider{obj["provider"]},
		author{obj["author"]}
	{
		if (!obj["timestamp"].is_null())
			this->timestamp = Date(static_cast<time_t>(obj["timestamp"]));
		for (auto &val : obj["fields"])
			fields.emplace_back(val);
	}
}