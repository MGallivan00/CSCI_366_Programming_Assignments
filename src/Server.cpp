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
#include "Server.hpp"
#include "cereal/archives/json.hpp"
#include <algorithm>

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */

int get_file_length(ifstream *file) {
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board) {

    string line;
    string setup[2] = {p1_setup_board, p2_setup_board};
    this->board_size = board_size;

    players[0] = new char[board_size * board_size];
    players[1] = new char[board_size * board_size];

    //make method and use for players[1]
    for (int i = 0; i < 2; i++) {
        int rows = 0;
        ifstream board;
        board.open(setup[i]); //opens appropriate board

        //checks for valid file name
        if (!board.good()) {
            cout << "bad file name" << endl;
            throw 0;
        }

        while (getline(board, line) && !line.empty()) { //gets line

            //removes white spaces
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

            //loops through each character of the line and assigns it to players[i]
            for (int j = 0; j < line.length(); j++) {
                players[i][(rows * board_size) + j] = line[j];
            }

            //will not catch correct board size with spaces at the end
            //so we remove spaces
            if (line.length() != board_size) {
                cout << "wrong board size" << endl;
                throw 0;
            }

            rows++;
            if (rows > board_size) {
                cout << "wrong board size (row)" << endl;
                throw 0;
            }
        }
        if (rows < board_size) {
            cout << "wrong board size (row)" << endl;
            throw 0;
        }
    }

}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    //catches bad player number
    if (player < 1 || player > MAX_PLAYERS) {
        cout << "Bad player number" << endl;
        throw 0;
    }

    char *board = players[3 - player - 1]; //make it the other player's setup board 3-2=1 or 3-1=2
    char shot;
    int result = MISS;

    //out of bounds
    if (x < 0 || x >= board_size || y < 0 || y >= board_size) {
        result = OUT_OF_BOUNDS;
    } else {
        //assigns the shot to a variable
        shot = board[(y * board_size) + x];

        //detects if it is a hit or miss
        if (shot == 'C' || shot == 'D' || shot == 'R' || shot == 'B' || shot == 'S') {
            result = HIT;
        } else {
            result = MISS;
        }
    }

    return result;
}


int Server::process_shot(unsigned int player) {
    int result;

    if (player < 0 || player > 2) {
        throw 0;
    }

    string get_coor = "player_" + to_string(player) + ".shot.json";

    //reads in the coordinates
    int x, y;
    ifstream coordinates(get_coor);
    cereal::JSONInputArchive ina(coordinates);
    ina(x, y);

    //removes unnecessary files so shot can be processed again
    std::remove("player_1.shot.json");
    std::remove("player_2.shot.json");

    result = evaluate_shot((player), x, y); //set x and y

    //writes out the result
    string fn = "player_" + to_string(player) + ".result.json";
    ofstream shot_file(fn);
    cereal::JSONOutputArchive outa(shot_file);

    outa(CEREAL_NVP(result));
    shot_file.write("\n}", 2);
    shot_file.close();

    return SHOT_FILE_PROCESSED;
}
