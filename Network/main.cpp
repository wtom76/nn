// Network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network.h"

//---------------------------------------------------------------------------------------------------------
shared_ptr<spdlog::logger> createLog()
{
	shared_ptr<spdlog::logger> log_ptr;

	if (!spdlog::get("log"))
	{
		log_ptr = spdlog::stdout_color_mt("log");
		log_ptr->info("Log is created");
	}
	return log_ptr;
}

int main()
{
	shared_ptr<spdlog::logger> log_ptr = createLog();
	Network network(log_ptr);
	network.init();
	network.learn();
    return 0;
}

