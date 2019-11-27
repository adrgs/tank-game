#pragma once
#include "engine.h"
#include "input.h"
#include "output.h"
class zid : public unit {
private:
	static unsigned char nume;
protected:
	void action() override {}
	void behaviour(int msg) override {}
public:
	zid(int x, int y):unit(x, y) {}
	zid(const zid& z) :unit(z) {
		*this = z;
	}
	~zid() {}
	void* gettip() override { return &nume; }

	zid& operator=(const zid&z)
	{
		return *this;
	}

	friend input& operator>>(input& i, zid&t);
	friend output& operator<<(output& o, zid&t);
};

class bomba : public unit {
private:
	static unsigned char nume;
	int dirx;
	int diry;
protected:
	void action();
	void behaviour(int msg);
public:
	bomba(int x, int y, int dirx, int diry):unit(x, y), dirx(dirx), diry(diry) { }
	bomba(const bomba& b) : unit(b) {
		*this = b;
	}
	~bomba() {}
	void *gettip() override { return &nume; }

	bomba& operator=(const bomba&b)
	{
		if (this != &b)
		{
			this->dirx = b.dirx;
			this->diry = b.diry;
		}
		return *this;
	}

	friend input& operator>>(input& i, bomba&t);
	friend output& operator<<(output& o, bomba&t);
};

class tanc : public unit {
private:
	unsigned char nume;
	char stg;
	char dr;
	char sus;
	char jos;
	char foc;
	char pc;
	bool checktasta(char c);
protected:
	void action();
	void behaviour(int msg);
public:
	static char cc; // asa scrie in tema
	tanc(char nume, char stg, char dr, char sus, char jos, char foc, char pc, int x, int y) :unit(x, y), nume(nume), stg(stg), dr(dr), sus(sus), jos(jos), foc(foc), pc(pc) {}
	tanc(const tanc& t) : unit(t) {
		*this = t;
	}
	~tanc() {}
	void *gettip() { return static_cast<void *>(&nume); }

	tanc& operator=(const tanc&t)
	{
		if (this != &t)
		{
			this->nume = t.nume;
			this->stg = t.stg;
			this->dr = t.dr;
			this->sus = t.sus;
			this->jos = t.jos;
			this->foc = t.foc;
			this->pc = t.pc;
		}
		return *this;
	}

	friend input& operator>>(input& i, tanc&t);
	friend output& operator<<(output& o, tanc&t);
};