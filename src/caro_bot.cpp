#ifndef CARO_BOT_CPP
#define CARO_BOT_CPP

#include "config.h"

typedef long long longlong;

namespace { //<= START AMONYMOUS NAMESPACE

namespace internal {
	bool is_running = true;
	bool is_win = false;
	std::vector<std::vector<short> > cells;
	//std::vector<std::vector<longlong> > reward_for_defense(16, std::vector<longlong> (16, 0));
	std::vector<sf::Vector2i> moves, next_moves, adj_cells;
	//std::vector<std::vector<std::vector<int> > > mark_score_grid;

/*
*			SCORE DISTRIBUTION
		Count		0	1	2	3	4			5
		Unblocked	0	10	20	100	99999999	99999999999
		Blocked		0	5	10	50	200			99999999999
*/

	//idx 0: count;   idx 1: is blocked
	int score_distribution[6][2] = {
		{ 
			0, 0
		},
		{ 
			10, 5
		},
		{ 
			100, 50
		},
		{ 
			3000, 1000
		},
		{ 
			2000000, 100000
		},
		{ // Count 5: WIN
			INT32_MAX, INT32_MAX
		}
	};

	void cleanUpForNewGame(){
		internal::cells.clear();
		internal::cells.resize(16, std::vector<short> (16, 0));
		{
			std::lock_guard<std::mutex> lock(gameStats::movesMutex);
			gameStats::next_moves = internal::next_moves;
		};
	};
};

bool is_same(char x, char y){
	if(x == 3) x = 1;
	else if(x == 4) x = 2;
	if(y == 3) y = 1;
	else if(y == 4) y = 2;
	return (x == y);
};

bool isInside(char x, char y){
	return ((x >= 0) && (x < 16) && (y >= 0) && (y < 16));
};

void addRandomMove(
	player first_player
){
	static int count = 0;
	srand(time(0) + count);
	unsigned char _x, _y;
	do{
		_x = 6 + rand() % 4;
		_y = 6 + rand() % 4;
		++count;
	} while(internal::cells[_x][_y] != 0);
	internal::cells[_x][_y] = first_player + 1;
	internal::moves.push_back(sf::Vector2i(_x, _y));
};

namespace algo {

	std::vector<sf::Vector2i> must_go_moves(player playAs){
		std::vector<sf::Vector2i> list_of_must_go_moves;
		sf::Vector2i this_move;
		char max_count = 0;
		if(internal::next_moves.empty()) this_move = internal::moves.back();
		else this_move = internal::next_moves.back();
		auto checkDirection = [&list_of_must_go_moves, &max_count, &playAs](const char& x, const char& y, char _X, char _Y){

			auto updateMaxCount = [&max_count, &list_of_must_go_moves](const char& count){
				if(max_count < count) list_of_must_go_moves.clear();
				max_count = std::max(count, max_count);
			};

			char count = 1, idx_back = -1, idx_front = -1;
			for(int i = -1;; --i){
				if(isInside(x + i * _X, y + i * _Y) && !is_same(internal::cells[x][y], playAs + 1)
					&& is_same(internal::cells[x][y], internal::cells[x + i * _X][y + i * _Y])
				){
					++count;
					continue;
				} else if(isInside(x + i * _X, y + i * _Y) && internal::cells[x + i * _X][y + i * _Y] == 0){
					idx_back = -1 * i;
				};
				break;
			};

			for(int i = 1;; ++i){
				if(isInside(x + i * _X, y + i * _Y) && !is_same(internal::cells[x][y], playAs + 1)
					&& is_same(internal::cells[x][y], internal::cells[x + i * _X][y + i * _Y])
				){
					++count;
					continue;
				} else if(isInside(x + i * _X, y + i * _Y) && internal::cells[x + i * _X][y + i * _Y] == 0){
					idx_front = i;
				};
				break;
			};

			bool unblocked = (idx_back > -1 && idx_front > -1);
			if(count >= 4 && unblocked){
				updateMaxCount(5);
				list_of_must_go_moves.push_back({x - idx_back * _X, y - idx_back * _Y});
				list_of_must_go_moves.push_back({x + idx_front * _X, y + idx_front * _Y});
			} else if(max_count < 5 && count >= 4 && !unblocked && (idx_back + idx_front > -2)){
				updateMaxCount(4);
				list_of_must_go_moves.push_back({
					x + ((idx_back > -1) ? (-idx_back) : idx_front) * _X,
					y + ((idx_back > -1) ? (-idx_back) : idx_front) * _Y
				});
			} else if(max_count < 4 && count == 3 && unblocked){
				//updateMaxCount(3);
				//list_of_must_go_moves.push_back({x - idx_back * _X, y - idx_back * _Y});
				//list_of_must_go_moves.push_back({x + idx_front * _X, y + idx_front * _Y});
			};
		};

		checkDirection((char)this_move.x, (char)this_move.y, 1, 1);
		checkDirection((char)this_move.x, (char)this_move.y, 1, -1);
		checkDirection((char)this_move.x, (char)this_move.y, 0, 1);
		checkDirection((char)this_move.x, (char)this_move.y, 1, 0);

		return list_of_must_go_moves;
	};

	std::vector<sf::Vector2i> adjacent_cells(){

		std::vector<sf::Vector2i> result;
		std::vector<std::vector<bool> > mark_cells(16, std::vector<bool> (16, 0));

		auto addAdjacent = [&result, &mark_cells](sf::Vector2i& this_move){
			for(int i = -1; i <= 1; ++i){
				for(int j = -1; j <= 1; ++j){
					if(!isInside(this_move.x + i, this_move.y + j)
						|| internal::cells[this_move.x + i][this_move.y + j] != 0
						|| i == 0 && j == 0
						) continue;
					if(mark_cells[this_move.x + i][this_move.y + j]) continue;
					mark_cells[this_move.x + i][this_move.y + j] = true;
					result.push_back({this_move.x + i, this_move.y + j});
				};
			};
		};

		for(auto& i: internal::moves){
			addAdjacent(i);
		};
		for(auto& i: internal::next_moves){
			addAdjacent(i);
		};

		for(int i = 0; i < 10; ++i){
			int
				_x = rand() % result.size(),
				_y = rand() % result.size();
			std::swap(result[_x], result[_y]);
		};

		return result;
	};

	std::vector<sf::Vector2i> moves_to_iterate(player playAs){
		auto return_list = must_go_moves(playAs);
		if(return_list.empty()) return_list = adjacent_cells();
		return return_list;
	};

	std::vector<std::vector<longlong>> calculateDefenseReward(player rewardFor){

		std::vector<std::vector<longlong> > reward_for_defense(16, std::vector<longlong> (16, 0));

		//std::cout << "--------------------\n";

		auto checkDirection = [&rewardFor, &reward_for_defense](char x, char y, char _X, char _Y){

			//std::cout << (int)x << ' ' << (int)y << ' ' << (int)_X << ' ' << (int)_Y << " => ";

			char mul = (is_same(rewardFor, X)) ? 1 : -1;

			char count = 1, blocked = 0;

			for(int i = -1;; --i){
				if(isInside(x + i * _X, y + i * _Y)){
					if(internal::cells[x + i * _X][y + i * _Y] == 0){
						break;
					} else if(!is_same(rewardFor + 1, internal::cells[x + i * _X][y + i * _Y])){
						++count;
					} else if(is_same(rewardFor + 1, internal::cells[x + i * _X][y + i * _Y])){
						++blocked;
						break;
					};
				} else{
					++blocked;
					break;
				};
			};

			for(int i = 1;; ++i){
				if(isInside(x + i * _X, y + i * _Y)){
					if(internal::cells[x + i * _X][y + i * _Y] == 0){
						break;
					} else if(!is_same(rewardFor + 1, internal::cells[x + i * _X][y + i * _Y])){
						++count;
					} else if(is_same(rewardFor + 1, internal::cells[x + i * _X][y + i * _Y])){
						++blocked;
						break;
					};
				} else{
					++blocked;
					break;
				};
			};

			if(blocked > 1){
				//std::cout << " '0' \n";
				return;
			};
			//std::cout << internal::reward_for_defense[x][y] << " (" << mul * internal::score_distribution[std::min(count, (char)5)][blocked] << ") \n";
			reward_for_defense[x][y] += mul * internal::score_distribution[std::min(count, (char)5)][blocked];
		};

		auto list_adj_cell = adjacent_cells();

		for(auto& i: list_adj_cell){
			checkDirection(i.x, i.y, 1, 1);
			checkDirection(i.x, i.y, -1, 1);
			checkDirection(i.x, i.y, 0, 1);
			checkDirection(i.x, i.y, 1, 0);
		};

		return reward_for_defense;

	};

	longlong calculateEvaluation(player playAs, player first_player){

#define WON internal::next_won.back()

		std::vector<std::vector<char> > mark_checked(16, std::vector<char> (16, 0));

		longlong result = 0;

		auto checkDirection = [&mark_checked](char x, char y, char _X, char _Y) -> int {

			char shift = 1;
			if(_X < 0 && _Y > 0) shift = 3;
			else if(_X == 0 && _Y > 0) shift = 2;
			else if(_X > 0 && _Y > 0) shift = 1;
			else if(_X > 0 && _Y == 0) shift = 0;

			if((mark_checked[x][y] & (1 << shift))) return 0;

			bool blocked_top = false, blocked_bottom = 0;
			int count = 1, last_idx;

			if(!isInside(x - _X, y - _Y) || internal::cells[x-_X][y-_Y] != 0) blocked_top = true;

			for(int i = 1;; ++i){
				if(isInside(x + i*_X, y + i*_Y)){
					if(is_same(internal::cells[x][y], internal::cells[x + i*_X][y + i*_Y])){
						++count;
						continue;
					} else if(internal::cells[x + i*_X][y + i*_Y] != 0){
						blocked_bottom = true;
					} else if(internal::cells[x + i*_X][y + i*_Y] == 0){
						blocked_bottom = false;
					};
				} else{
					blocked_bottom = true;
				};
				last_idx = i;
				break;
			};

			count = std::min(count, 5);

			if(count >= 1 && blocked_bottom + blocked_top < 2){
				for(int i = 0; i < last_idx; ++i){
					mark_checked[x + i * _X][y + i * _Y] |= (1 << shift);
				};
				return internal::score_distribution[count][blocked_top + blocked_bottom];
			};
			return 0;
		};

		for(int i = 0; i < 16; ++i){
			for(int j = 0; j < 16; ++j){
				if(internal::cells[i][j] == 0) continue;
				char mul = (is_same(internal::cells[i][j], X + 1)) ? 1 : -1;
				result += mul * checkDirection(i, j, 1, 0);
				result += mul * checkDirection(i, j, 1, 1);
				result += mul * checkDirection(i, j, 0, 1);
				result += mul * checkDirection(i, j, -1, 1);
			};
		};

#undef WON

		return result;

	};

	//std::pair<longlong, sf::Vector2i> minimaxWithPruning(
	//	sf::Vector2i this_move,
	//	char depth,
	//	player playAs,
	//	player firstPlayer,
	//	longlong alpha,
	//	longlong beta
	//){

	//	if(!internal::is_running){
	//		return {0, {0,0}};
	//	};

	//	//printCells();
	//	//auto reward_for_defense = calculateDefenseReward(playAs);

	//	internal::cells[this_move.x][this_move.y] = playAs + 3;
	//	internal::next_moves.push_back(this_move);

	//	auto updateNextMoves = [](){
	//		std::lock_guard<std::mutex> lock(gameStats::movesMutex);
	//		gameStats::next_moves = internal::next_moves;
	//	};
	//	updateNextMoves();
	//	if(depth == 0){
	//		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//		//for(auto& i: internal::next_moves){
	//		//	std::cout << '(' << i.x << ',' << i.y << ") ";
	//		//};
	//		internal::next_moves.pop_back();
	//		longlong result = calculateEvaluation(playAs, firstPlayer);
	//		
	//		//std::cout << " -- " << result << '\n';
	//		internal::cells[this_move.x][this_move.y] = 0;
	//		updateNextMoves();
	//		return {result, this_move};
	//	} else{
	//		std::pair<longlong, sf::Vector2i> best_eval;
	//		if(playAs == X){
	//			best_eval = {INT64_MIN, {0, 0}};
	//			std::pair<longlong, sf::Vector2i> t_eval;
	//			auto cells_to_iterate = moves_to_iterate(O);
	//			for(auto& i: cells_to_iterate){
	//				t_eval = minimaxWithPruning(i, depth-1, O, firstPlayer, alpha, beta);
	//				if(t_eval.first > best_eval.first) best_eval = t_eval;
	//				alpha = std::max(best_eval.first, alpha);
	//				if(alpha >= beta) break;
	//			};
	//		} else{
	//			best_eval = {INT64_MAX, {0, 0}};
	//			std::pair<longlong, sf::Vector2i> t_eval;
	//			auto cells_to_iterate = moves_to_iterate(X);
	//			for(auto& i: cells_to_iterate){
	//				t_eval = minimaxWithPruning(i, depth-1, X, firstPlayer, alpha, beta);
	//				if(t_eval.first < best_eval.first) best_eval = t_eval;
	//				beta = std::min(best_eval.first, beta);
	//				if(alpha >= beta) break;
	//			};
	//		};
	//		internal::next_moves.pop_back();
	//		internal::cells[this_move.x][this_move.y] = 0;
	//		updateNextMoves();
	//		return best_eval;
	//	};

	//};

	//old minimax function
	std::pair<longlong, sf::Vector2i> minimaxWithPruning(
		sf::Vector2i this_move,
		char depth,
		player playAs,
		player firstPlayer,
		longlong alpha,
		longlong beta,
		longlong defense_reward = 0
	){

		if(!internal::is_running){
			return {0, {0,0}};
		};

		//printCells();
		auto reward_for_defense = calculateDefenseReward(playAs);

		internal::cells[this_move.x][this_move.y] = playAs + 3;
		internal::next_moves.push_back(this_move);

		auto updateNextMoves = [](){
			std::lock_guard<std::mutex> lock(gameStats::movesMutex);
			gameStats::next_moves = internal::next_moves;
		};
		updateNextMoves();
		if(depth == 0){
			internal::next_moves.pop_back();
			longlong result = (int)(0.8 * (reward_for_defense[this_move.x][this_move.y] + defense_reward));
			result += calculateEvaluation(playAs, firstPlayer);
			internal::cells[this_move.x][this_move.y] = 0;
			updateNextMoves();
			return {result, this_move};
		} else{
			std::pair<longlong, sf::Vector2i> best_eval;
			if(playAs == X){
				best_eval = {INT64_MIN, {0, 0}};
				std::pair<longlong, sf::Vector2i> t_eval;
				auto cells_to_iterate = moves_to_iterate(O);
				auto reward_for_defense = calculateDefenseReward(O);
				for(auto& i: cells_to_iterate){
					t_eval = minimaxWithPruning(i, depth-1, O, firstPlayer, alpha, beta, defense_reward + reward_for_defense[this_move.x][this_move.y]);
					if(t_eval.first > best_eval.first) best_eval = t_eval;
					alpha = std::max(best_eval.first, alpha);
					if(alpha >= beta) break;
				};
			} else{
				best_eval = {INT64_MAX, {0, 0}};
				std::pair<longlong, sf::Vector2i> t_eval;
				auto cells_to_iterate = moves_to_iterate(X);
				auto reward_for_defense = calculateDefenseReward(X);
				for(auto& i: cells_to_iterate){
					t_eval = minimaxWithPruning(i, depth-1, X, firstPlayer, alpha, beta, defense_reward + reward_for_defense[this_move.x][this_move.y]);
					if(t_eval.first < best_eval.first) best_eval = t_eval;
					beta = std::min(best_eval.first, beta);
					if(alpha >= beta) break;
				};
			};
			internal::next_moves.pop_back();
			internal::cells[this_move.x][this_move.y] = 0;
			updateNextMoves();
			return best_eval;
		};
	
	};

};

void readGameState(std::vector<sf::Vector2i>& moves, std::vector<std::vector<short> >& cells){
	internal::cells = cells;
	internal::moves = moves;
};

}; //<= END AMONYMOUS NAMESPACE

void bot_playing(player playAs, player firstPlayer, std::vector<sf::Vector2i>& moves, std::vector<std::vector<short> >& cells, int depth){
	internal::cleanUpForNewGame();
	readGameState(moves, cells);
	internal::is_running = true;
	if(internal::moves.size() == 0){
		addRandomMove(playAs);
	} else{
		auto cells_to_iterate = algo::moves_to_iterate(playAs);
		std::pair<longlong, sf::Vector2i> t_eval, best_eval;
		if(playAs == X){
			best_eval = {INT64_MIN, {0,0}};
		} else if(playAs == O){
			best_eval = {INT64_MAX, {0,0}};
		};
		for(auto& i: cells_to_iterate){
			t_eval = algo::minimaxWithPruning(i, gameStats::difficulty, playAs, firstPlayer, INT64_MIN, INT64_MAX);
			//std::cout << "t-eval: " << t_eval.second.x << " " << t_eval.second.y << " => " << t_eval.first << '\n';
			if(playAs == X && t_eval.first > best_eval.first){
				best_eval = {t_eval.first, i};
			} else if(playAs == O && t_eval.first < best_eval.first){
				best_eval = {t_eval.first, i};
			};
			{
				std::lock_guard<std::mutex> lock(gameStats::movesMutex);
				gameStats::next_moves = internal::next_moves;
			};
			if(!internal::is_running) break;
		};
		internal::moves.push_back(best_eval.second);
		//std::cout << "BEST=" << best_eval.first << '\n';
	};
	if(internal::is_running){
		{
			std::lock_guard<std::mutex> lock(gameStats::movesMutex);
			cells[internal::moves[internal::moves.size()-1].x][internal::moves[internal::moves.size()-1].y] = playAs + 1;
			gameStats::next_moves = internal::next_moves;
			moves.push_back(internal::moves[internal::moves.size()-1]);
		};
	};
};

void kill_bot(){
	//std::cout << "Killed bot!\n";
	for(auto& i: internal::next_moves){
		internal::cells[i.x][i.y] = 0;
	};
	internal::next_moves.clear();
	internal::is_running = false;
};

#endif