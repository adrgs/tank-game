#pragma once
#include <iostream>
class input
{
private:
	static input* instance;
	input();
	input(const input&i);
	~input();
public:
	static input* getInstance();
	void init();
	void close();
	char getc();

	input& operator=(const input&i)
	{
		if (this != &i)
		{
			this->instance = i.instance;
		}
		return *this;
	}

	friend input& operator>>(input&i, char *c);
	std::ostream& operator<<(std::ostream& out)
	{
		out << "<class 'input'>\n";
		return out;
	}
};

