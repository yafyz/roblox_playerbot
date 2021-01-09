#pragma once

#include <string>
#include <cpr/cpr.h>
#include "out.h"

namespace accs {
	class account {
	public:
		std::string cookie;
		std::string xcsrf;
		std::string authTicket;
		bool isValid = true;

		account(std::string c) {
			cookie = c;
		};

		int getAuthTicket(std::string placeId) {
			char url[64];
			sprintf_s(url, "https://www.roblox.com/games/%s/", placeId.c_str()); // yeah yeah not needed but who knows, maybe roblox would use it in future ban wave

			cpr::Response r = cpr::Post(
				cpr::Url{ "https://auth.roblox.com/v1/authentication-ticket/" },
				cpr::Header{
					{"cookie", ".ROBLOSECURITY=" + cookie},
					{"x-csrf-token", xcsrf},
					{"Referer", url}
				},
				cpr::Body{}
			);

			if (r.header.count("x-csrf-token") > 0)
				xcsrf = r.header["x-csrf-token"];

			if (r.header.count("RBX-Authentication-Ticket") > 0)
				authTicket = r.header["RBX-Authentication-Ticket"];

			out::debug("Auth: %s %d\n", r.text.c_str(), r.status_code);
			return r.status_code;
		}

		int updateAuthTicket(std::string placeId) {
			int ret = getAuthTicket(placeId);
			if (ret == 403)
				ret = getAuthTicket(placeId);
			else if (ret == 401) {
				isValid = false;
			}
			return ret;
		}
	};
}