#ifndef PROCESS_CPP
#define PROCESS_CPP

#include "config.h"

bool inside(short n){
	return (n >= 0 && n < 16);
};

// save game

// load game:

void loadGame(
	gameDataPackage& package,
	unsigned int& x_score,
	unsigned int& y_score,
	std::vector<sf::Vector2i>& moves,
	std::vector<std::vector<short> >& cells,
	std::wstring& return_message // if successful, return "success", else return the error's content
){
	return_message = L"success";
	std::ifstream fi(package.load_game_from);
	fi >> package.save_name;
	fi >> package.is_multiplayer;
	fi >> package.playerX_name;
	fi >> package.playerO_name;
	fi >> package.first_turn;
	fi >> x_score >> y_score;
	unsigned int n;
	unsigned short x, y;
	fi >> n;
	cells.resize(16, std::vector<short> (16,0));
	for(int i = 0; i < n; ++i){
		fi >> x >> y;
		if(!inside(x) || !inside(y)){
			if(globalConfig::language == 0){
				return_message = L"Save's corrupted: Invalid moves' coordinates.";
			} else{
				return_message = L"Lỗi tải ván chơi: Các nước đi được lưu không hợp lệ.";
			};
			fi.close();
			return;
		};
		moves.push_back({x, y});
		cells[x][y] = (i + 1 + package.first_turn) % 2 + 1;
	};
	if(!fi.eof()){
		if(globalConfig::language == 0){
			return_message = L"Save's corrupted: Invalid save format.";
		} else{
			return_message = L"Lỗi tải ván chơi: File lưu ván chơi không hợp lệ.";
		};
	};
	fi.close();
};

/*std::wstring is_load_game_success; //"success" for success, otherwise store the content of the error.
	gameDataPackage saveInfo;
	unsigned int x_score, o_score;
	std::vector<sf::Vector2i> moves;
	std::vector<std::vector<short> > cells;*/

void saveGame(
	gameDataPackage& saveInfo,
	unsigned int x_score,
	unsigned int o_score,
	std::vector<sf::Vector2i>& moves
){
	std::ofstream f;
	if(saveInfo.is_new_game){
		f.open("saves/" + saveInfo.save_name + ((saveInfo.is_multiplayer) ? ".txt" : ".txtx"));
	} else{
		f.open(saveInfo.load_game_from);
	};
	f << saveInfo.save_name << '\n';
	f << saveInfo.is_multiplayer << '\n';
	f << saveInfo.playerX_name << '\n';
	f << saveInfo.playerO_name << '\n';
	f << saveInfo.first_turn << '\n';
	f << x_score << ' ' << o_score << '\n';
	f << moves.size() << '\n';
	for(auto& i: moves){
		f << i.x << ' ' << i.y << '\n';
	};
	f.close();
};

// check for win
std::vector<sf::Vector2i> checkForWin(
	sf::Vector2i& last_move, 
	std::vector<std::vector<short> >& cells,
	bool& status
){
	std::vector<sf::Vector2i> result;
	result.push_back(last_move);
	bool status2 = true;
	auto check = [&last_move, &cells, &status, &status2, &result](short _x, short _y){
		std::vector<sf::Vector2i> temp_res;
		int count = 1, i = 0;
		bool end_loop = false;
		while(!end_loop){
			end_loop = true;
			++i;
			if(
				(inside(last_move.x + (i * _x))) && (inside(last_move.y + (i * _y))) &&
				(cells[last_move.x + (i * _x)][last_move.y + (i * _y)] == cells[last_move.x][last_move.y])
			){
				++count;
				temp_res.push_back(sf::Vector2i(last_move.x + (i * _x), last_move.y + (i * _y)));
				end_loop = false;
			};
			if(
				(inside(last_move.x - (i * _x))) && (inside(last_move.y - (i * _y))) &&
				cells[last_move.x - (i * _x)][last_move.y - (i * _y)] == cells[last_move.x][last_move.y]
			){
				++count;
				temp_res.push_back(sf::Vector2i(last_move.x - (i * _x), last_move.y - (i * _y)));
				end_loop = false;
			};
			if(count >= 5){
				status2 *= false;
				for(auto i: temp_res) result.push_back(i);
			} else status2 *= true;
		};
	};
	
	check(1, 0);
	check(0, 1);
	check(1, 1);
	check(-1, 1);

	status = !status2;

	return result;
};

//check for draw
bool checkForDraw(std::vector<sf::Vector2i>& moves){
	if(moves.size() >= 16 * 16) return true;
	return false;
};


#endif