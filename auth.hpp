#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>

std::string tm_to_readable_time(const std::tm& ctx) {
	char buffer[80];
	strftime(buffer, sizeof(buffer), skCrypt("%a %m/%d/%y %H:%M:%S "), &ctx);
	return std::string(buffer);
}

std::time_t string_to_timet(const std::string& timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10);
	return static_cast<time_t>(cv);
}

std::tm timet_to_tm(time_t timestamp) {
	std::tm context;
	localtime_s(&context, &timestamp);
	return context;
}
std::string format_duration(time_t time_diff) {
    const int seconds_in_minute = 60;
    const int seconds_in_hour = 60 * seconds_in_minute;
    const int seconds_in_day = 24 * seconds_in_hour;
    const int seconds_in_week = 7 * seconds_in_day;
    const int seconds_in_month = 30 * seconds_in_day;
    const int seconds_in_year = 365 * seconds_in_day;

    int years = time_diff / seconds_in_year;

    if (years >= 1) {
        return (std::string)skCrypt("Lifetime");
    }

    time_diff %= seconds_in_year;
    int months = time_diff / seconds_in_month;
    time_diff %= seconds_in_month;

    int weeks = time_diff / seconds_in_week;
    time_diff %= seconds_in_week;

    int days = time_diff / seconds_in_day;
    time_diff %= seconds_in_day;

    int hours = time_diff / seconds_in_hour;
    time_diff %= seconds_in_hour;

    int minutes = time_diff / seconds_in_minute;

    int seconds = time_diff % seconds_in_minute;

    if (years == 0 && months == 0 && weeks == 0 && days == 0 && hours == 0 && minutes == 0 && seconds == 0) {
        exit(0);
    }

    std::string result;
    if (months > 0) result += std::to_string(months) + (std::string)skCrypt(" month") + (months > 1 ? (std::string)skCrypt("s ") : (std::string)skCrypt(" "));
    if (weeks > 0) result += std::to_string(weeks) + (std::string)skCrypt(" week") + (weeks > 1 ? (std::string)skCrypt("s ") : (std::string)skCrypt(" "));
    if (days > 0) result += std::to_string(days) + (std::string)skCrypt(" day") + (days > 1 ? (std::string)skCrypt("s ") : (std::string)skCrypt(" "));
    if (hours > 0) result += std::to_string(hours) + (std::string)skCrypt(" hour") + (hours > 1 ? (std::string)skCrypt("s ") : (std::string)skCrypt(" "));
    if (minutes > 0) result += std::to_string(minutes) + (std::string)skCrypt(" minute") + (minutes > 1 ? (std::string)skCrypt("s ") : (std::string)skCrypt(" "));
    if (seconds > 0) result += std::to_string(seconds) + (std::string)skCrypt(" second") + (seconds > 1 ? (std::string)skCrypt("s ") : (std::string)skCrypt(" "));

    return result.empty() ? (std::string)skCrypt("0 seconds") : result;
}
namespace KeyAuth {
	class api {
	public:

		std::string name, ownerid, secret, version, url;

		api(std::string name, std::string ownerid, std::string secret, std::string version, std::string url) : name(name), ownerid(ownerid), secret(secret), version(version), url(url) {}

		void ban();
		void init();
		void check();
		void log(std::string msg);
		void license(std::string key);
		std::string var(std::string varid);
		std::string webhook(std::string id, std::string params);
		void setvar(std::string var, std::string vardata);
		std::string getvar(std::string var);
		bool checkblack();
		void upgrade(std::string username, std::string key);
		void login(std::string username, std::string password);
		void web_login();
		void button(std::string value);
		std::vector<unsigned char> download(std::string fileid);
		void regstr(std::string username, std::string password, std::string key);

		class data_class {
		public:
			// app data
			std::string numUsers;
			std::string numOnlineUsers;
			std::string numKeys;
			std::string version;
			std::string customerPanelLink;
			// user data
			std::string username;
			std::string ip;
			std::string hwid;
			std::string createdate;
			std::string lastlogin;
			std::vector<std::string> subscriptions;
			std::string expiry;
			// response data
			bool success;
			std::string message;
		};
		data_class data;

	private:
		std::string sessionid, enckey;
	};
}
