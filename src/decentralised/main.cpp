#include <stdio.h>
#include <string>
#include <iostream>
#include "dc-config.h"

int main()
{
	printf(std::string("Version ")
		.append(std::to_string(Decentralised_VERSION_MAJOR))
		.append(".")
		.append(std::to_string(Decentralised_VERSION_MINOR))
		.append("\n").c_str());

	printf("Gotta start sumwhere\n");

	std::string input = "";
	getline(std::cin, input);

	return 0;
}