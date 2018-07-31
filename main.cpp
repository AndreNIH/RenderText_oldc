#include "HAPIH.h"
#include <iostream>
#include <string>
bool changeProtection(HackIH &h,PointerIH &p)
{
	HWND hwnd = FindWindow(NULL, "Geometry Dash");
	if (hwnd == NULL) return false;
	DWORD processID;
	GetWindowThreadProcessId(hwnd, &processID);
	if (processID == NULL)	return false;
	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (process_handle == NULL) return false;
	DWORD oldProtect = NULL;
	if (VirtualProtectEx(process_handle, h.GetPointerAddress(p), sizeof(int), PAGE_READWRITE, &oldProtect) == NULL)
	{
		CloseHandle(process_handle);
		return false;
	}
	return true;
}


int main()
{

	
	const long long force_draw = 0x0F0200000494bb80;
	const long long usual_draw = 0x0F0100000494bb80;
	
	HackIH process;
	std::cout << "Waiting for Geometry Dash\n";
	while (!process.bind("GeometryDash.exe")) { Sleep(200); }

	PointerIH text = { process.BaseAddress,0x002D7DD8 };
	PointerIH draw = { process.BaseAddress,0x001FD269 };

	if (!changeProtection(process, text))
	{
		std::cout << "\nCould not change memory protection... Quitting";
		std::cin.get();
		return 0;
	}
	std::string str;
	std::cout << "\n>>";
	std::getline(std::cin, str);
	const char *write_text = str.c_str() + '\0';
	process.SetDebugOutput(std::cout);
	process.Write(draw, force_draw);
	//process.Write(text, write_text);
	for(int index = 0; index <= str.size();index++) process.Write(text+index, write_text[index]);
	system("pause");
}
