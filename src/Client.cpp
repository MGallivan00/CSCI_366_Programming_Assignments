//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"
#include "string.h"
#include "cereal/archives/json.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size) {
    this->board_size = board_size;
    this->player = player;
    string file_name = "player_" + to_string(player) + ".action_board.json";

    vector<int> a(board_size, 0);
    vector<vector<int>> board(board_size, a);

    ofstream a_b(file_name);
    cereal::JSONOutputArchive outa(a_b);

    outa(CEREAL_NVP(board));
    a_b.flush();

    initialized = true;

}


void Client::fire(unsigned int x, unsigned int y) {
    string fn = "player_" + to_string(player) + ".shot.json";

    ofstream shot_file(fn);
    cereal::JSONOutputArchive outa(shot_file);

    outa(CEREAL_NVP(x), CEREAL_NVP(y));
    shot_file.flush();
}


bool Client::result_available() {
    //if file exists
    string fn = "player_" + to_string(player) + ".result.json";
    ifstream result;
    result.open(fn); //opens appropriate board

    //checks for valid file name
    return result.good();
}


int Client::get_result() {
    //get the result then remove the file
    string fn = "player_" + to_string(player) + ".result.json";
    ifstream result(fn);

    int r;
    cereal::JSONInputArchive ina(result);
    ina(r);
    result.close();

    if (r == HIT || r == MISS || r == OUT_OF_BOUNDS) {
        std::remove("player_1.result.json");
        std::remove("player_2.result.json");
        return r;
    } else {
        cout << "bad result" << endl;
        throw 0;
    }
}


void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    string file_name = "player_" + to_string(player) + ".action_board.json";

    fstream a_b(file_name);
    cereal::JSONInputArchive ina(a_b);
    cereal::JSONOutputArchive outa(a_b);

    vector<int> a(board_size, 0);
    vector<vector<int>> updated_board(board_size, a);

    ina(updated_board);

    updated_board[x][y] = result;
    cout << updated_board[x][y] << endl;
    //render_action_board();

    outa(cereal::make_nvp("board", updated_board));
    a_b.flush();

}


string Client::render_action_board() {
    string file_name = "player_" + to_string(player) + ".action_board.json";
    string rendered;

    fstream a_b(file_name);
    cereal::JSONInputArchive ina(a_b);

    vector<int> a(board_size, 0);
    vector<vector<int>> updated_board(board_size, a);

    ina(updated_board);

    for(int i=0; i < board_size; i++){
        for(int j=0; j < board_size; j++){
            rendered = updated_board[i][j] + " ";
        }
    }

    return rendered;
}
