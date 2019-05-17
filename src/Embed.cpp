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

	EmbedAuthor::EmbedAuthor(const Optional <std::string> &name,
				 const Optional <std::string> &iconUrl,
				 const Optional <std::string> &url,
				 const Optional <std::string> &proxyIconUrl) :
		iconUrl(iconUrl),
		proxyIconUrl(proxyIconUrl),
		url(url),
		name(name)
	{
	}

	std::string EmbedAuthor::dump() const
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
			stream << "\"" << val.first << "\":" << val.second;
		}

		return "{" + stream.str() + "}";
	}



	EmbedField::EmbedField(json obj) :
		name(obj["name"]),
		value(obj["value"]),
		isInline{obj["inline"]}
	{
	}

	EmbedField::EmbedField(
		const std::string &name,
		const std::string &value,
		const Optional<bool> &isInline
	) :
		name(name),
		value(value),
		isInline(isInline)
	{
	}

	EmbedField::EmbedField(const DisCXXord::EmbedField &other) :
		name{other.name},
		value{other.value},
		isInline{other.isInline && *other.isInline}
	{
	}

	std::string EmbedField::dump() const
	{
		return R"({"value":")" + this->value + R"(","name":")" + this->name + R"(","inline":)" + (this->isInline || *this->isInline ? "true" : "false") + "}";
	}



	EmbedFooter::EmbedFooter(json obj) :
		text(obj["text"]),
		iconUrl{obj["icon_url"]},
		proxyIconUrl{obj["proxy_icon_url"]}
	{
	}

	EmbedFooter::EmbedFooter(
		const std::string &text,
		const Optional <std::string> &iconUrl,
		const Optional <std::string> &proxyIconUrl
	) :
		text(text),
		iconUrl(iconUrl),
		proxyIconUrl(proxyIconUrl)
	{
	}

	std::string EmbedFooter::dump() const
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
			stream << '"' << val.first << R"(":)" << val.second;
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

	EmbedImage::EmbedImage(
		const Optional <std::string> &url,
		const Optional <std::string> &proxyUrl,
		const Optional<int> &height,
		const Optional<int> &width
	) :
		url(url),
		proxyUrl(proxyUrl),
		height(height),
		width(width)
	{
	}

	std::string EmbedImage::dump() const
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
			stream << "\"" << val.first << "\":" << val.second;
		}

		return "{" + stream.str() + "}";
	}



	EmbedProvider::EmbedProvider(json obj) :
		url{obj["url"]},
		name{obj["name"]}
	{
	}

	EmbedProvider::EmbedProvider(
		const Optional <std::string> &url,
		const Optional <std::string> &name
	) :
		url(url),
		name(name)
	{
	}

	std::string EmbedProvider::dump() const
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
			stream << "\"" << val.first << "\":" << val.second;
		}

		return "{" + stream.str() + "}";
	}



	EmbedVideo::EmbedVideo(json obj) :
		url{obj["url"]},
		height{obj["height"]},
		width{obj["width"]}
	{
	}

	EmbedVideo::EmbedVideo(
		const Optional <std::string> &url,
		const Optional<int> &height,
		const Optional<int> &width
	) :
		url(url),
		height(height),
		width(width)
	{
	}

	std::string EmbedVideo::dump() const
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
			stream << "\"" << val.first << "\":" << val.second;
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
		fields = std::vector<EmbedField>();
		for (auto &val : obj["fields"])
			fields->emplace_back(val);
	}

	Embed::Embed(
		const std::string &title,
		const std::string &description,
		const std::string &url,
		const Optional <Date> &timestamp,
		const Optional<int> &color,
		const Optional <EmbedFooter> &footer,
		const Optional <EmbedImage> &image,
		const Optional <EmbedImage> &thumbnail,
		const Optional <EmbedVideo> &video,
		const Optional <EmbedProvider> &provider,
		const Optional <EmbedAuthor> &author,
		const Optional <std::vector<EmbedField>> &fields,
		const Optional <std::string> &type
	) :
		title(title.empty() ? json() : json(title)),
		type(type),
		description(description.empty() ? json() : json(description)),
		url(url.empty() ? json() : json(url)),
		timestamp(timestamp),
		color(color),
		footer(footer),
		image(image),
		thumbnail(thumbnail),
		video(video),
		provider(provider),
		author(author),
		fields(fields)
	{
	}

	std::string Embed::dump() const
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

		if (this->footer)
			arr["footer"] = this->footer->dump();

		if (this->fields) {
			arr["fields"] = "[";
			for (auto &val : *this->fields) {
				if (first)
					arr["fields"] += ",";
				first = false;
				arr["fields"] += val.dump();
			}
			arr["fields"] += "]";
		}

		if (this->image)
			arr["image"] = this->image->dump();

		if (this->video)
			arr["video"] = this->video->dump();

		if (this->author)
			arr["author"] = this->author->dump();

		if (this->provider)
			arr["provider"] = this->provider->dump();

		if (this->thumbnail)
			arr["thumbnail"] = this->thumbnail->dump();

		first = true;
		for (auto &val : arr) {
			if (!first)
				stream << ",";
			first = false;
			stream << "\"" << val.first << "\":" << val.second;
		}

		return "{" + stream.str() + "}";
	}
}