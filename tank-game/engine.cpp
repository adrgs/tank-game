#include "pch.h"
#include "engine.h"
#include "joctanc.h"
#include <stack>
#include <vector>
#include <set>

#pragma region "Cod engine"
engine::engine()
{
	init(MAXNC, MAXNL);
}

engine::~engine()
{
}

void engine::init(int nc, int nl)
{
	this->nc = nc;
	this->nl = nl;
	this->nu = 0;
	for (int x = 0; x < nc; x++)
	{
		for (int y = 0; y < nl; y++)
		{
			unittab[x][y] = nullptr;
		}
	}
}

int engine::validxy(int x, int y)
{
	if (x>=0 && x<nc && y>=0 && y<nl)
		return 1;
	return 0;
}

int engine::validpoz(int poz)
{
	if (poz >= 0 && poz < nu) 
		return 1;
	return 0;
}

unit * engine::getunitxy(int x, int y)
{
	if (validxy(x,y)==0)
		return nullptr;
	return unittab[x][y];
}

unit * engine::getunitpoz(int poz)
{
	if (validpoz(poz)==0)
		return nullptr;
	return unitvec[poz];
}

void engine::next()
{
	for (int i = 0; i < nu; i++)
	{
		unit *u = unitvec[i];
		u->action();
	}
	for (int i = 0; i < nu; i++)
	{
		unit *u = unitvec[i];
		u->react();
	}
	for (int i = 0; i < nu; i++)
	{
		unit *u = unitvec[i];
		if (u->alive == 0) {
			unittab[u->getx()][u->gety()] = nullptr; // stergem unitatea de pe harta
			delete u; // stergem tancul
			for (int j = i; j < nu - 1; j++) {
				unitvec[j] = unitvec[j + 1]; // shiftam tancurile la stanga
			}
			--nu; // scadem numarul de tancuri din joc
			--i; // pentru a nu sari peste tancul urmator
		}
	}
}

unit* engine::adaugareunitate(int tip, int x, int y)
{
	int new_x=x;
	int new_y=y;
	if (x == -1337 || y == -1337) {
		while (1) {
			new_x = rand() % tanc::motor->getnc();
			new_y = rand() % tanc::motor->getnl();
			if (tanc::motor->getunitxy(new_x, new_y) == nullptr) {
				break;
			}
		}
	}
	else {
		if (validxy(x, y) == 0) return nullptr;
	}
	if (tip == 0) {
		return new zid(new_x, new_y);
	}
	else if (tip == 1) {
		return new tanc(0, 0, 0, 0, 0, 0, 0, new_x, new_y);
	}
	return nullptr;
}

void engine::generarelabirint(int seed)
{
	std::set<std::pair<int, int>> vizitat;
	for (int x = 0; x < this->nc; x++) {
		this->adaugareunitate(0, x, 0);
		this->adaugareunitate(0, x, this->nl - 1);
		vizitat.insert(std::pair<int, int>(x,0));
		vizitat.insert(std::pair<int,int>(x,this->nl - 1));
	}
	for (int y = 1; y < this->nl - 1; y++) {
		this->adaugareunitate(0, 0, y);
		this->adaugareunitate(0, this->nc - 1, y);
		vizitat.insert(std::pair<int, int>(0, y ));
		vizitat.insert(std::pair<int, int>(this->nc - 1, y));
	}
	srand(seed);

	std::stack<std::pair<int, int>> stiva;
	std::pair<int, int> tmp;
	tmp.first = 2 + rand()%(this->nc-2);
	tmp.second = 2 + rand() % (this->nl - 2);
	vizitat.insert(tmp);
	stiva.push(tmp);
	while (!stiva.empty())
	{
		tmp = stiva.top();

		stiva.pop();
		std::vector<std::pair<int, int>> vecini;

		if (vizitat.find(std::pair<int, int>(tmp.first + 1,tmp.second)) == vizitat.end()
			&& vizitat.find(std::pair<int, int>(tmp.first + 2, tmp.second)) == vizitat.end())
		{
			vecini.push_back(std::pair<int, int>(tmp.first + 2,tmp.second));
			vizitat.insert(std::pair<int, int>(tmp.first + 1,tmp.second));
			vizitat.insert(std::pair<int, int>(tmp.first + 2, tmp.second));
		}
		if (vizitat.find(std::pair<int, int>(tmp.first - 1,tmp.second)) == vizitat.end()
			&& vizitat.find(std::pair<int, int>(tmp.first - 2, tmp.second)) == vizitat.end())
		{
			vecini.push_back(std::pair<int, int>(tmp.first - 2,tmp.second ));
			vizitat.insert(std::pair<int, int>(tmp.first - 1, tmp.second));
			vizitat.insert(std::pair<int, int>(tmp.first - 2,tmp.second ));
		}
		if (vizitat.find(std::pair<int, int>(tmp.first,tmp.second +1)) == vizitat.end()
			&& vizitat.find(std::pair<int, int>(tmp.first, tmp.second + 2)) == vizitat.end())
		{
			vecini.push_back(std::pair<int, int>(tmp.first,tmp.second+2));
			vizitat.insert(std::pair<int, int>(tmp.first,tmp.second+2));
			vizitat.insert(std::pair<int, int>(tmp.first, tmp.second + 1));
		}
		if (vizitat.find(std::pair<int, int>(tmp.first,tmp.second -1)) == vizitat.end()
			&& vizitat.find(std::pair<int, int>(tmp.first, tmp.second - 2)) == vizitat.end())
		{
			vecini.push_back(std::pair<int, int>(tmp.first,tmp.second-2));
			vizitat.insert(std::pair<int, int>(tmp.first,tmp.second-2));
			vizitat.insert(std::pair<int, int>(tmp.first, tmp.second - 1));
		}

		if (!vecini.empty()) {
			int idx = rand() % vecini.size();
			for (size_t i = 0; i < vecini.size(); i++)
			{
				if (i != idx) {
					int dif_x = vecini[i].first - tmp.first;
					int dif_y = vecini[i].second - tmp.second;
					this->adaugareunitate(0, tmp.first+(dif_x/2), tmp.second + (dif_y / 2));
					stiva.push(vecini[i]);
				}
			}
			this->adaugareunitate(0, tmp.first, tmp.second);
			stiva.push(vecini[idx]);
		}
	}
}

bool engine::gamerunning()
{
	int numar_tancuri = 0;
	for (int i = 0; i < nu; i++)
	{
		tanc *u = dynamic_cast<tanc *>(getunitpoz(i));
		if (u != NULL) {
			numar_tancuri++;
		}
	}
	if (numar_tancuri < 2) return false;

	return true;
}

engine * engine::getInstance()
{
	if (instance == 0)
	{
		instance = new engine;
	}
	return instance;
}

engine* engine::instance = 0;
#pragma endregion

#pragma region "Cod unit"
void unit::react()
{
	for (int i = 0; i < nm; i++)
		behaviour(message[i]);
	nm = 0;
	delete[] message;
	message = nullptr;
}
unit::unit(int x, int y)
{
	this->nm = 0;
	this->alive = 1;
	this->x = x;
	this->y = y;
	motor->unittab[x][y] = this;
	message = nullptr;
	motor->unitvec[motor->nu] = this;
	motor->nu++;
}
unit::unit(const unit & u)
{
	this->alive = u.alive;
	this->x = u.x;
	this->y = u.y;
	this->poz = u.poz;
	this->message = new int[u.nm];
	memcpy(this->message, u.message, u.nm * sizeof(int));
	this->nm = u.nm;
}
unit::~unit()
{
	if (message!=nullptr)
		delete[] message;
}
void unit::die()
{
	this->alive = 0;
}
void unit::move(int dx, int dy)
{
	if (motor->unittab[x + dx][y + dy] != nullptr || motor->validxy(x+dx,y+dy) == 0) return;
	motor->unittab[x + dx][y + dy] = motor->unittab[x][y];
	motor->unittab[x][y] = nullptr;
	x += dx;
	y += dy;
}
void unit::sendmes(int pm, unit& pu)
{
	int *tmp = new int[pu.nm + 1];
	for (int i = 0; i < pu.nm; i++)
		tmp[i] = pu.message[i];
	tmp[pu.nm] = pm;
	delete[] pu.message;
	pu.message = tmp;
	++pu.nm;
}
engine* unit::motor = engine::getInstance();
#pragma endregion

input & operator>>(input & i, unit & u)
{
	return i;
}

output & operator<<(output & o, unit & u)
{
	o << "<class 'unit'>\n";
	return o;
}

input & operator>>(input & i, engine & e)
{
	return i;
}

output & operator<<(output & o, engine & e)
{
	o << "<class 'engine'>\n";
	return o;
}

std::istream & operator>>(std::istream & i, engine & e)
{
	return i;
}

std::ostream & operator<<(std::ostream & o, engine & e)
{
	o << "<class 'engine'>\n";
	return o;
}

std::istream & operator>>(std::istream & i, unit & u)
{
	return i;
}

std::ostream & operator<<(std::ostream & o, unit & u)
{
	o << "<class 'unit'>\n";
	return o;
}
