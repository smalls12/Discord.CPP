#pragma once
#define VERSION		"0.7.1"

#define GATEWAY_URL	"wss://gateway.discord.gg?v=7&encoding=json"
#define API_URL		"https://discordapp.com/api/v7"

#define is_valid_field(name)	(data.has_field(U(name))) && (!data.at(U(name)).is_null())

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include <chrono>
#include <cpprest/asyncrt_utils.h>

DLL_EXPORT pplx::task<void> waitFor(const std::chrono::milliseconds ms);

DLL_EXPORT void hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2);
DLL_EXPORT std::string urlencode(std::string s);
