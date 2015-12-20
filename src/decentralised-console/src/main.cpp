#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "dc-config.h"
#include <decentralised_core.hpp>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

template <typename T>
std::wstring to_wstring(T val)
{
	std::wstringstream stream;
	stream << val;
	return stream.str();
}

void event_received(int code, std::string msg)
{
	printf(msg.append("\n").c_str());
};

int main()
{
	std::wstring title = std::wstring(APP_TITLE)
		.append(L" ")
		.append(to_wstring(Decentralised_VERSION_MAJOR))
		.append(L".")
		.append(to_wstring(Decentralised_VERSION_MINOR))
		.append(L"\n").c_str();

#ifdef WIN32
	SetConsoleTitle(std::string(title.begin(), title.end()).c_str());
#endif

	printf(std::string(title.begin(), title.end()).c_str());

	decentralised::core::decentralised_client* client = new decentralised::core::decentralised_client(&event_received);
	client->start("data\\blockchain_data");

	std::string input;
	while (getline(std::cin, input))
	{
		if (input == "quit" || input == "exit")
			break;
		else if (input == "help" || input == "?")
		{
			printf("--------------------------------------------\n");
			printf("help\t- displays this information\n");
			printf("quit\t- shuts down decentralised and exits\n");
			printf("--------------------------------------------\n");
		}
	}

	delete client;

	return 0;
}