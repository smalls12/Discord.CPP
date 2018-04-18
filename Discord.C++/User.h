#pragma once
//#include "snowflake.h"
#include <iostream>
#include <cpprest\json.h>

#include "Logger.h"

namespace DiscordCPP {

	using namespace std;
	using namespace web::json;
	using namespace utility;

	class User {
	//protected:
	//	Logger _log;
	public:
		string id;	//Snowflake
		string username;
		string discriminator;
		string avatar;
		bool bot = false;
		bool mfa_enabled = false;
		bool verified = false;
		string email;

		__declspec(dllexport) User();
		__declspec(dllexport) User(value data);
		__declspec(dllexport) User(string id, string_t token);
		__declspec(dllexport) ~User();
	};

}