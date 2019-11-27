#pragma once
#include "engine.h"
#include <iostream>
class output
{
private:
	engine *motor;

	static output* instance;
	output(engine *pm);
	output(const output&o);
	~output();
public:
	static output* getInstance();
	static output* getInstance(engine *pm);
	void init();
	void close();
	void afisare();
	void clear();

	output& operator=(const output&o)
	{
		if (this != &o)
		{
			this->motor = o.motor;
			this->instance = o.instance;
		}
		return *this;
	}

	std::istream& operator>>(std::istream& in) {
		return in;
	}
	friend output& operator<<(output&o, const char *str);
};

