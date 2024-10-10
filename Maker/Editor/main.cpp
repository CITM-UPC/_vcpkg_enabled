#include <iostream>
using namespace std;
#include "Engine/GameObject.h"

int main(int argc, char** argv)
{
	GameObject go;
	go.Func();
	cout << "from editor" << endl;
	return 0;
}