#pragma once
#include <cstring>
#include <iostream>
#define MAXNC 200
#define MAXNL 200
#define MAXNU 4000

class unit
{
private:
	int alive;               // 0 - dead, 1 - alive
	int x;                   // x coordinate of unit
	int y;                   // y coordinate of unit
	int poz;                 // position of unit in the unitvec array
	int *message;            // list of messages from other units
	int nm;                  // number of messages
	void react();            // response of the unit after interacting with the messages from other units
protected:
	unit(int x, int y);
	unit(const unit& u);
	virtual ~unit();
	void die();
	void move(int dx, int dy);
	void sendmes(int pm, unit& pu);
	virtual void action() = 0;
	virtual void behaviour(int msg) = 0;
public:
	int getx() const { return x; }
	int gety() const { return y; }
	virtual void* gettip() = 0;

	unit& operator=(const unit&u)
	{
		if (this != &u)
		{
			this->alive = u.alive;
			this->x = u.x;
			this->y = u.y;
			this->poz = u.poz;
			this->message = new int[u.nm];
			memcpy(this->message, u.message, u.nm * sizeof(int));
			this->nm = u.nm;
		}
		return *this;
	}

	friend class engine;
	
	friend std::istream& operator>>(std::istream& i, engine&e);
	friend std::ostream& operator<<(std::ostream& o, engine&e);

	static engine *motor;
};

class engine
{
private:
	int nl;                  // number of lines
	int nc;                  // number of columns
	int nu;                  // number of units
	unit *unittab[MAXNC][MAXNL];           // game table
	unit *unitvec[MAXNU];           // list of units on the table

	engine();
	engine(const engine&e);
	~engine();
	static engine* instance;

public:
	void init(int nc, int nl);
	int getnl() const { return nl; }
	int getnc() const { return nc; }
	int getnu() const { return nu; }
	int validxy(int x, int y);
	int validpoz(int poz);
	unit* getunitxy(int x, int y);
	unit* getunitpoz(int poz);
	void next();

	unit* adaugareunitate(int tip, int x = -1337, int y = -1337);

	engine& operator=(const engine&e)
	{
		if (this != &e)
		{
			this->nc = e.nc;
			this->nl = e.nl;
			this->nu = e.nu;
			for (int x = 0; x < this->nc; x++) {
				for (int y = 0; y < this->nl; y++) {
					if (e.unittab[x][y] == nullptr) this->unittab[x][y] = nullptr;
					else {
						*this->unittab[x][y] = *e.unittab[x][y];
					}
				}
			}
			for (int i = 0; i < this->nu; i++) {
				*this->unitvec[i] = *e.unitvec[i];
			}
		}
		return *this;
	}

	static engine* getInstance();
	void generarelabirint(int seed);
	bool gamerunning();

	friend std::istream& operator>>(std::istream& i, unit&u);
	friend std::ostream& operator<<(std::ostream& o, unit&u);

	friend class unit;
};

enum {
	msg_KILL = 1337
};