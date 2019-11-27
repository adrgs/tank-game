#include "pch.h"
#include "output.h"
#include "joctanc.h"
#include <Windows.h>

static int pictvect[MAXNU][2], npict;
HANDLE hConsole;

void scrie_caracter_la_poz(char* buff, int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hConsole, coord);
	DWORD dwCount = 0;
	WriteConsoleA(hConsole, buff, 1, &dwCount, NULL);
}

output::output(engine *pm) : motor(pm){
};

output::output(const output&o) {
	this->motor = o.motor;
}

output::~output()
{
	delete instance; // stergem instanta
}

output* output::instance = 0;

output* output::getInstance()
{
	if (instance == 0) throw "Eroare, instanta nu exista!";
	return instance;
}

output* output::getInstance(engine *pm)
{
	if (instance == 0) {
		instance = new output(pm);
	}
	return instance;
}

void output::init()
{
	// ascundem cursorul din consola
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
	
	// stergem output-ul din consola
	system("cls");
	
	//desenam jocul initial
	for (int x = 0; x < motor->getnc(); x++) {
		for (int y = 0; y < motor->getnl(); y++) {
			unit *u = motor->getunitxy(x, y);
			if (u == nullptr) {
				scrie_caracter_la_poz(const_cast<char*>(" "), x, y);
			}
			else {
				scrie_caracter_la_poz(static_cast<char*>(u->gettip()), x, y);
			}
		}
	}
	npict = 0;
	for (int i = 0; i < motor->getnu(); i++) {
		unit *u = motor->getunitpoz(i);
		if (dynamic_cast<zid*>(u) == nullptr) { // rescriem elementele mobile cu coordonatele lor
			pictvect[npict][0] = u->getx();
			pictvect[npict][1] = u->gety();
			npict++;
		}
	}
}

void output::close()
{
}

void output::afisare()
{
	for (int i = 0; i < npict; i++) // stergem elementele mobile desenate anterior
	{
		scrie_caracter_la_poz(const_cast<char*>(" "), pictvect[i][0], pictvect[i][1]);
	}
	npict = 0;
	for (int i = 0; i < motor->getnu(); i++) {
		unit *u = motor->getunitpoz(i);
		if (dynamic_cast<zid*>(u) == nullptr) { // rescriem elementele mobile cu coordonatele lor
			pictvect[npict][0] = u->getx();
			pictvect[npict][1] = u->gety();
			npict++;
			scrie_caracter_la_poz(static_cast<char*>(u->gettip()), u->getx(), u->gety());
		}
	}
}

void output::clear()
{
	system("cls");
}

output & operator<<(output & o, const char * str)
{
	std::cout << str;
	return o;
}
