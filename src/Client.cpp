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
#include "cereal/archives/json.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size) {
    //initializes fields
    this->board_size = board_size;
    this->player = player;
    string file_name = "player_" + to_string(player) + ".action_board.json";

    //2D array to hold action board
    vector<int> a(board_size, 0);
    vector<vector<int>> board(board_size, a);

    //fills the action board as blank
    ofstream a_b(file_name);
    cereal::JSONOutputArchive outa(a_b);

    outa(CEREAL_NVP(board));
    a_b.flush();

    initialized = true;

}


void Client::fire(unsigned int x, unsigned int y) {
    string fn = "player_" + to_string(player) + ".shot.json";

    //read out the coordinates
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
    string fn = "player_" + to_string(player) + ".result.json";
    ifstream result(fn);

    //read in results
    int r;
    cereal::JSONInputArchive ina(result);
    ina(r);
    result.close();

    //remove the files
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

    //2D vector array to manipulate and read back in
    vector<int> a(board_size, 0);
    vector<vector<int>> updated_board(board_size, a);

    //read in
    ifstream a_b(file_name);
    cereal::JSONInputArchive ina(a_b);
    ina(updated_board);
    a_b.close();

    render_action_board();
    cout << endl;
    cout << endl;

    //set the coordinates to result
    updated_board[x][y] = result;

    //read out
    ofstream b_a(file_name);
    cereal::JSONOutputArchive outa(b_a);
    outa(cereal::make_nvp("board", updated_board));
    b_a.write("\n}", 2);
    b_a.close();

    render_action_board();
}


string Client::render_action_board() {
    string file_name = "player_" + to_string(player) + ".action_board.json";

    fstream a_b(file_name);
    cereal::JSONInputArchive ina(a_b);

    //new 2D vector array to hold the action board info
    vector<int> a(board_size, 0);
    vector<vector<int>> updated_board(board_size, a);

    ina(updated_board);
    a_b.close();

    //loop through the action board and set each value to the string rendered
    string rendered;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            rendered +=
                    (updated_board[i][j] ==  1) ? " 1 " :
                    (updated_board[i][j] == -1) ? "-1 " :
                                                  " . " ;
        }
        rendered += "\n";
    }
    cout << rendered;
    return rendered;
}