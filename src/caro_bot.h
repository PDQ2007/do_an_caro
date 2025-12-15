#ifndef CARO_BOT_H
#define CARO_BOT_H

#include "config.h"

void kill_bot();

void bot_playing(player playAs, player firstPlayer, std::vector<sf::Vector2i>& moves, std::vector<std::vector<short> >& cells, int depth);

#endif