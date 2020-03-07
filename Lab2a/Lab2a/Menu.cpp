#include "Menu.h"

void SetColor(int color){
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(color));
}

MenuItem::MenuItem(string title, int (*FuncOfItem)()){
	nameMenuItem = title;
	Func = FuncOfItem;
}

MenuItem::MenuItem(string title, int (*FuncOfItem)(int), int value){
	returnValue = value;
	nameMenuItem = title;
	IntFunc = FuncOfItem;
}

void Menu::ViewItems(int key){
	cout << title << "\n\n";
	int j = 1;
	//todo: firstly draw defult
	for (vector <MenuItem> ::iterator i = menuItems.begin(); i < menuItems.end(); i++, j++){
		if (key == j)
			SetColor(48, " >> ", i->nameMenuItem, '\n');
		else
			cout <<"    "<< i->nameMenuItem << '\n';
	}
	SetColor(6, "\n", endMessage);
	//todo: draw selected
}

void Menu::SetMenuItems(std::initializer_list <MenuItem> items) {
	menuItems = items;
	for (int i = 0, n = items.size(); i < n; ++i) {
		menuItems[i].index = i + 1;
	}
}

int Menu::ShowMenu(const size_t& choice, const size_t& selectTime,const size_t& switchTime){
	//todo: ViewItems() here defult selected item = 1
	for(size_t key = 1; key <= choice; key++) {
		system("cls");
		//todo: ReDrawItems({key, key - 1});
		ViewItems(key);
		Sleep(switchTime);
	}
	Sleep(selectTime);
	system("cls");
	return choice;
}

int Menu::ShowMenu() {
	size_t key = 1;
	int code = 0;
	while (code != 13) {
		system("cls");

		if (key == 0) key = menuItems.size();
		if (key > menuItems.size()) key = 1;

		ViewItems(key);

		code = _getch();
		if (code == 224) {
			code = _getch();
			if (code == 80)
				key++;
			if (code == 72)
				key--;

		}
		if (code == 27)
			return (int) MenuMode::PressEsc;
	}
	system("cls");

	return key;
}

Menu::Menu(string title, std::initializer_list <MenuItem> items, string endMessage) :
	title(title), endMessage(endMessage) {
	//count enter count and ++ in private var(defult = 2)
	SetMenuItems(items);
}

int Menu::DoMenu(initializer_list<size_t> order, size_t selectTime, size_t totalSwitchTime) {
	int currentKey = 0, ans;

	while (true) {
		if(order.size() == 0) ans = ShowMenu();
		else {
			if (currentKey == order.size())
				return (int)MenuMode::EXIT;
			size_t choice = *(order.begin() + currentKey++);
			
			if (choice != 0)
				ans = ShowMenu(choice, selectTime, Between(totalSwitchTime / choice, 200, 500));
			else ans = ShowMenu();
		}

		bool isEmpty = menuItems.size() == 0;

		if (ans == (int)MenuMode::PressEsc || isEmpty)
			return (int)MenuMode::EXIT;

		bool withoutFunc = (menuItems[ans - 1].Func == nullptr &&
			menuItems[ans - 1].IntFunc == nullptr);
		if (withoutFunc)  return menuItems[ans - 1].returnValue;

		int result;
		if (menuItems[ans - 1].Func != nullptr)
			result = menuItems[ans - 1].Func();
		else 
			result = menuItems[ans - 1].IntFunc(menuItems[ans - 1].returnValue);
		if (result != (int)MenuMode::REPEATE) return result;
	}
}

size_t Between(size_t value, size_t left, size_t right) {
	return max(min(value, right), left);
}

	
