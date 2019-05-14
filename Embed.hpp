#ifndef DISCXXORD_EMBED_HPP
#define DISCXXORD_EMBED_HPP


#include "Date.hpp"
#include "Optional.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class EmbedFooter {
	public:
		std::string		text = "";
		Optional<std::string>	iconUrl;
		Optional<std::string>	proxyIconUrl;

		EmbedFooter(json obj);
		EmbedFooter(
			const std::string &text = "",
			const Optional <std::string> &iconUrl = {},
			const Optional <std::string> &proxyIconUrl = {}
		);
		std::string dump() const;
	};

	class EmbedImage {
	public:
		Optional<std::string>	url;
		Optional<std::string>	proxyUrl;
		Optional<int>		height;
		Optional<int>		width;

		EmbedImage(json obj);
		EmbedImage(
			const Optional<std::string> &url = {},
			const Optional<std::string> &proxyUrl = {},
			const Optional<int> &height = {},
			const Optional<int> &width = {}
		);
		std::string dump() const;
	};

	class EmbedVideo {
	public:
		Optional<std::string>	url;
		Optional<int>		height;
		Optional<int>		width;

		EmbedVideo(json obj);
		EmbedVideo(
			const Optional<std::string> &url = {},
			const Optional<int> &height = {},
			const Optional<int> &width = {}
		);
		std::string dump() const;
	};

	class EmbedProvider {
	public:
		Optional<std::string>	url;
		Optional<std::string>	name;

		EmbedProvider(json obj);
		EmbedProvider(
			const Optional<std::string> &url = {},
			const Optional<std::string> &name = {}
		);
		std::string dump() const;
	};

	class EmbedAuthor {
	public:
		Optional<std::string>	iconUrl;
		Optional<std::string>	proxyIconUrl;
		Optional<std::string>	url;
		Optional<std::string>	name;

		EmbedAuthor(json obj);
		EmbedAuthor(const Optional <std::string> &name = {},
			    const Optional <std::string> &iconUrl = {},
			    const Optional <std::string> &url = {},
			    const Optional <std::string> &proxyIconUrl = {});
		std::string dump() const;
	};

	class EmbedField {
	public:
		std::string	name = "";
		std::string	value = "";
		Optional<bool>	isInline;

		EmbedField() = default;
		EmbedField(json obj);
		EmbedField(const EmbedField &);
		EmbedField(
			const std::string &name = {},
			const std::string &value = {},
			const Optional<bool> &isInline = {}
		);
		std::string dump() const;
	};

	class Embed {
	public:
		Optional<std::string>			title;
		Optional<std::string>			type;
		Optional<std::string>			description;
		Optional<std::string>			url;
		Optional<Date>				timestamp;
		Optional<int>				color;
		Optional<EmbedFooter>			footer;
		Optional<EmbedImage>			image;
		Optional<EmbedImage>			thumbnail;
		Optional<EmbedVideo>			video;
		Optional<EmbedProvider>			provider;
		Optional<EmbedAuthor>			author;
		Optional<std::vector<EmbedField>>	fields;

		Embed(json obj);
		Embed(Embed &) = default;
		Embed(Embed &&) = default;
		Embed(
			const std::string &title = {},
			const std::string &description = {},
			const std::string &url = {},
			const Optional<Date> &timestamp = {},
			const Optional<int> &color = {},
			const Optional<EmbedFooter> &footer = {},
			const Optional<EmbedImage> &image = {},
			const Optional<EmbedImage> &thumbnail = {},
			const Optional<EmbedVideo> &video = {},
			const Optional<EmbedProvider> &provider = {},
			const Optional<EmbedAuthor> &author = {},
			const Optional<std::vector<EmbedField>> &fields = {},
			const Optional<std::string> &type = {}
		);
		std::string dump() const;
	};
}


#endif //DISCXXORD_EMBED_HPP
