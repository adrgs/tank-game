#include "pch.h"
#include "input.h"
#include <conio.h>
#include <Windows.h>

input* input::instance = 0;

input* input::getInstance()
{
	if (instance == 0)
	{
		instance = new input;
	}
	
	return instance;
}

void input::init()
{

}

void input::close()
{

}

char input::getc()
{
	char x = _getch();
	while (x == 0 || x == -32)
	{
		x = _getch();
	}
	return x;
}

input::input(const input&i) {

}


input::input()
{

}
input::~input()
{
	delete instance;
}

input& operator>>(input& i, char * c)
{
	*c = i.getc();
	return i;
}
