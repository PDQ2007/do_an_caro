#ifndef CONFIG_CPP
#define CONFIG_CPP

namespace globalConfig{
	unsigned int win_width = 1200, win_height = 800;
	bool dark_mode = 0;
	int current_win = 5; 
	// 0 = program terminated
	// 1 = menu
	// 2 = play screen
	// 3 = game screen
	// 4 = settings
	// 5 = in game settings
};

#endif