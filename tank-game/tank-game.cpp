// tank-game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "pch.h"
#include <iostream>
#include "input.h"
#include "output.h"
#include "joctanc.h"

input* intrare = input::getInstance();
output* iesire = output::getInstance(tanc::motor);

int main()
{
	intrare->init();

	tanc::motor->init(40, 20);

	*iesire << "TANK GAME\n";

	char nr;
	bool labirint = false;
	*iesire << "Cu labirint? D/N\n";
	while (1) {
		*intrare >> &nr;
		if (nr == 'D') labirint = true;
		else if (nr != 'N') {
			*iesire << "Tasta incorecta\n";
			continue;
		}
		break;
	}

	if (labirint) {
		*iesire << "Introduceti seed pentru labirint\n";
		*intrare >> &nr;
		tanc::motor->generarelabirint(static_cast<int>(nr));
	}

	while (1) {
		*iesire << "Introduceti numarul de tancuri (2-6)\n";
		*intrare >> &nr;
		try {
			if (nr < '2' || nr > '6') {
				throw "Numarul ales se afla in afara intervalului! \n";
			}
			break;
		}
		catch (const char* eroare)
		{
			*iesire << eroare;
		}
	}

	for (int i = '0'; i<nr; i++) {
		unit *u = tanc::motor->adaugareunitate(1);
		tanc *t = dynamic_cast<tanc *>(u);
		*iesire << "\nConfigurare tanc " << reinterpret_cast<char *>(&i);
		if (t != NULL) {
			while (1)
			{
				try {
					*intrare >> *t;
					break;
				}
				catch (const char *eroare) {
					*iesire << eroare;
				}
			}
		}
		else {
			i--;
		}
	}

	iesire->clear();
	*iesire << "Loading...";

	iesire->init();
	iesire->afisare();
	do {
		*intrare >> &tanc::cc;
		//tanc::cc = intrare->getc();
		tanc::motor->next();
		iesire->afisare();
	} while (tanc::cc != 27 && tanc::motor->gamerunning());
	iesire->clear();
	*iesire << "Jocul s-a terminat!\n";

	intrare->close();
	iesire->close();

}