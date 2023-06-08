#include"Synt.h"

int main()
{
	setlocale(LC_ALL, "rus");
	synt Auto;
	Auto.start();
	bool flag = Auto.dissasemble();
	if (flag)
		cout << "программа прочитана";
	else
		cout << "программа не прочитана";
	return 0;
}