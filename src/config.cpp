#ifndef CONFIG_CPP
#define CONFIG_CPP

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>

namespace globalConfig{
	unsigned int win_width = 1200, win_height = 800;
	bool dark_mode = true, re_init = false, sound_on = false;
	int current_win = 1, previous_win = 1;
	// 0 = program terminated
	// 1 = menu
	// 2 = play screen
	// 3 = game screen
	// 4 = settings
	// 5 = in game settings

	short language = 1;
	// 0 = english
	// 1 = vietnamese
};

void print_sth(std::wstring text){
	std::wcout << L"Giả bộ như tui chuyển sang cửa sổ " << text << ".\n\n";
	std::wcout << L"Tui chỉ thiết kế 'Menu Screen' và 'Play Screen' để demo "
		<< L"cho mọi người thấy cấu trúc chung của chương trình. Mấy cửa sổ kia "
		<< L"tui lười làm quá nên tui in đỡ dòng này.\n\n---\n\n";
};

#endif