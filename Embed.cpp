#include <sstream>
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

	std::string EmbedAuthor::dump()
	{
		bool first = true;
		std::stringstream stream;
		std::map<std::string, std::string> arr;

		if (this->url)
			arr["url"] = "\"" + *this->url + "\"";

		if (this->name)
			arr["name"] = "\"" + *this->name + "\"";

		if (this->iconUrl)
			arr["icon_url"] = "\"" + *this->iconUrl + "\"";

		if (this->proxyIconUrl)
			arr["proxy_icon_url"] = "\"" + *this->proxyIconUrl + "\"";

		if (arr.empty())
			return "null";

		for (auto &val : arr) {
			if (!first)
				stream << ",";
			first = false;
			stream << "\"" << val.first << "\":\"" << val.second << "\"";
		}

		return "{" + stream.str() + "}";
	}

	EmbedField::EmbedField(json obj) :
		name(obj["name"]),
		value(obj["value"]),
		isInline{obj["inline"]}
	{
	}

	std::string EmbedField::dump()
	{
		return R"({"value":")" + this->value + R"(","name":")" + this->name + R"(","inline":)" + (this->isInline || *this->isInline ? "true" : "false") + "}";
	}

	EmbedFooter::EmbedFooter(json obj) :
		text(obj["text"]),
		iconUrl{obj["icon_url"]},
		proxyIconUrl{obj["proxy_icon_url"]}
	{
	}

	std::string EmbedFooter::dump()
	{
		std::stringstream stream;
		std::map<std::string, std::string> arr;

		stream << R"("text":")" << this->text << '"';

		if (this->iconUrl)
			arr["icon_url"] = "\"" + *this->iconUrl + "\"";

		if (this->proxyIconUrl)
			arr["proxy_icon_url"] = "\"" + *this->proxyIconUrl + "\"";

		if (arr.empty())
			return "null";

		for (auto &val : arr) {
			stream << ",";
			stream << '"' << val.first << R"(":")" << val.second << '"';
		}

		return "{" + stream.str() + "}";
	}

	EmbedImage::EmbedImage(json obj) :
		url{obj["url"]},
		proxyUrl{obj["proxy_url"]},
		height{obj["height"]},
		width{obj["width"]}
	{
	}

	std::string EmbedImage::dump()
	{
		bool first = true;
		std::stringstream stream;
		std::map<std::string, std::string> arr;

		if (this->url)
			arr["url"] = "\"" + *this->url + "\"";

		if (this->proxyUrl)
			arr["proxy_url"] = "\"" + *this->proxyUrl + "\"";

		if (this->height)
			arr["height"] = std::to_string(*this->height);

		if (this->width)
			arr["width"] = std::to_string(*this->width);

		if (arr.empty())
			return "null";

		for (auto &val : arr) {
			if (!first)
				stream << ",";
			first = false;
			stream << "\"" << val.first << "\":\"" << val.second << "\"";
		}

		return "{" + stream.str() + "}";
	}

	EmbedProvider::EmbedProvider(json obj) :
		url{obj["url"]},
		name{obj["name"]}
	{
	}

	std::string EmbedProvider::dump()
	{
		bool first = true;
		std::stringstream stream;
		std::map<std::string, std::string> arr;

		if (this->url)
			arr["url"] = "\"" + *this->url + "\"";

		if (this->name)
			arr["name"] = "\"" + *this->name + "\"";

		if (arr.empty())
			return "null";

		for (auto &val : arr) {
			if (!first)
				stream << ",";
			first = false;
			stream << "\"" << val.first << "\":\"" << val.second << "\"";
		}

		return "{" + stream.str() + "}";
	}

	EmbedVideo::EmbedVideo(json obj) :
		url{obj["url"]},
		height{obj["height"]},
		width{obj["width"]}
	{
	}

	std::string EmbedVideo::dump()
	{
		bool first = true;
		std::stringstream stream;
		std::map<std::string, std::string> arr;

		if (this->url)
			arr["url"] = "\"" + *this->url + "\"";

		if (this->height)
			arr["height"] = std::to_string(*this->height);

		if (this->width)
			arr["width"] = std::to_string(*this->width);

		if (arr.empty())
			return "null";

		for (auto &val : arr) {
			if (!first)
				stream << ",";
			first = false;
			stream << "\"" << val.first << "\":\"" << val.second << "\"";
		}

		return "{" + stream.str() + "}";
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

	std::string Embed::dump()
	{
		bool first = true;
		std::stringstream stream;
		std::map<std::string, std::string> arr;

		if (this->title)
			arr["title"] = "\"" + *this->title + "\"";

		if (this->type)
			arr["type"] = "\"" + *this->type + "\"";

		if (this->description)
			arr["description"] = "\"" + *this->description + "\"";

		if (this->url)
			arr["url"] = "\"" + *this->url + "\"";

		if (this->color)
			arr["color"] = std::to_string(*this->color);

		if (this->timestamp)
			arr["timestamp"] = "\"" + this->timestamp->toISO() + "\"";

		arr["image"] = this->image.dump();
		arr["video"] = this->video.dump();
		arr["footer"] = this->footer.dump();
		arr["author"] = this->author.dump();
		arr["provider"] = this->provider.dump();
		arr["thumbnail"] = this->thumbnail.dump();
		arr["fields"] = "[";
		for (auto &val : this->fields) {
			if (!first)
				arr["fields"] += ",";
			first = false;
			arr["fields"] += val.dump();
		}
		arr["fields"] += "]";

		if (arr.empty())
			return "null";

		first = true;
		for (auto &val : arr) {
			if (val.second != "null") {
				if (!first)
					stream << ",";
				first = false;
				stream << "\"" << val.first << "\":\"" << val.second << "\"";
			}
		}

		return "{" + stream.str() + "}";
	}
}