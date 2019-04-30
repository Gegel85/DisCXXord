/*
** EPITECH PROJECT, 2019
** DisCXXord
** File description:
** Embed.hpp
*/

#ifndef DISCXXORD_EMBED_HPP
#define DISCXXORD_EMBED_HPP


#include "Date.hpp"
#include "Optional.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	struct EmbedFooter {
		std::string		text = "";
		Optional<std::string>	iconUrl;
		Optional<std::string>	proxyIconUrl;

		EmbedFooter() = default;
		EmbedFooter(json obj);

		std::string dump();
	};

	struct EmbedImage {
		Optional<std::string>	url;
		Optional<std::string>	proxyUrl;
		Optional<int>		height;
		Optional<int>		width;

		EmbedImage() = default;
		EmbedImage(json obj);

		std::string dump();
	};

	struct EmbedVideo {
		Optional<std::string>	url;
		Optional<int>		height;
		Optional<int>		width;

		EmbedVideo() = default;
		EmbedVideo(json obj);

		std::string dump();
	};

	struct EmbedProvider {
		Optional<std::string>	url;
		Optional<std::string>	name;

		EmbedProvider() = default;
		EmbedProvider(json obj);

		std::string dump();
	};

	struct EmbedAuthor {
		Optional<std::string>	iconUrl;
		Optional<std::string>	proxyIconUrl;
		Optional<std::string>	url;
		Optional<std::string>	name;

		EmbedAuthor() = default;
		EmbedAuthor(json obj);

		std::string dump();
	};

	struct EmbedField {
		std::string	name = "";
		std::string	value = "";
		Optional<bool>	isInline;

		EmbedField() = default;
		EmbedField(json obj);

		std::string dump();
	};

	struct Embed {
		Optional<std::string>	title;
		Optional<std::string>	type;
		Optional<std::string>	description;
		Optional<std::string>	url;
		Optional<Date>		timestamp;
		Optional<int>		color;
		EmbedFooter		footer;
		EmbedImage		image;
		EmbedImage		thumbnail;
		EmbedVideo		video;
		EmbedProvider		provider;
		EmbedAuthor		author;
		std::vector<EmbedField>	fields;

		Embed() = default;
		Embed(json obj);

		std::string dump();
	};
}


#endif //DISCXXORD_EMBED_HPP