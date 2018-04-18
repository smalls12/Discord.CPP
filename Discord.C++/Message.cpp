#include "Message.h"
#include "static.h"
#include "VoiceChannel.h"
#include "GuildChannel.h"

#include <cpprest\http_client.h>

using namespace web::http;
using namespace web::http::client;
using namespace std;
using namespace web::json;
using namespace utility;

DiscordCPP::Message::Message(value data, string_t token) {
	//_log = Logger("discord.message");

	if(is_valid_field("id"))
		id = conversions::to_utf8string(data.at(U("id")).as_string());
	
	if (is_valid_field("channel_id")) {
		string url = "/channels/" + conversions::to_utf8string(data.at(U("channel_id")).as_string());

		http_client c(U(API_URL));
		http_request request(methods::GET);

		request.set_request_uri(uri(conversions::to_string_t(url)));
		request.headers().add(U("Authorization"), conversions::to_string_t("Bot " + conversions::to_utf8string(token)));

		Concurrency::task<Channel *> requestTask = c.request(request).then([this, token](http_response response) {
			string response_string = response.extract_utf8string().get();

			//_log.debug(response_string);

			value data = value::parse(conversions::to_string_t(response_string));

			//_log.debug(conversions::to_utf8string(data.serialize()));

			if (is_valid_field("bitrate")) {
				return (Channel *)new VoiceChannel(data, token);
			}
			else if (is_valid_field("guild_id")) {
				return (Channel *)new GuildChannel(data, token);
			}
			else {
				return new Channel(data, token);
			}
		});

		try {
			requestTask.wait();
			channel = requestTask.get();
		}
		catch (const std::exception &e) {
			Logger("discord.message").error("exception in Message constructor: " + string(e.what()));
		}
	}

	if (is_valid_field("author"))
		author = new User(data.at(U("author")));

	if (is_valid_field("content"))
		content = conversions::to_utf8string(data.at(U("content")).as_string());

	if (is_valid_field("timestamp"))
		timestamp = conversions::to_utf8string(data.at(U("timestamp")).as_string());

	if (is_valid_field("edited_timestamp"))
		edited_timestamp = conversions::to_utf8string(data.at(U("edited_timestamp")).as_string());

	if (is_valid_field("tts"))
		tts = data.at(U("tts")).as_bool();

	if (is_valid_field("mention_everyone"))
		tts = data.at(U("mention_everyone")).as_bool();

	if (is_valid_field("mentions")) {
		web::json::array tmp = data.at(U("mentions")).as_array();
		for (int i = 0; i < tmp.size(); i++)
			mentions.push_back(new User(tmp[i]));
	}

	//mention_roles

	//attachements

	//embeds

	//reactions

	if (is_valid_field("pinned"))
		pinned = data.at(U("pinned")).as_bool();

	if (is_valid_field("webhook_id"))
		webhook_id = conversions::to_utf8string(data.at(U("webhook_id")).as_string());

	if (is_valid_field("type"))
		type = data.at(U("type")).as_integer();

	//activity

	//application

	//_log.debug("created message object");
}

DiscordCPP::Message::Message(const Message & old) {
	//_log = old._log;
	id = old.id;
	
	if (old.channel != NULL) {
		try {
			channel = old.channel->copy(*old.channel);
		}
		catch (std::exception &e) {
			Logger("discord.message").error("Error in channel copy: " + string(e.what()));
		}
	}
	else {
		channel = NULL;
	}

	author = new User(*old.author);
	content = old.content;
	timestamp = old.timestamp;
	edited_timestamp = old.edited_timestamp;
	tts = old.tts;
	mention_everyone = old.mention_everyone;
	//mentions = old.mentions;
	for (int i = 0; i < old.mentions.size(); i++) {
		mentions.push_back(new User(*old.mentions[i]));
	}
	//mention_roles
	//attachements
	//embeds
	//reactions
	pinned = old.pinned;
	webhook_id = old.webhook_id;
	type = old.type;
	//activity
	//application
}

DiscordCPP::Message::Message() {
	//_log = Logger("discord.message");
	//_log.debug("created empty message object");
}

DiscordCPP::Message::~Message() {
	//_log.debug("destroyed message object");

	//if(channel != NULL)
	delete channel;
	delete author;
	for (int i = 0; i < mentions.size(); i++) {
		delete mentions[i];
	}
}