#ifndef CARO_BOT_CPP
#define CARO_BOT_CPP

#include "config.h"

typedef long long longlong;

namespace { //<= START AMONYMOUS NAMESPACE

namespace internal {
	bool is_running = true;
	bool is_win = false;
	std::vector<std::vector<short> > cells;
	std::vector<sf::Vector2i> moves, next_moves, adj_cells;
	//std::vector<std::vector<std::vector<int> > > mark_score_grid;
	std::vector<std::vector<std::vector<int> > >
		mark_defense_score(16, std::vector<std::vector<int> > (16, std::vector<int> (0)));

	int score_distribution[6][3] = {
		{0, 0, 0},
		{20, 10, 0},                   // 1 con: Live > Blocked > Dead
		{500, 100, 0},                 // 2 con
		{5000, 200, 0},                // 3 con: Live 3 (5000) > Blocked 3 (200)
		{200000, 10000, 0},            // 4 con: Live 4 (Thắng chắc) > Blocked 4
		{100000000, 100000000, 100000000} // 5 con: Thắng tuyệt đối
	};

	void cleanUpForNewGame(){
		{
			std::lock_guard<std::mutex> lock(gameStats::movesMutex);
			gameStats::next_moves = internal::next_moves;
		};
		mark_defense_score.clear();
		mark_defense_score.resize(16, std::vector<std::vector<int> > (16, std::vector<int> (0)));
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

	longlong calculateEvaluation(){
		std::vector<std::vector<char> > mark_checked(16, std::vector<char> (16, 0));
		
		longlong result = 0;

		auto checkDirection = [&mark_checked](char x, char y, char _X, char _Y) -> int {

			char shift = 1;
			if(_X < 0 && _Y > 0) shift = 3;
			else if(_X == 0 && _Y > 0) shift = 2;
			else if(_X > 0 && _Y > 0) shift = 1;
			else if(_X > 0 && _Y == 0) shift = 0;

			if((mark_checked[x][y] & (1 << shift)) == 1) return 0;

			bool blocked_top = false, blocked_bottom = false;
			int count = 1, last_idx;

			if(!isInside(x - _X, y - _Y) || internal::cells[x-_X][y-_Y] != 0) blocked_top = true;

			for(int i = 1; i <= 5; ++i){
				if(isInside(x + i*_X, y + i*_Y) 
					&& is_same(internal::cells[x][y], internal::cells[x + i*_X][y + i*_Y]) 
				){
					++count;
				} else{
					if(!isInside(x + i*_X, y + i*_Y) || internal::cells[x + i*_X][y + i*_Y] != 0){
						blocked_bottom = true;
					};
					last_idx = i;
					break;
				};
			};

			if(count >= 2){
				for(int i = 0; i < last_idx; ++i){
					mark_checked[x + i * _X][y + i * _Y] |= (1 << shift);
				};
				return internal::score_distribution[count][blocked_bottom + blocked_top];
			};

			return 0;

		};

		for(int i = 0; i < 16; ++i){
			for(int j = 0; j < 16; ++j){
				if(internal::cells[i][j] == 0) continue;
				char mul = (is_same(internal::cells[i][j], 1)) ? 1 : -1;
				result += mul * checkDirection(i, j, 1, 0);
				result += mul * checkDirection(i, j, 1, 1);
				result += mul * checkDirection(i, j, 0, 1);
				result += mul * checkDirection(i, j, -1, 1);
			};
		};

		return result;
	};

	std::vector<sf::Vector2i> adjacent_cells(){
		std::vector<sf::Vector2i> result;
		std::vector<std::vector<bool> > mark_cells(16, std::vector<bool> (16, 0));

		auto addAdjacent = [&result, &mark_cells](sf::Vector2i& this_move){
			for(int i = -1; i <= 1; ++i){
				for(int j = -1; j <= 1; ++j){
					if(!isInside(this_move.x + i, this_move.y + j)
						|| !internal::cells[this_move.x + i][this_move.y + j] == 0
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

std::pair<longlong, sf::Vector2i> minimaxWithPruning(
	sf::Vector2i this_move,
	char depth,
	player playAs,
	longlong alpha,
	longlong beta
){
	internal::cells[this_move.x][this_move.y] = 0;
	{
		std::lock_guard<std::mutex> lock(gameStats::movesMutex);
		gameStats::next_moves = internal::next_moves;
	};
	if(!internal::is_running){
		return {0, {0,0}};
	};

	internal::cells[this_move.x][this_move.y] = playAs + 3;

	internal::next_moves.push_back(this_move);
	{
		std::lock_guard<std::mutex> lock(gameStats::movesMutex);
		gameStats::next_moves = internal::next_moves;
	};
	if(depth == 0){
		internal::next_moves.pop_back();
		
		return {calculateEvaluation(), this_move};
	} else{
		if(playAs == X){
			std::pair<longlong, sf::Vector2i> best_eval = {INT64_MIN, {0, 0}}, t_eval;
			auto list_adj_cells = adjacent_cells();
			for(auto& i: list_adj_cells){
				t_eval = minimaxWithPruning(i, depth-1, O, alpha, beta);
				if(t_eval.first > best_eval.first) best_eval = t_eval;
				alpha = std::max(best_eval.first, alpha);
				if(alpha >= beta) break;
			};
			internal::next_moves.pop_back();
			internal::cells[this_move.x][this_move.y] = 0;
			return best_eval;
		} else{
			std::pair<longlong, sf::Vector2i> best_eval = {INT64_MAX, {0, 0}}, t_eval;
			auto list_adj_cells = adjacent_cells();
			for(auto& i: list_adj_cells){
				t_eval = minimaxWithPruning(i, depth-1, X, alpha, beta);
				if(t_eval.first < best_eval.first) best_eval = t_eval;
				beta = std::min(best_eval.first, beta);
				if(alpha >= beta) break;
			};
			internal::next_moves.pop_back();
			internal::cells[this_move.x][this_move.y] = 0;
			return best_eval;
		};
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
		auto list_adj_cells = algo::adjacent_cells();
		std::pair<longlong, sf::Vector2i> t_eval, best_eval;
		if(playAs == X){
			best_eval = {INT64_MIN, {0,0}};
		} else if(playAs == O){
			best_eval = {INT64_MAX, {0,0}};
		};
		for(auto& i: list_adj_cells){
			t_eval = algo::minimaxWithPruning(i, gameStats::difficulty, playAs, INT64_MIN, INT64_MAX);
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