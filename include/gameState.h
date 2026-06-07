#ifndef GAMESTATE_H
#define GAMESTATE_H

// custom includes
#include "./Flag.h"
#include "./Player.h"
#include "./Point.h"

// other includes
#include <utility>
#include <vector>
#include <string>
#include <sstream>

struct State{
    std::vector<Player> players;
    std::pair<Flag, Flag> flags;    
};

std::ostream& operator<<(std::ostream&, const State&);
std::istream& operator>>(std::istream&, State&);

std::string serializeState(const State&);
State deserializeState(const std::string&);

void initializePosition(State& state);

#endif