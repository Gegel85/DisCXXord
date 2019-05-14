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

		EmbedFooter() = default;
		EmbedFooter(json obj);
		EmbedFooter(
			std::string		&&text = "",
			Optional<std::string>	&&iconUrl = {},
			Optional<std::string>	&&proxyIconUrl = {}
		);
		std::string dump() const;
	};

	class EmbedImage {
	public:
		Optional<std::string>	url;
		Optional<std::string>	proxyUrl;
		Optional<int>		height;
		Optional<int>		width;

		EmbedImage() = default;
		EmbedImage(json obj);
		EmbedImage(
			Optional<std::string>	&&url = {},
			Optional<std::string>	&&proxyUrl = {},
			Optional<int>		&&height = {},
			Optional<int>		&&width = {}
		);
		std::string dump() const;
	};

	class EmbedVideo {
	public:
		Optional<std::string>	url;
		Optional<int>		height;
		Optional<int>		width;

		EmbedVideo() = default;
		EmbedVideo(json obj);
		EmbedVideo(
			Optional<std::string>	&&url = {},
			Optional<int>		&&height = {},
			Optional<int>		&&width = {}
		);
		std::string dump() const;
	};

	class EmbedProvider {
	public:
		Optional<std::string>	url;
		Optional<std::string>	name;

		EmbedProvider() = default;
		EmbedProvider(json obj);
		EmbedProvider(
			Optional<std::string>	&&url = {},
			Optional<std::string>	&&name = {}
		);
		std::string dump() const;
	};

	class EmbedAuthor {
	public:
		Optional<std::string>	iconUrl;
		Optional<std::string>	proxyIconUrl;
		Optional<std::string>	url;
		Optional<std::string>	name;

		EmbedAuthor() = default;
		EmbedAuthor(json obj);
		EmbedAuthor(
			Optional<std::string>	&&iconUrl = {},
			Optional<std::string>	&&proxyIconUrl = {},
			Optional<std::string>	&&url = {},
			Optional<std::string>	&&name = {}
		);
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
			std::string	&&name = {},
			std::string	&&value = {},
			Optional<bool>	&&isInline = {}
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
			std::string				&&title = {},
			std::string				&&description = {},
			std::string				&&url = {},
			Optional<Date>				&&timestamp = {},
			Optional<int>				&&color = {},
			Optional<EmbedFooter>			&&footer = {},
			Optional<EmbedImage>			&&image = {},
			Optional<EmbedImage>			&&thumbnail = {},
			Optional<EmbedVideo>			&&video = {},
			Optional<EmbedProvider>			&&provider = {},
			Optional<EmbedAuthor>			&&author = {},
			Optional<std::vector<EmbedField>>	&&fields = {},
			Optional<std::string>			&&type = {}
		);
		std::string dump() const;
	};
}


#endif //DISCXXORD_EMBED_HPP
