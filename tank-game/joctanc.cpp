#include "pch.h"
#include "joctanc.h"

unsigned char zid::nume = 177;
unsigned char bomba::nume = '.';
char tanc::cc = 0;

#pragma region "Cod bomba"
void bomba::action()
{
	int xn = getx() + dirx;
	int yn = gety() + diry;
	if (motor->validxy(xn, yn) == 0) {
		die();
	}
	else {
		unit *nextpoz = motor->getunitxy(xn, yn);
		if (nextpoz == nullptr) {
			move(dirx, diry);
		}
		else {
			sendmes(msg_KILL, *nextpoz);
			die();
		}
	}
}

void bomba::behaviour(int msg)
{
	if (msg == msg_KILL) {
		die();
	}
}
#pragma endregion

#pragma region "Cod tanc"
bool tanc::checktasta(char c)
{
	if (c == 23) return false;

	for (int i = 0; i < motor->getnu(); i++)
	{
		tanc *u = dynamic_cast<tanc *>(motor->getunitpoz(i));
		if (u != NULL)
		{
			if (c == u->nume || c == u->stg || c==u->jos || c==u->sus || c==u->dr || c==u->foc)
				return false;
		}
	}

	return true;
}
void tanc::action()
{
	int dx = 0;
	int dy = 0;
	int tmp;
	if (cc == foc) tmp = pc;
	else tmp = cc;

	dx = (dr == tmp) - (stg == tmp);
	dy = (jos == tmp) - (sus == tmp);

	if (cc != foc && (cc==stg || cc == dr || cc == jos || cc == sus)) this->pc = cc;
	
	if (motor->validxy(getx() + dx, gety() + dy)) {
		if (cc != foc) {
			if (motor->getunitxy(getx() + dx, gety() + dy)==nullptr)
				move(dx, dy);
		}
		else {
			int bx = getx() + dx;
			int by = gety() + dy;
			unit *u = motor->getunitxy(bx, by);
			if (u == nullptr) {
				new bomba(getx(), gety(), dx, dy);
			}
			else {
				sendmes(msg_KILL, *u);
			}
		}
	}
}

void tanc::behaviour(int msg)
{
	if (msg == msg_KILL) {
		die();
	}
}
#pragma endregion

input & operator>>(input & i, zid & t)
{
	return i;
}

output & operator<<(output & o, zid & t)
{
	o << "<class 'zid'>\n";
	return o;
}

input & operator>>(input & i, bomba & t)
{
	return i;
}

output & operator<<(output & o, bomba & t)
{
	o << "<class 'bomba'>\n";
	return o;
}

input& operator>>(input &i, tanc &t)
{
	char c;

	if (t.nume == 0) {
		std::cout << "\nNume tanc:";
		i >> &c;
		if (t.checktasta(c) == false) throw "Eroare, tasta este deja folosita!";
		t.nume = c;
	}

	if (t.stg == 0) {
		std::cout << "\nStanga tanc:";
		i >> &c;
		if (t.checktasta(c) == false) throw "Eroare, tasta este deja folosita!";
		t.stg = c;
	}

	if (t.dr == 0) {
		std::cout << "\nDreapta tanc:";
		i >> &c;
		if (t.checktasta(c) == false) throw "Eroare, tasta este deja folosita!";
		t.dr = c;
	}

	if (t.sus == 0) {
		std::cout << "\nSus tanc:";
		i >> &c;
		if (t.checktasta(c) == false) throw "Eroare, tasta este deja folosita!";
		t.sus = c;
	}

	if (t.jos == 0) {
		std::cout << "\nJos tanc:";
		i >> &c;
		if (t.checktasta(c) == false) throw "Eroare, tasta este deja folosita!";
		t.jos = c;
	}

	if (t.foc == 0) {
		std::cout << "\nFoc tanc:";
		i >> &c;
		if (t.checktasta(c) == false) throw "Eroare, tasta este deja folosita!";
		t.foc = c;
	}

	return i;
}


output &operator<<(output &o, tanc & t)
{
	o << "<class 'tanc'>\n";
	return o;
}

input &operator<<(input &i, tanc & t)
{
	return i;
}