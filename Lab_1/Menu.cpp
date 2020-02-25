#include <Windows.h>
#include "Header.h"
#include <conio.h>
#include <vector>


#pragma region Declaration

using std::vector;
using std::to_string;


int ChoiceMode();
int Interactive(int mode);
int Demonstration();
int Benchmark();

int ShopChoice();
int ShopCreate();
int ShopFind();

int Add();
void AddBin();
void AddVector();
void AddTxt();

int ShowAll();
bool ShowAllVector();
bool ShowAllTxt();
bool ShowAllBin();

int Search();
void SearchName();
void SearchAmount();
void SearchDate();

int shopId = 0; 
int ShopId() {
	return shopId;
}
Mode workMode = Mode::WAIT;

enum class MenuMode {
	EXIT = -10000,
	REPEATE,
	PressEsc
};

#pragma endregion


#pragma region Menu classes

void SetColor(int color)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(color));
}

void SetColor(int color, const char* str){
	SetColor(color);
	cout << str << endl;
	SetColor();
}

class MenuItem
{
	friend class Menu;
private:
	int index = 0;
	string nameMenuItem;
	int (*Func)() = nullptr;
	int (*IntFunc)(int) = nullptr;
	int returnValue = -1;

public:
	MenuItem(string title = "#", int (*FuncOfItem)() = nullptr){
		nameMenuItem = title;
		Func = FuncOfItem;
	}

	MenuItem(string title, int (*FuncOfItem)(int), int value){
		returnValue = value;
		nameMenuItem = title;
		IntFunc = FuncOfItem;
	}
};

class Menu : MenuItem
{
private:
	string title;
	vector<MenuItem> menuItems;

	void ViewItems(int key){
		SetColor();
		cout << title << "\n\n";

		int j = 1;
		for (vector <MenuItem> ::iterator i = menuItems.begin(); i < menuItems.end(); i++, j++){
			if (key == j){
				SetColor(48);
				cout <<" >> "<< i->nameMenuItem << '\n';
				SetColor();
			}
			else{
				cout <<"    "<< i->nameMenuItem << '\n';
			}
		}

		SetColor(6, "\nPress ESC to go back or exit");
	}

	void SetMenuItems(vector <MenuItem> items) {
		menuItems = items;
		for (int i = 0, n = items.size(); i < n; ++i)
			menuItems[i].index = i + 1;
	}

	int ShowMenu() {
		size_t key = 1;
		int code = 0;
		while (code != 13) {
			system("cls");

			if (key == 0) key = menuItems.size();
			if (key > menuItems.size()) key = 1;

			ViewItems(key);

			code = _getch();

			if (code == 80)
				key++;
			if (code == 72)
				key--;

			if (code == 27)
				return (int) MenuMode::PressEsc;
		}
		system("cls");

		return key;
	}

public:
	Menu(string title = "#", vector <MenuItem>* items = new vector<MenuItem>){
		this->title = title;
		SetMenuItems(*items);
		delete items;
	}

	int DoMenu(size_t sleepTime = 0) { // todo: int sleepTime
		while (true){
			int ans = ShowMenu();
			bool isEmpty = menuItems.size() == 0;

			if (ans == (int)MenuMode::PressEsc || isEmpty)
				return (int)MenuMode::EXIT;

			bool withoutFunc = (menuItems[ans - 1].Func == nullptr &&
								menuItems[ans - 1].IntFunc == nullptr);
			if (withoutFunc) {
				return menuItems[ans - 1].returnValue;
			}
			int result;
			if (menuItems[ans - 1].Func != nullptr) {
				result = menuItems[ans - 1].Func();
			}
			else {
				result = menuItems[ans - 1].IntFunc(menuItems[ans - 1].returnValue);
			}
			if (result != (int)MenuMode::REPEATE)
				return result;
		}
	}
};

#pragma endregion


#pragma region Start

int StartMenu() {
	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Interactive", ChoiceMode));
	(*all).push_back(MenuItem("Demonstration", Demonstration));
	(*all).push_back(MenuItem("Benchmark", Benchmark));
	
	Menu* menu = new Menu("Choose your mode:", all);
	(*menu).DoMenu();
	delete menu;

	return (int)MenuMode::EXIT;
}

int ChoiceMode() {
	vector<MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Vector", Interactive, (int)Mode::VECTOR));
	(*all).push_back(MenuItem("Text", Interactive, (int)Mode::TXT));
	(*all).push_back(MenuItem("Binary", Interactive, (int)Mode::BIN));

	Menu* menu = new Menu("Choose your read/write mode", all);
	(*menu).DoMenu();
	delete menu;

	return (int)MenuMode::REPEATE;
}

int Interactive(int mode) {
	workMode = Mode(mode);

	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Find shop", ShopFind));
	(*all).push_back(MenuItem("Choose shop", ShopChoice));
	(*all).push_back(MenuItem("Create new", ShopCreate));

	Menu* menu = new Menu("Choose or create shop:", all);
	shopId = (*menu).DoMenu();
	delete menu;

	if (shopId == (int)MenuMode::EXIT)
		return (int)MenuMode::REPEATE;
	//todo: switct workmode
	CreatePathBin(to_string(shopId).c_str());
	CreatePathTxt(to_string(shopId).c_str());
	SetLastIdBin();
	SetLastIdTxt();

	all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Add", Add));
	(*all).push_back(MenuItem("Show all", ShowAll));
	(*all).push_back(MenuItem("Search", Search));
	(*all).push_back(MenuItem("Modify"));
	(*all).push_back(MenuItem("Delete"));

	menu = new Menu("Interactive mode", all);
	(*menu).DoMenu();
	delete menu;

	return (int)MenuMode::REPEATE;
}

int Demonstration() {
	Menu* menu = new Menu("Demonstration...");
	(*menu).DoMenu();
	delete menu;
	return (int) MenuMode::REPEATE;
}

int Benchmark() {
	Menu* menu = new Menu("Benchmark...");
	(*menu).DoMenu();
	delete menu;
	return (int)MenuMode::REPEATE;
}

#pragma endregion


#pragma region Shop

int ShopChoice() {
	vector <MenuItem>* all = new vector<MenuItem>;
	int i = 0;
	while (true) {
		Store* a = TakeStore(i++);
		if (a == nullptr) break;
		(*all).push_back(MenuItem(a->name, nullptr, a->id));
		delete a;
	}

	if (all->size() == 0) {
		cout << "Don't find any shops. Create new...\n\n";
		system("pause");
		delete all;
		return ShopCreate();
	}
	else {
		Menu* menu = new Menu("Choose your shop:", all);
		int result = (*menu).DoMenu();
		delete menu;

		return result;
	}
}

int ShopCreate() {
	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Random", nullptr, 1));
	(*all).push_back(MenuItem("Yourself", nullptr, 2));

	Menu* menu = new Menu("Fill type:", all);
	int type = (*menu).DoMenu();
	delete menu;

	Store* newStore = new Store();
	if (type == 1) {
		cout << "Enter count of add stores: ";
		int n;
		InputStr(n);
		while (n < 1) {
			SetColor(6, "\tcount > 0!");
			InputStr(n);
		}
		for (int i = 0; i < n; i++) {
			newStore->Randomaze();
			AppendStore(newStore);
		}
	}
	else {

		cout << "Enter shop's name: ";
		InputStr(newStore->name);
		cout << "(perfect)\n" << endl;

		cout << "Enter shop's adress: ";
		InputStr(newStore->adress);
		cout << "(perfect)\n" << endl;

		cout << "Enter shop's rating: ";
		InputStr(newStore->rating);
		while (newStore->rating < 0 || newStore->rating > 10) {
			SetColor(6, "\tRating should be in interval [0,10]. Try again");
			InputStr(newStore->rating);
		}
		cout << "(perfect)\n" << endl;

		cout << "Enter shop's maximum of product count: ";
		InputStr(newStore->maxProductCount);
		while (newStore->maxProductCount < 1) {
			SetColor(6, "\tReal?) Product count should be more than 0. Try again");
			InputStr(newStore->maxProductCount);
		}

		newStore->id = GetLastIdStore();

		AppendStore(newStore);
	}
	cout << "\nSuccess" << endl;
	delete newStore;
	system("pause");
	return (int)MenuMode::REPEATE;
}

int ShopFind() {
	Store* a = TakeStore(0);
	if (a == nullptr) {
		cout << "Don't find any shops. Create new...\n\n";
		system("pause");
		return ShopCreate();
	}
	else {
		//�������� �� ������� ���������� ������ � ����, � ���� �� ������
		//	����� ����� ����������� �� ����� ��������.
		vector <MenuItem>* all = new vector<MenuItem>;
		int i = 0;
		while (true) {
			Store* a = TakeStore(i++);
			if (a == nullptr) break;
			(*all).push_back(MenuItem(a->name, nullptr, a->id));
			delete a;
		}
		Menu* menu = new Menu("Choose your shop:", all);
		int result = (*menu).DoMenu();
		delete menu;

		return result;
	}



	return (int)MenuMode::REPEATE;
}

#pragma endregion


#pragma region Add

void AddTxt() {
	ProductString* newProduct = new ProductString();

	cout << "Enter product's name: ";
	getline(cin, newProduct->name);
	cout << "(perfect)\n\n";
	Capitalize(newProduct->name);

	cout << "Enter product's date in format day.mount.year hours:minutes (15.02.2002 21:10)" << endl;
	InputStr(newProduct->date);
	cout << "(perfect)\n\n";

	cout << "Enter product's expiration days:" << endl;
	InputStr(newProduct->expirationDate);
	cout << "(perfect)\n\n";

	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Bag", nullptr, (int)Units::BAG));
	(*all).push_back(MenuItem("Kilogramms", nullptr, (int)Units::KILOGRAMMS));
	(*all).push_back(MenuItem("Liters", nullptr, (int)Units::LITERS));
	(*all).push_back(MenuItem("Piece", nullptr, (int)Units::PIECE));

	Menu* menu = new Menu("Choose units of product", all);
	int unitId = (*menu).DoMenu();
	delete menu;

	if (unitId == (int)MenuMode::EXIT) {
		delete newProduct;
		return;
	}

	newProduct->units = Units(unitId);

	cout << "Enter product's count in " << UnitsToString(unitId) << ":\n";

	InputStr(newProduct->count);
	if (Units(unitId) == Units::BAG || Units(unitId) == Units::PIECE)
		newProduct->count = trunc(newProduct->count);
	system("cls");

	newProduct->storeId = shopId;
	newProduct->id = GetLastIdTxt();

	AppendProductTxt(newProduct);
	cout << newProduct->ToString() << endl;
	delete newProduct;
}

void AddVector() {
	Product* newProduct = new Product();

	cout << "Enter product's name: ";
	InputStr(newProduct->name);
	cout << "(perfect)\n\n";

	cout << "Enter product's date in format day.mount.year hours:minutes (15.02.2002 21:10)" << endl;
	InputStr(newProduct->date);
	cout << "(perfect)\n\n";

	cout << "Enter product's expiration days:" << endl;
	InputStr(newProduct->expirationDate);
	cout << "(perfect)\n\n";

	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Bag", nullptr, (int)Units::BAG));
	(*all).push_back(MenuItem("Kilogramms", nullptr, (int)Units::KILOGRAMMS));
	(*all).push_back(MenuItem("Liters", nullptr, (int)Units::LITERS));
	(*all).push_back(MenuItem("Piece", nullptr, (int)Units::PIECE));

	Menu* menu = new Menu("Choose units of product", all);
	int unitId = (*menu).DoMenu();
	delete menu;

	if (unitId == (int)MenuMode::EXIT) {
		delete newProduct;
		return;
	}

	newProduct->units = Units(unitId);

	cout << "Enter product's count in " << UnitsToString(unitId) << ":\n";


	InputStr(newProduct->count);
	if (Units(unitId) == Units::BAG || Units(unitId) == Units::PIECE)
		newProduct->count = trunc(newProduct->count);
	system("cls");

	newProduct->storeId = shopId;
	newProduct->id = GetLastIdVector() + 1;

	AppendProductVector(newProduct);
	cout << newProduct->ToString() << endl;
	delete newProduct;
}

void AddBin() {
	Product* newProduct = new Product();

	cout << "Enter product's name: ";
	InputStr(newProduct->name);
	cout << "(perfect)\n\n";

	cout << "Enter product's date in format day.mount.year hours:minutes (15.02.2002 21:10)" << endl;
	InputStr(newProduct->date);
	cout << "(perfect)\n\n";

	cout << "Enter product's expiration days:" << endl;
	InputStr(newProduct->expirationDate);
	cout << "(perfect)\n\n";

	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Bag", nullptr, (int)Units::BAG));
	(*all).push_back(MenuItem("Kilogramms", nullptr, (int)Units::KILOGRAMMS));
	(*all).push_back(MenuItem("Liters", nullptr, (int)Units::LITERS));
	(*all).push_back(MenuItem("Piece", nullptr, (int)Units::PIECE));

	Menu* menu = new Menu("Choose units of product", all);
	int unitId = (*menu).DoMenu();
	delete menu;

	if (unitId == (int)MenuMode::EXIT) {
		delete newProduct;
		return;
	}

	newProduct->units = Units(unitId);

	cout << "Enter product's count in " << UnitsToString(unitId) << ":\n";


	InputStr(newProduct->count);
	if (Units(unitId) == Units::BAG || Units(unitId) == Units::PIECE)
		newProduct->count = trunc(newProduct->count);
	system("cls");

	newProduct->storeId = shopId;
	newProduct->id = GetLastIdBin();

	AppendProductBin(newProduct);
	cout << newProduct->ToString() << endl;
	delete newProduct;
}

int Add() {
	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("Random", nullptr, 1));
	(*all).push_back(MenuItem("Yourself", nullptr, 2));

	Menu* menu = new Menu("Fill type:", all);
	int type = (*menu).DoMenu();
	delete menu;

	if (type == (int)MenuMode::EXIT)
		return (int)MenuMode::REPEATE;

	if (type == 1) {
		cout << "Enter count of add products: ";
		int n;
		InputStr(n);
		while (n < 1) {
			SetColor(6, "\tcount > 0!");
			InputStr(n);
		}
		switch (workMode)
		{
		case Mode::VECTOR:
			AddVectorRandom(n);
			break;
		case Mode::TXT:
			AddTxtRandom(n);
			break;
		case Mode::BIN:
			AddBinRandom(n);
		}
	}
	else
		switch (workMode)
		{
		case Mode::VECTOR:
			AddVector();
			break;
		case Mode::TXT:
			AddTxt();
			break;
		case Mode::BIN:
			AddBin();
		}
	cout << "\nSuccess" << endl;
	system("pause");
	return (int)MenuMode::REPEATE;
}

#pragma endregion


#pragma region Show

bool ShowAllVector() {
	int i = 0;
	bool isProductFind = false;
	while (true) {
		Product* product = TakeProductVector(i++);
		if (product == nullptr) break;
		if (product->storeId == shopId) {
			isProductFind = true;
			cout << product->ToString() << endl;
		}
	}
	return isProductFind;

}

bool ShowAllTxt() {
	int i = 0;
	bool isProductFind = false;
	while (true) {
		ProductString* product = TakeProductTxt(i++);
		if (product == nullptr) 
			break;
		isProductFind = true;
		cout << product->ToString() << endl;
		delete product;
	}
	return isProductFind;
}

bool ShowAllBin() {
	int i = 0;
	bool isProductFind = false;
	while (true) {
		Product* product = TakeProductBin(i++);
		if (product == nullptr) 
			break;
		isProductFind = true;
		cout << product->ToString() << endl;
		delete product;
	}
	return isProductFind;
}

int ShowAll() {
	bool isProductFind = false;
	switch (workMode){
	case Mode::VECTOR:
		isProductFind = ShowAllVector();
		break;
	case Mode::TXT:
		isProductFind = ShowAllTxt();
		break;
	case Mode::BIN:
		isProductFind = ShowAllBin();
		break;
	}
	if (!isProductFind) 
		cout << "Store is empty\n\n";
	system("pause");
	return (int)MenuMode::REPEATE;
}

#pragma endregion


#pragma region Search

void SearchName() {
	cout << "Enter substring for name: ";
	size_t totalFind = 0, i = 0;
	switch (workMode){
	case Mode::VECTOR:{
		char* name = new char[nameSize];
		InputStr(name);
		system("cls");

		while (true) {
			Product* product = TakeProductVector(i++);
			if (product && SubString(product->name, name) && product->storeId == shopId) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
		}
	}
		break;
	case Mode::TXT: {
		string name;
		getline(cin, name);
		system("cls");

		while (true) {
			ProductString* product = TakeProductTxt(i++);
			if (product && SubString(product->name, name)) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
			delete product;
		}
	}
		break;
	case Mode::BIN: {
		char* name = new char[nameSize];
		InputStr(name);
		system("cls");

		while (true) {
			Product* product = TakeProductBin(i++);
			if (product && SubString(product->name, name) && product->storeId == shopId) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
			delete product;
		}
	}
		break;
	default:
		throw - 2;
	}
	cout << "Total find: " << totalFind << " products" << endl;
}

void SearchAmount() {
	cout << "Enter min count of product: ";
	int count;
	InputStr(count);
	while (count < 1) {
		SetColor(6, "count > 0!");
		InputStr(count);
	}
	system("cls");
	size_t totalFind = 0, i = 0;
	switch (workMode){
	case Mode::VECTOR: {
		while (true) {
			Product* product = TakeProductVector(i++);
			if (product && product->count >= count && product->storeId == shopId) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
		}
	}
		break;
	case Mode::TXT: {
		while (true) {
			ProductString* product = TakeProductTxt(i++);
			if (product && product->count >= count) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
			delete product;
		}
	}
				  break;
	case Mode::BIN: {
		while (true) {
			Product* product = TakeProductBin(i++);
			if (product && product->count >= count) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
			delete product;
		}
	}
		break;
	default:
		throw - 2;
	}
	cout << "Total find: " << totalFind << " products" << endl;
}

void SearchDate() {
	cout << "Enter data (in format 'day.mounth.year hour:minutes') to find products that will die: ";
	Date* date = new Date;
	InputStr(*date);
	system("cls");

	size_t totalFind = 0, i = 0;
	switch (workMode) {
	case Mode::VECTOR: {
		while (true) {
			Product* product = TakeProductVector(i++);
			if (product && product->date.DaysBetween(*date) >= product->expirationDate && product->storeId == shopId) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
		}
	}
					 break;
	case Mode::TXT: {
		while (true) {
			ProductString* product = TakeProductTxt(i++);
			if (product && product->date.DaysBetween(*date) >= product->expirationDate) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
			delete product;
		}
	}
				  break;
	case Mode::BIN: {
		while (true) {
			Product* product = TakeProductBin(i++);
			if (product && product->date.DaysBetween(*date) >= product->expirationDate) {
				cout << product->ToString() << endl;
				totalFind++;
			}
			if (product == nullptr)
				break;
			delete product;
		}
	}
				  break;
	default:
		throw - 2;
	}
	delete date;
	cout << "Total find: " << totalFind << " products" << endl;

}

int Search() {
	vector <MenuItem>* all = new vector<MenuItem>;
	(*all).push_back(MenuItem("By name", nullptr, 1));
	(*all).push_back(MenuItem("By amount", nullptr, 2));
	(*all).push_back(MenuItem("By date", nullptr, 3));

	Menu* menu = new Menu("Search type:", all);
	int type = (*menu).DoMenu();
	delete menu;

	if (type == (int)MenuMode::EXIT)
		return (int)MenuMode::REPEATE;

	switch (type)
	{
	case 1:
		SearchName();
		break;
	case 2:
		SearchAmount();
		break;
	case 3:
		SearchDate();
	}

	system("pause");
	return (int)MenuMode::REPEATE;
}

#pragma endregion 


//maybe add this in search
void f1() {
	string name = "";

	while (true)
	{
		int a = _getch();
		switch (a)
		{
		case 224:
		{
			int b = _getch();
			switch (b)
			{
			case 72: //up
			case 80: //down
			case 77:
			case 75:
				break;
			case 224: //p
				name += 'p';
				break;
			case 8: //backspace
				break;
			case 27: //esc
				break;
			case 13: //enter
				break;
			default:
				name += "p";
				name += char(b);
				break;
			}
			break;
		}
		case 8: //backspace
			if (name.length() > 0)
				name.pop_back();
			break;
		case 27: //esc
			return;
			break;
		case 13: //enter
			break;
		default:
			name += a;
			break;
		}
		system("cls");
		cout << name << endl << "ESC to exit";

		COORD cr;
		HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
		//cr.X = name.size();
		cr.Y = 0;
		SetConsoleCursorPosition(hnd, cr);
	}
}
