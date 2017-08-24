#pragma once
#include<ctime>

class timer
{
private:
	long begTime;
public:
	bool started;

	timer()
	{
		begTime = -0x7FFFF000;
	}

	auto start()
	{
		begTime = clock();
		started = true;
	}

	auto elapsed()
	{
		return (clock() - begTime) / (double)CLOCKS_PER_SEC;
	}	
};
