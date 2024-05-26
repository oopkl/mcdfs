#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <tuple>
using namespace std;

// Define the State struct
struct State {
    int missionariesLeft;
    int cannibalsLeft;
    int missionariesRight;
    int cannibalsRight;
    int boat; // 0 for left side, 1 for right side

    // Overload < operator for using State in set
    bool operator<(const State& other) const {
        return tie(missionariesLeft, cannibalsLeft, missionariesRight, cannibalsRight, boat) <
               tie(other.missionariesLeft, other.cannibalsLeft, other.missionariesRight, other.cannibalsRight, other.boat);
    }
};

void print_state(const State &state) {
    cout << "Left: " << state.missionariesLeft << "M " << state.cannibalsLeft << "C"
         << "\tRight: " << state.missionariesRight << "M " << state.cannibalsRight << "C"
         << "\tBoat: " << (state.boat == 0 ? "Left" : "Right") << "\n";
}

void move(State &state, int missionaries, int cannibals) {
    if (state.boat == 0) {
        state.missionariesLeft -= missionaries;
        state.cannibalsLeft -= cannibals;
        state.missionariesRight += missionaries;
        state.cannibalsRight += cannibals;
    } else {
        state.missionariesRight -= missionaries;
        state.cannibalsRight -= cannibals;
        state.missionariesLeft += missionaries;
        state.cannibalsLeft += cannibals;
    }
    state.boat = 1 - state.boat; // Flip the boat side
}

void undo_move(State &state, int missionaries, int cannibals) {
    state.boat = 1 - state.boat; // Flip the boat side back
    if (state.boat == 0) {
        state.missionariesLeft += missionaries;
        state.cannibalsLeft += cannibals;
        state.missionariesRight -= missionaries;
        state.cannibalsRight -= cannibals;
    } else {
        state.missionariesRight += missionaries;
        state.cannibalsRight += cannibals;
        state.missionariesLeft -= missionaries;
        state.cannibalsLeft -= cannibals;
    }
}

bool is_valid(const State &state) {
    if (state.missionariesLeft < 0 || state.missionariesRight < 0 ||
        state.cannibalsLeft < 0 || state.cannibalsRight < 0)
        return false;
    if (state.missionariesLeft > 0 && state.missionariesLeft < state.cannibalsLeft)
        return false;
    if (state.missionariesRight > 0 && state.missionariesRight < state.cannibalsRight)
        return false;
    return true;
}

bool is_goal(const State &state) {
    return state.missionariesLeft == 0 && state.cannibalsLeft == 0 &&
           state.missionariesRight == 3 && state.cannibalsRight == 3;
}

bool dfs(State &state, vector<State> &path, set<State> &visited) {
    if (!is_valid(state) || visited.find(state) != visited.end())
        return false;

    path.push_back(state);
    visited.insert(state);

    if (is_goal(state))
        return true;

    for (int m = 0; m <= 2; ++m) {
        for (int c = 0; c <= 2; ++c) {
            if (m + c >= 1 && m + c <= 2) {
                move(state, m, c);
                if (dfs(state, path, visited)) {
                    return true;
                }
                undo_move(state, m, c);
            }
        }
    }

    path.pop_back();
    visited.erase(state);
    return false;
}

int main() {
    State initial_state = {3, 3, 0, 0, 0};
    vector<State> path;
    set<State> visited;

    if (dfs(initial_state, path, visited)) {
        for (const auto &state : path) {
            print_state(state);
        }
    } 
    else {
        cout << "No solution found\n";
    }

    return 0;
}
