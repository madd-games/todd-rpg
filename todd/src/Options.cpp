/**
 * Options.cpp
 * Player's options.
 */

#include "Options.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>

using namespace std;

Options options;

Options::Options() :
	confirmKey(SDLK_x),
	cancelKey(SDLK_z),
	upKey(SDLK_UP),
	downKey(SDLK_DOWN),
	leftKey(SDLK_LEFT),
	rightKey(SDLK_RIGHT),
	menuKey(SDLK_ESCAPE)
{
	ifstream ifs(getPathToOptions().c_str(), ios::binary);
	if (!ifs.fail())
	{
		ifs.read((char*) this, sizeof(Options));
		ifs.close();
	};
};

string Options::getPathToOptions()
{
	stringstream ss;
#ifdef _WIN32
	ss << getenv("APPDATA") << "\\madd.todd-options";
#else
	ss << getenv("HOME") << "/madd.todd-options";
#endif
	return ss.str();
};

void Options::save()
{
	ofstream ofs(getPathToOptions().c_str(), ios::binary);
	if (!ofs.fail())
	{
		ofs.write((const char*) this, sizeof(Options));
		ofs.close();
	};
};
