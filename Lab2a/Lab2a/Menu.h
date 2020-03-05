#ifndef MENU_HEADER_CLASS_AND_COLOR
#define MENU_HEADER_CLASS_AND_COLOR

#include <Windows.h>
#include <initializer_list>
#include <conio.h>
#include <vector>
#include <string>

#include <iostream>

using std::cout;
using std::endl;

using std::vector;
using std::string;
using std::initializer_list;

enum class MenuMode {
	EXIT = -10000,
	REPEATE,
	PressEsc
};

void SetColor(int color = 7);

template <typename T>
void SetColor(int color, T& str) {
	SetColor(color);
	cout << str;
	SetColor();
}

template <typename T, typename... Ttail>
void SetColor(int color, T& str, const Ttail&... tail) {
	SetColor(color, str);
	SetColor(color, tail...);
}


class MenuItem{
	friend class Menu;
private:
	int index = 0;
	string nameMenuItem;
	int (*Func)() = nullptr;
	int (*IntFunc)(int) = nullptr;
	int returnValue = -1;

public:
	MenuItem(string title = "#", int (*FuncOfItem)() = nullptr);

	MenuItem(string title, int (*FuncOfItem)(int), int value);
};

class Menu : MenuItem{
private:
	string title;
	vector<MenuItem> menuItems;

	void ViewItems(int key);

	void SetMenuItems(vector <MenuItem> items);

	int ShowMenu();

	int ShowMenu(size_t choice, size_t& selectTime, size_t& switchTime);

public:
	Menu(string title = "#", vector <MenuItem>* items = new vector<MenuItem>);

	int DoMenu(initializer_list<size_t> order = initializer_list<size_t>(),
		size_t selectTime = 800, size_t switchTime = 550);
};


#endif