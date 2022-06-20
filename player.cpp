#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

struct Node {
    int value;
    vector<Node*> next;
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

int calculate[15][15];

#define enemy 3 - player

int GetValue(int k, int l) {
    int SumValue = 0;   //存各方向的value總和。
    // 向左右輻射
    int cnt_curline = 0;
    int flag_left = l, flag_right = l;
    for(int m = 1; m < SIZE; m++) {
        if(l - m >= 0) {
            if(board[k][l - m] == player) {
                cnt_curline++;
            }
            else if(board[k][l - m] == 0 || board[k][l - m] == enemy) {
                flag_left = l - m + 1;
                break;
            }
        }
        if(l - m < 0) {
            flag_left = 0;
            break;
        }
    }
    for(int m = 1; m < SIZE; m++) {
        if(l + m < SIZE) {
            if(board[k][l + m] == player) {
                cnt_curline++;
            }
            else if(board[k][l + m] == 0 || board[k][l + m] == enemy) {
                flag_right = l + m - 1;
                break;
            }
        }
        if(l + m == SIZE) {
            flag_right = SIZE - 1;
            break;
        }
    }
    // 以下Sumvalue暫定NO.
    // 連五
    if(cnt_curline == 4) {
        SumValue += 1;  //必勝位置，將value設最高，並且不會被別人的死四超過
    }
    // 連四
    else if(cnt_curline == 3) {
        if(flag_left == 0) {   //左邊牆壁右邊空格
            if(board[k][flag_right + 1] == 0) {
                SumValue += 3;
            }
        }
        else if(flag_right == SIZE - 1) {   //左邊空格右邊牆壁
            if(board[k][flag_left - 1] == 0) {
                SumValue += 3;
            }
        }
        else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == enemy) {    //左邊空格右邊對手
            SumValue += 3;
        }
        else if(board[k][flag_left - 1] == enemy && board[k][flag_right + 1] == 0) {    //左邊對手右邊空格
            SumValue += 3;
        }
        else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0) {    //左邊空格右邊空格
            SumValue += 2;
        }
    }
    // 連三
    else if(cnt_curline == 2) {
        if(flag_left == 0) {
            if(board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 4.5;    // #XXX..
            }
            else if(board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 3;   // #XXX.X
            }
        }
        else if(flag_right + 1 == SIZE) {
            if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0) {
                SumValue += 4.5;    // ..XXX#
            }
            else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0) {
                SumValue += 3;   // X.XXX#
            }
        }
        else if(flag_left - 1 == 0) {
            if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 3;   // #.XXX.X
            }
            else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 3.8;    // #.XXX..
            }
            else if((board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy)) {
                SumValue += 4.5;    // #.XXX.O
            }
        }
        else if(flag_right + 2 == SIZE) {
            if((board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0)) {
                SumValue += 3;   // X.XXX.#
            }
            else if((board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0)) {
                SumValue += 3.8;    // ..XXX.#
            }
            else if((board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0)) {
                SumValue += 4.5;    // O.XXX.#
            }
        }
        else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
            SumValue += 2;   // X.XXX.X
        }
        else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
            SumValue += 2.9;   // ..XXX.X
        }
        else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
            SumValue += 2.9;   // X.XXX..
        }
        else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
            SumValue += 3.5;    // ..XXX..
        }
        else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
            SumValue += 3;   // O.XXX.X
        }
        else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
            SumValue += 3;   // X.XXX.O
        }
        else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
            SumValue += 3.8;    // O.XXX..
        }
        else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
            SumValue += 3.8;    // ..XXX.O
        }
        else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
            SumValue += 4.5;    // O.XXX.O
        }
        else if(board[k][flag_left - 1] == enemy && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
            SumValue += 4.5;    // OXXX..
        }
        else if(board[k][flag_left - 1] == enemy && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
            SumValue += 3;   // OXXX.X
        }
        else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == enemy) {
            SumValue += 4.5;    // ..XXXO
        }
        else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == enemy) {
            SumValue += 3;   // X.XXXO
        }
    }
    // 連二
    // 明天記得判斷優先度
    else if(cnt_curline == 1) {
        if(flag_left == 0) {
            if(board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 8;      // 第六種變形：#XX...
            }
            else if(board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 4.5;    // 第七種變形：#XX..X
            }
            else if(board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 4.5;    // 第八種變形：#XX.X.
            }
        }
        else if(flag_right + 1 == SIZE) {
            if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0) {
                SumValue += 8;      // 第六種變形：...XX#
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0) {
                SumValue += 4.5;    // 第七種變形：X..XX#
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0) {
                SumValue += 4.5;    // 第八種變形：.X.XX#
            }
        }
        else if(flag_left - 1 == 0) {
            if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 7;      // 第九種變形：#.XX...
            }
            else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 8;      // 第十種變形：#.XX..O
            }
            else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 4.5;    // 第十一種變形：#.XX..X
            }
            else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 4;      // 第十二種變形：#.XX.X.
            }
            else if(board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // 第十三種變形：#.XX.XO
            }
        }
        else if(flag_right + 2 == SIZE) {
            if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0) {
                SumValue += 7;      // 第九種變形：...XX.#
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0) {
                SumValue += 8;      // 第十種變形：O..XX.#
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0) {
                SumValue += 4.5;    // 第十一種變形：X..XX.#
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0) {
                SumValue += 4;      // 第十二種變形：.X.XX.#
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0) {
                SumValue += 4.5;    // 第十三種變形：OX.XX.#
            }
        }
        else if(flag_left - 2 == 0) {
            if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 6;      // 第二種變形：#..XX...
            }
            else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 4.5;    // 第五種變形：#X.XX...
            }
            else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 8;      // 第十種變形：#..XX.O
            }
            else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 4.5;    // 第十三種變形：#X.XX.O
            }
            else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 3.7;    // 第十四種變形：#..XX.X.
            }
            else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 3.3;    // 第十六種變形：#X.XX.X.
            }
            else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 6.5;    // 第十七種變形：#..XX..O
            }
            else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 4.3;    // 第十八種變形：#..XX..X
            }
            else if(board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // 第十九種變形：#..XX.XO
            }
            else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // 第十九種變形：#X.XX..O
            }
            else if(board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 3.5;    // 第二十種變形：#X.XX..X
            }
        }
        else if(flag_right + 3 == SIZE) {
            if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 6;      // 第二種變形：...XX..#
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 4.5;    // 第五種變形：...XX.X#
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 8;      // 第十種變形：O.XX..#
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 4.5;    // 第十三種變形：O.XX.X#
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 3.7;    // 第十四種變形：.X.XX..#
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 3.3;    // 第十六種變形：.X.XX..#
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 6.5;    // 第十七種變形：O..XX..#
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 4.3;    // 第十八種變形：X..XX..#
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 4.5;    // 第十九種變形：O..XX.X#
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0) {
                SumValue += 4.5;    // 第十九種變形：OX.XX..#
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player) {
                SumValue += 3.5;    // 第二十種變形：X..XX.X#
            }
        }
        else if(flag_left - 3 >= 0 && flag_right + 3 < SIZE) {
            if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 5;      // ...XX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 6;      // ...XX..O
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 6;      // O..XX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 3.9;    // ...XX..X
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 3.9;    // X..XX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 3.7;    // ...XX.X.
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 3.7;    // .X.XX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // ...XX.XO
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 4.5;    // OX.XX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 3.7;    // .X.XX..O
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 3.7;    // O..XX.X.
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 3.6;    // .X.XX..X
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 3.6;    // X..XX.X.
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 3.3;    // .X.XX.XO
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 3.3;    // OX.XX.X.
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 6.5;    // O..XX..O
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 4.3;    // O..XX..X
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 4.3;    // X..XX..O
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // O..XX.XO
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // OX.XX..O
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 3.5;    // OX.XX..X
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 3.5;    // X..XX.XO
            }
            else if(board[k][flag_left - 1] == enemy && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 8;      // 第六種變形：OXX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == enemy) {
                SumValue += 8;      // 第六種變形：...XXO
            }
            else if(board[k][flag_left - 1] == enemy && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 4.5;    // 第七種變形：OXX..X
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == enemy) {
                SumValue += 4.5;    // 第七種變形：X..XXO
            }
            else if(board[k][flag_left - 1] == enemy && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 4.5;    // 第八種變形：OXX.X.
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == enemy) {
                SumValue += 4.5;    // 第八種變形：.X.XXO
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == 0) {
                SumValue += 7;      // 第九種變形：O.XX...
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 7;      // 第九種變形：...XX.O
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == enemy) {
                SumValue += 8;      // 第十種變形：O.XX..O
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 8;      // 第十種變形：O..XX.O
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == 0 && board[k][flag_right + 3] == player) {
                SumValue += 4.5;    // 第十一種變形：O.XX..X
            }
            else if(board[k][flag_left - 3] == player && board[k][flag_left - 2] == 0 && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 4.5;    // 第十一種變形：X..XX.O
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == 0) {
                SumValue += 4;      // 第十二種變形：O.XX.X.
            }
            else if(board[k][flag_left - 3] == 0 && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 4;      // 第十二種變形：.X.XX.O
            }
            else if(board[k][flag_left - 2] == enemy && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == player && board[k][flag_right + 3] == enemy) {
                SumValue += 4.5;    // 第十三種變形：O.XX.XO
            }
            else if(board[k][flag_left - 3] == enemy && board[k][flag_left - 2] == player && board[k][flag_left - 1] == 0 && board[k][flag_right + 1] == 0 && board[k][flag_right + 2] == enemy) {
                SumValue += 4.5;    // 第十三種變形：OX.XX.O
            }
        }
    }
    //垂直方向
    cnt_curline = 0;
    int flag_up = k, flag_down = k;
    for(int m = 1; m < SIZE; m++) {
        if(k - m >= 0) {
            if(board[k - m][l] == player) {
                cnt_curline++;
            }
            else if(board[k - m][l] == 0 || board[k - m][l] == enemy) {
                flag_up = k - m + 1;
                break;
            }
        }
        if(k - m < 0) {
            flag_up = 0;
            break;
        }
    }
    for(int m = 1; m < SIZE; m++) {
        if(k + m < SIZE) {
            if(board[k + m][l] == player) {
                cnt_curline++;
            }
            else if(board[k + m][l] == 0 || board[k + m][l] == enemy) {
                flag_down = k + m - 1;
                break;
            }
        }
        if(k + m == SIZE) {
            flag_down = SIZE - 1;
            break;
        }
    }
    // 以下Sumvalue暫定NO.
    // 連五
    if(cnt_curline == 4) {
        SumValue += 1;  //必勝位置，將value設最高，並且不會被別人的死四超過
    }
    // 連四
    else if(cnt_curline == 3) {
        if(flag_up == 0) {   //左邊牆壁右邊空格
            if(board[flag_down + 1][l] == 0) {
                SumValue += 3;
            }
        }
        else if(flag_down == SIZE - 1) {   //左邊空格右邊牆壁
            if(board[flag_up - 1][l] == 0) {
                SumValue += 3;
            }
        }
        else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == enemy) {    //左邊空格右邊對手
            SumValue += 3;
        }
        else if(board[flag_up - 1][l] == enemy && board[flag_down + 1][l] == 0) {    //左邊對手右邊空格
            SumValue += 3;
        }
        else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0) {    //左邊空格右邊空格
            SumValue += 2;
        }
    }
    // 連三
    else if(cnt_curline == 2) {
        if(flag_up == 0) {
            if(board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 4.5;    // #XXX..
            }
            else if(board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 3;   // #XXX.X
            }
        }
        else if(flag_down + 1 == SIZE) {
            if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0) {
                SumValue += 4.5;    // ..XXX#
            }
            else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0) {
                SumValue += 3;   // X.XXX#
            }
        }
        else if(flag_up - 1 == 0) {
            if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 3;   // #.XXX.X
            }
            else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 3.8;    // #.XXX..
            }
            else if((board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy)) {
                SumValue += 4.5;    // #.XXX.O
            }
        }
        else if(flag_down + 2 == SIZE) {
            if((board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0)) {
                SumValue += 3;   // X.XXX.#
            }
            else if((board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0)) {
                SumValue += 3.8;    // ..XXX.#
            }
            else if((board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0)) {
                SumValue += 4.5;    // O.XXX.#
            }
        }
        else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
            SumValue += 2;   // X.XXX.X
        }
        else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
            SumValue += 2.9;   // ..XXX.X
        }
        else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
            SumValue += 2.9;   // X.XXX..
        }
        else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
            SumValue += 3.5;    // ..XXX..
        }
        else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
            SumValue += 3;   // O.XXX.X
        }
        else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
            SumValue += 3;   // X.XXX.O
        }
        else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
            SumValue += 3.8;    // O.XXX..
        }
        else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
            SumValue += 3.8;    // ..XXX.O
        }
        else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
            SumValue += 4.5;    // O.XXX.O
        }
        else if(board[flag_up - 1][l] == enemy && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
            SumValue += 4.5;    // OXXX..
        }
        else if(board[flag_up - 1][l] == enemy && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
            SumValue += 3;   // OXXX.X
        }
        else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == enemy) {
            SumValue += 4.5;    // ..XXXO
        }
        else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == enemy) {
            SumValue += 3;   // X.XXXO
        }
    }
    // 連二
    // 明天記得判斷優先度
    else if(cnt_curline == 1) {
        if(flag_up == 0) {
            if(board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 8;      // 第六種變形：#XX...
            }
            else if(board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 4.5;    // 第七種變形：#XX..X
            }
            else if(board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 4.5;    // 第八種變形：#XX.X.
            }
        }
        else if(flag_down + 1 == SIZE) {
            if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0) {
                SumValue += 8;      // 第六種變形：...XX#
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0) {
                SumValue += 4.5;    // 第七種變形：X..XX#
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0) {
                SumValue += 4.5;    // 第八種變形：.X.XX#
            }
        }
        else if(flag_up - 1 == 0) {
            if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 7;      // 第九種變形：#.XX...
            }
            else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 8;      // 第十種變形：#.XX..O
            }
            else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 4.5;    // 第十一種變形：#.XX..X
            }
            else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 4;      // 第十二種變形：#.XX.X.
            }
            else if(board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // 第十三種變形：#.XX.XO
            }
        }
        else if(flag_down + 2 == SIZE) {
            if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0) {
                SumValue += 7;      // 第九種變形：...XX.#
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0) {
                SumValue += 8;      // 第十種變形：O..XX.#
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0) {
                SumValue += 4.5;    // 第十一種變形：X..XX.#
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0) {
                SumValue += 4;      // 第十二種變形：.X.XX.#
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0) {
                SumValue += 4.5;    // 第十三種變形：OX.XX.#
            }
        }
        else if(flag_up - 2 == 0) {
            if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 6;      // 第二種變形：#..XX...
            }
            else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 4.5;    // 第五種變形：#X.XX...
            }
            else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 8;      // 第十種變形：#..XX.O
            }
            else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 4.5;    // 第十三種變形：#X.XX.O
            }
            else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 3.7;    // 第十四種變形：#..XX.X.
            }
            else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 3.3;    // 第十六種變形：#X.XX.X.
            }
            else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 6.5;    // 第十七種變形：#..XX..O
            }
            else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 4.3;    // 第十八種變形：#..XX..X
            }
            else if(board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // 第十九種變形：#..XX.XO
            }
            else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // 第十九種變形：#X.XX..O
            }
            else if(board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 3.5;    // 第二十種變形：#X.XX..X
            }
        }
        else if(flag_down + 3 == SIZE) {
            if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 6;      // 第二種變形：...XX..#
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 4.5;    // 第五種變形：...XX.X#
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 8;      // 第十種變形：O.XX..#
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 4.5;    // 第十三種變形：O.XX.X#
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 3.7;    // 第十四種變形：.X.XX..#
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 3.3;    // 第十六種變形：.X.XX..#
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 6.5;    // 第十七種變形：O..XX..#
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 4.3;    // 第十八種變形：X..XX..#
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 4.5;    // 第十九種變形：O..XX.X#
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0) {
                SumValue += 4.5;    // 第十九種變形：OX.XX..#
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player) {
                SumValue += 3.5;    // 第二十種變形：X..XX.X#
            }
        }
        else if(flag_up - 3 >= 0 && flag_down + 3 < SIZE) {
            if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 5;      // ...XX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 6;      // ...XX..O
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 6;      // O..XX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 3.9;    // ...XX..X
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 3.9;    // X..XX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 3.7;    // ...XX.X.
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 3.7;    // .X.XX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // ...XX.XO
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 4.5;    // OX.XX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 3.7;    // .X.XX..O
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 3.7;    // O..XX.X.
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 3.6;    // .X.XX..X
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 3.6;    // X..XX.X.
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 3.3;    // .X.XX.XO
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 3.3;    // OX.XX.X.
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 6.5;    // O..XX..O
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 4.3;    // O..XX..X
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 4.3;    // X..XX..O
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // O..XX.XO
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // OX.XX..O
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 3.5;    // OX.XX..X
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 3.5;    // X..XX.XO
            }
            else if(board[flag_up - 1][l] == enemy && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 8;      // 第六種變形：OXX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == enemy) {
                SumValue += 8;      // 第六種變形：...XXO
            }
            else if(board[flag_up - 1][l] == enemy && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 4.5;    // 第七種變形：OXX..X
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == enemy) {
                SumValue += 4.5;    // 第七種變形：X..XXO
            }
            else if(board[flag_up - 1][l] == enemy && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 4.5;    // 第八種變形：OXX.X.
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == enemy) {
                SumValue += 4.5;    // 第八種變形：.X.XXO
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == 0) {
                SumValue += 7;      // 第九種變形：O.XX...
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 7;      // 第九種變形：...XX.O
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == enemy) {
                SumValue += 8;      // 第十種變形：O.XX..O
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 8;      // 第十種變形：O..XX.O
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == 0 && board[flag_down + 3][l] == player) {
                SumValue += 4.5;    // 第十一種變形：O.XX..X
            }
            else if(board[flag_up - 3][l] == player && board[flag_up - 2][l] == 0 && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 4.5;    // 第十一種變形：X..XX.O
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == 0) {
                SumValue += 4;      // 第十二種變形：O.XX.X.
            }
            else if(board[flag_up - 3][l] == 0 && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 4;      // 第十二種變形：.X.XX.O
            }
            else if(board[flag_up - 2][l] == enemy && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == player && board[flag_down + 3][l] == enemy) {
                SumValue += 4.5;    // 第十三種變形：O.XX.XO
            }
            else if(board[flag_up - 3][l] == enemy && board[flag_up - 2][l] == player && board[flag_up - 1][l] == 0 && board[flag_down + 1][l] == 0 && board[flag_down + 2][l] == enemy) {
                SumValue += 4.5;    // 第十三種變形：OX.XX.O
            }
        }
    }
    // 左上右下
    cnt_curline = 0;
    int flag_leftup_x = k, flag_leftup_y = l;
    int flag_rightdown_x = k, flag_rightdown_y = l;
    for(int m = 1; m < SIZE; m++) {
        if(k - m >= 0 && l - m >= 0) {
            if(board[k - m][l - m] == player) {
                cnt_curline++;
            }
            else if(board[k - m][l - m] == 0 || board[k - m][l - m] == enemy) {
                flag_leftup_x = k - m + 1;
                flag_leftup_y = l - m + 1;
                break;
            }
        }
        else if(k - m < 0 || l - m < 0) {
            flag_leftup_x = k - m + 1;
            flag_leftup_y = l - m + 1;
            break;
        }
    }
    for(int m = 1; m < SIZE; m++) {
        if(k + m < SIZE && l + m < SIZE) {
            if(board[k + m][l + m] == player) {
                cnt_curline++;
            }
            else if(board[k + m][l + m] == 0 || board[k + m][l + m] == enemy) {
                flag_rightdown_x = k + m - 1;
                flag_rightdown_y = l + m - 1;
                break;
            }
        }
        else if(k + m == SIZE || l + m == SIZE) {
            flag_rightdown_x = k + m - 1;
            flag_rightdown_y = l + m - 1;
            break;
        }
    }
    // 以下Sumvalue暫定NO.
    // 連五
    if(cnt_curline == 4) {
        SumValue += 1;  //必勝位置，將value設最高，並且不會被別人的死四超過
    }
    // 連四
    else if(cnt_curline == 3) {
        if(flag_leftup_x - flag_leftup_y <= 10 && flag_leftup_y - flag_leftup_x <= 10) {
            if(flag_leftup_x == 0 || flag_leftup_y == 0) {   //左邊牆壁右邊空格
                if(board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {
                    SumValue += 3;
                }
            }
            else if(flag_rightdown_x == SIZE - 1 || flag_rightdown_y == SIZE - 1) {   //左邊空格右邊牆壁
                if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0) {
                    SumValue += 3;
                }
            }
            if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == enemy) {    //左邊空格右邊對手
                SumValue += 3;
            }
            else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == enemy && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {    //左邊對手右邊空格
                SumValue += 3;
            }
            else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {    //左邊空格右邊空格
                SumValue += 2;
            }
        }
    }
    // 連三
    else if(cnt_curline == 2) {
        if(flag_leftup_x - flag_leftup_y <= 10 && flag_leftup_y - flag_leftup_x <= 10) {
            if(flag_leftup_x == 0 || flag_leftup_y == 0) {
                if(board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 4.5;    // #XXX..
                }
                else if(board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 3;   // #XXX.X
                }
            }
            else if(flag_rightdown_x + 1 == SIZE || flag_rightdown_y + 1 == SIZE) {
                if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0) {
                    SumValue += 4.5;    // ..XXX#
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0) {
                    SumValue += 3;   // X.XXX#
                }
            }
            else if(flag_leftup_x - 1 == 0 || flag_leftup_y - 1 == 0) {
                if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 3;   // #.XXX.X
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 3.8;    // #.XXX..
                }
                else if((board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy)) {
                    SumValue += 4.5;    // #.XXX.O
                }
            }
            else if(flag_rightdown_x + 2 == SIZE || flag_rightdown_y + 2 == SIZE) {
                if((board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0)) {
                    SumValue += 3;   // X.XXX.#
                }
                else if((board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0)) {
                    SumValue += 3.8;    // ..XXX.#
                }
                else if((board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0)) {
                    SumValue += 4.5;    // O.XXX.#
                }
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                SumValue += 2;   // X.XXX.X
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                SumValue += 2.9;   // ..XXX.X
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                SumValue += 2.9;   // X.XXX..
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                SumValue += 3.5;    // ..XXX..
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                SumValue += 3;   // O.XXX.X
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                SumValue += 3;   // X.XXX.O
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                SumValue += 3.8;    // O.XXX..
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                SumValue += 3.8;    // ..XXX.O
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                SumValue += 4.5;    // O.XXX.O
            }
            else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == enemy && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                SumValue += 4.5;    // OXXX..
            }
            else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == enemy && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                SumValue += 3;   // OXXX.X
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == enemy) {
                SumValue += 4.5;    // ..XXXO
            }
            else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == enemy) {
                SumValue += 3;   // X.XXXO
            }
        }
    }
    // 連二
    // 明天記得判斷優先度
    else if(cnt_curline == 1) {
        if(flag_leftup_x - flag_leftup_y <= 10 && flag_leftup_y - flag_leftup_x <= 10) {
            if(flag_leftup_x == 0 || flag_leftup_y == 0) {
                if(board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 8;      // 第六種變形：#XX...
                }
                else if(board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 4.5;    // 第七種變形：#XX..X
                }
                else if(board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 4.5;    // 第八種變形：#XX.X.
                }
            }
            else if(flag_rightdown_x + 1 == SIZE || flag_rightdown_y + 1 == SIZE) {
                if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0) {
                    SumValue += 8;      // 第六種變形：...XX#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0) {
                    SumValue += 4.5;    // 第七種變形：X..XX#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0) {
                    SumValue += 4.5;    // 第八種變形：.X.XX#
                }
            }
            else if(flag_leftup_x - 1 == 0 || flag_leftup_y - 1 == 0) {
                if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 7;      // 第九種變形：#.XX...
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 8;      // 第十種變形：#.XX..O
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 4.5;    // 第十一種變形：#.XX..X
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 4;      // 第十二種變形：#.XX.X.
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：#.XX.XO
                }
            }
            else if(flag_rightdown_x + 2 == SIZE || flag_rightdown_y + 2 == SIZE) {
                if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {
                    SumValue += 7;      // 第九種變形：...XX.#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {
                    SumValue += 8;      // 第十種變形：O..XX.#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {
                    SumValue += 4.5;    // 第十一種變形：X..XX.#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {
                    SumValue += 4;      // 第十二種變形：.X.XX.#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0) {
                    SumValue += 4.5;    // 第十三種變形：OX.XX.#
                }
            }
            else if(flag_leftup_x - 2 == 0 || flag_leftup_y - 2 == 0) {
                if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 6;      // 第二種變形：#..XX...
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 4.5;    // 第五種變形：#X.XX...
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 8;      // 第十種變形：#..XX.O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：#X.XX.O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.7;    // 第十四種變形：#..XX.X.
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.3;    // 第十六種變形：#X.XX.X.
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 6.5;    // 第十七種變形：#..XX..O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 4.3;    // 第十八種變形：#..XX..X
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // 第十九種變形：#..XX.XO
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // 第十九種變形：#X.XX..O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 3.5;    // 第二十種變形：#X.XX..X
                }
            }
            else if(flag_rightdown_x + 3 == SIZE || flag_rightdown_y + 3 == SIZE) {
                if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 6;      // 第二種變形：...XX..#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 4.5;    // 第五種變形：...XX.X#
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 8;      // 第十種變形：O.XX..#
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 4.5;    // 第十三種變形：O.XX.X#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 3.7;    // 第十四種變形：.X.XX..#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 3.3;    // 第十六種變形：.X.XX..#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 6.5;    // 第十七種變形：O..XX..#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 4.3;    // 第十八種變形：X..XX..#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 4.5;    // 第十九種變形：O..XX.X#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0) {
                    SumValue += 4.5;    // 第十九種變形：OX.XX..#
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player) {
                    SumValue += 3.5;    // 第二十種變形：X..XX.X#
                }
            }
            else {
                if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 5;      // ...XX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 6;      // ...XX..O
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 6;      // O..XX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 3.9;    // ...XX..X
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.9;    // X..XX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.7;    // ...XX.X.
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.7;    // .X.XX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // ...XX.XO
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 4.5;    // OX.XX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 3.7;    // .X.XX..O
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.7;    // O..XX.X.
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 3.6;    // .X.XX..X
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.6;    // X..XX.X.
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 3.3;    // .X.XX.XO
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 3.3;    // OX.XX.X.
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 6.5;    // O..XX..O
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 4.3;    // O..XX..X
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.3;    // X..XX..O
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // O..XX.XO
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // OX.XX..O
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 3.5;    // OX.XX..X
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 3.5;    // X..XX.XO
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == enemy && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 8;      // 第六種變形：OXX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == enemy) {
                    SumValue += 8;      // 第六種變形：...XXO
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == enemy && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 4.5;    // 第七種變形：OXX..X
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == enemy) {
                    SumValue += 4.5;    // 第七種變形：X..XXO
                }
                else if(board[flag_leftup_x - 1][flag_leftup_y - 1] == enemy && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 4.5;    // 第八種變形：OXX.X.
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == enemy) {
                    SumValue += 4.5;    // 第八種變形：.X.XXO
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 7;      // 第九種變形：O.XX...
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 7;      // 第九種變形：...XX.O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 8;      // 第十種變形：O.XX..O
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 8;      // 第十種變形：O..XX.O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == 0 && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == player) {
                    SumValue += 4.5;    // 第十一種變形：O.XX..X
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == player && board[flag_leftup_x - 2][flag_leftup_y - 2] == 0 && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 4.5;    // 第十一種變形：X..XX.O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == 0) {
                    SumValue += 4;      // 第十二種變形：O.XX.X.
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == 0 && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 4;      // 第十二種變形：.X.XX.O
                }
                else if(board[flag_leftup_x - 2][flag_leftup_y - 2] == enemy && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == player && board[flag_rightdown_x + 3][flag_rightdown_y + 3] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：O.XX.XO
                }
                else if(board[flag_leftup_x - 3][flag_leftup_y - 3] == enemy && board[flag_leftup_x - 2][flag_leftup_y - 2] == player && board[flag_leftup_x - 1][flag_leftup_y - 1] == 0 && board[flag_rightdown_x + 1][flag_rightdown_y + 1] == 0 && board[flag_rightdown_x + 2][flag_rightdown_y + 2] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：OX.XX.O
                }
            }
        }
    }
    // 右上左下
    cnt_curline = 0;
    int flag_rightup_x = k, flag_rightup_y = l;
    int flag_leftdown_x = k, flag_leftdown_y = l;
    for(int m = 1; m < SIZE; m++) {
        if(k - m >= 0 && l + m < SIZE) {
            if(board[k - m][l + m] == player) {
                cnt_curline++;
            }
            else if(board[k - m][l + m] == 0 || board[k - m][l + m] == enemy) {
                flag_rightup_x = k - m + 1;
                flag_rightup_y = l + m - 1;
                break;
            }
        }
        else if(k - m < 0 || l + m >= SIZE) {
            flag_leftup_x = k - m + 1;
            flag_leftup_y = l + m - 1;
            break;
        }
    }
    for(int m = 1; m < SIZE; m++) {
        if(k + m < SIZE && l - m >= 0) {
            if(board[k + m][l - m] == player) {
                cnt_curline++;
            }
            else if(board[k + m][l - m] == 0 || board[k + m][l - m] == enemy) {
                flag_leftdown_x = k + m - 1;
                flag_leftdown_y = l - m + 1;
                break;
            }
        }
        else if(k + m == SIZE || l - m < 0) {
            flag_leftdown_x = k + m - 1;
            flag_leftdown_y = l - m + 1;
            break;
        }
    }
    // 以下Sumvalue暫定NO.
    // 連五
    if(cnt_curline == 4) {
        SumValue += 1;  //必勝位置，將value設最高，並且不會被別人的死四超過
    }
    // 連四
    else if(cnt_curline == 3) {
        if(flag_rightup_x + flag_rightup_y > 3 && flag_rightup_x + flag_rightup_y < 25) {
            if(flag_rightup_x == 0 || flag_rightup_y == SIZE - 1) {   //左邊牆壁右邊空格
                if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0) {
                    SumValue += 3;
                }
            }
            else if(flag_leftdown_x == SIZE - 1 || flag_leftdown_y == 0) {   //左邊空格右邊牆壁
                if(board[flag_rightup_x - 1][flag_rightup_y + 1] == 0) {
                    SumValue += 3;
                }
            }
            else if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == enemy) {    //左邊空格右邊對手
                SumValue += 3;
            }
            else if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0) {    //左邊對手右邊空格
                SumValue += 3;
            }
            else if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0) {    //左邊空格右邊空格
                SumValue += 2;
            }
        }
    }
    // 連三
    else if(cnt_curline == 2) {
        if(flag_rightup_x + flag_rightup_y > 3 && flag_rightup_x + flag_rightup_y < 25) {
            if(flag_rightup_x == 0 || flag_rightup_y + 1 == SIZE) {
                if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 4.5;    // #XXX..
                }
                else if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 3;   // #XXX.X
                }
            }
            else if(flag_leftdown_x + 1 == SIZE || flag_leftdown_y == 0) {
                if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0) {
                    SumValue += 4.5;    // ..XXX#
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0) {
                    SumValue += 3;   // X.XXX#
                }
            }
            else if(flag_rightup_x - 1 == 0 || flag_rightup_y + 2 == SIZE) {
                if(board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 3;   // #.XXX.X
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 3.8;    // #.XXX..
                }
                else if((board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy)) {
                    SumValue += 4.5;    // #.XXX.O
                }
            }
            else if(flag_leftdown_x + 2 == SIZE || flag_leftdown_y - 1 == 0) {
                if((board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0)) {
                    SumValue += 3;   // X.XXX.#
                }
                else if((board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0)) {
                    SumValue += 3.8;    // ..XXX.#
                }
                else if((board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0)) {
                    SumValue += 4.5;    // O.XXX.#
                }
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                SumValue += 2;   // X.XXX.X
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                SumValue += 2.9;   // ..XXX.X
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                SumValue += 2.9;   // X.XXX..
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                SumValue += 3.5;    // ..XXX..
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                SumValue += 3;   // O.XXX.X
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                SumValue += 3;   // X.XXX.O
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                SumValue += 3.8;    // O.XXX..
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                SumValue += 3.8;    // ..XXX.O
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                SumValue += 4.5;    // O.XXX.O
            }
            else if(board[flag_rightup_x - 1][flag_rightup_y + 1] == enemy && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                SumValue += 4.5;    // OXXX..
            }
            else if(board[flag_rightup_x - 1][flag_rightup_y + 1] == enemy && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                SumValue += 3;   // OXXX.X
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == enemy) {
                SumValue += 4.5;    // ..XXXO
            }
            else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == enemy) {
                SumValue += 3;   // X.XXXO
            }
        }
    }
    // 連二
    // 明天記得判斷優先度
    else if(cnt_curline == 1) {
        if(flag_rightup_x + flag_rightup_y > 3 && flag_rightup_x + flag_rightup_y < 25) {
            if(flag_rightup_x == 0 || flag_rightup_y + 1 == SIZE) {
                if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 8;      // 第六種變形：#XX...
                }
                else if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 4.5;    // 第七種變形：#XX..X
                }
                else if(board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 4.5;    // 第八種變形：#XX.X.
                }
            }
            else if(flag_leftdown_x + 1 == SIZE || flag_leftdown_y == 0) {
                if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1] == 0) {
                    SumValue += 8;      // 第六種變形：...XX#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0) {
                    SumValue += 4.5;    // 第七種變形：X..XX#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0) {
                    SumValue += 4.5;    // 第八種變形：.X.XX#
                }
            }
            else if(flag_rightup_x - 1 == 0 || flag_rightup_y + 2 == SIZE) {
                if(board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 7;      // 第九種變形：#.XX...
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 8;      // 第十種變形：#.XX..O
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 4.5;    // 第十一種變形：#.XX..X
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 4;      // 第十二種變形：#.XX.X.
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：#.XX.XO
                }
            }
            else if(flag_leftdown_x + 2 == SIZE || flag_leftdown_y - 1 == 0) {
                if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0) {
                    SumValue += 7;      // 第九種變形：...XX.#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0) {
                    SumValue += 8;      // 第十種變形：O..XX.#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0) {
                    SumValue += 4.5;    // 第十一種變形：X..XX.#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0) {
                    SumValue += 4;      // 第十二種變形：.X.XX.#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0) {
                    SumValue += 4.5;    // 第十三種變形：OX.XX.#
                }
            }
            else if(flag_rightup_x - 2 == 0 || flag_rightup_y + 3 == SIZE) {
                if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 6;      // 第二種變形：#..XX...
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 4.5;    // 第五種變形：#X.XX...
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 8;      // 第十種變形：#..XX.O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：#X.XX.O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.7;    // 第十四種變形：#..XX.X.
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.3;    // 第十六種變形：#X.XX.X.
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 6.5;    // 第十七種變形：#..XX..O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 4.3;    // 第十八種變形：#..XX..X
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // 第十九種變形：#..XX.XO
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // 第十九種變形：#X.XX..O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 3.5;    // 第二十種變形：#X.XX..X
                }
            }
            else if(flag_leftdown_x + 3 == SIZE || flag_leftdown_y - 2 == 0) {
                if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 6;      // 第二種變形：...XX..#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 4.5;    // 第五種變形：...XX.X#
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 8;      // 第十種變形：O.XX..#
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 4.5;    // 第十三種變形：O.XX.X#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 3.7;    // 第十四種變形：.X.XX..#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 3.3;    // 第十六種變形：.X.XX..#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 6.5;    // 第十七種變形：O..XX..#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 4.3;    // 第十八種變形：X..XX..#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 4.5;    // 第十九種變形：O..XX.X#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0) {
                    SumValue += 4.5;    // 第十九種變形：OX.XX..#
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player) {
                    SumValue += 3.5;    // 第二十種變形：X..XX.X#
                }
            }
            else {
                if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 5;      // ...XX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 6;      // ...XX..O
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 6;      // O..XX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 3.9;    // ...XX..X
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.9;    // X..XX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.7;    // ...XX.X.
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.7;    // .X.XX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // ...XX.XO
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 4.5;    // OX.XX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 3.7;    // .X.XX..O
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.7;    // O..XX.X.
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 3.6;    // .X.XX..X
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.6;    // X..XX.X.
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 3.3;    // .X.XX.XO
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 3.3;    // OX.XX.X.
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 6.5;    // O..XX..O
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 4.3;    // O..XX..X
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.3;    // X..XX..O
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // O..XX.XO
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // OX.XX..O
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 3.5;    // OX.XX..X
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 3.5;    // X..XX.XO
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== enemy && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 8;      // 第六種變形：OXX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == enemy) {
                    SumValue += 8;      // 第六種變形：...XXO
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== enemy && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 4.5;    // 第七種變形：OXX..X
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == enemy) {
                    SumValue += 4.5;    // 第七種變形：X..XXO
                }
                else if(board[flag_rightup_x - 1][flag_rightup_y + 1]== enemy && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 4.5;    // 第八種變形：OXX.X.
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == enemy) {
                    SumValue += 4.5;    // 第八種變形：.X.XXO
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 7;      // 第九種變形：O.XX...
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 7;      // 第九種變形：...XX.O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 8;      // 第十種變形：O.XX..O
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 8;      // 第十種變形：O..XX.O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == 0 && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == player) {
                    SumValue += 4.5;    // 第十一種變形：O.XX..X
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == player && board[flag_rightup_x - 2][flag_rightup_y + 2] == 0 && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 4.5;    // 第十一種變形：X..XX.O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == 0) {
                    SumValue += 4;      // 第十二種變形：O.XX.X.
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == 0 && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 4;      // 第十二種變形：.X.XX.O
                }
                else if(board[flag_rightup_x - 2][flag_rightup_y + 2] == enemy && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == player && board[flag_leftdown_x + 3][flag_leftdown_y - 3] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：O.XX.XO
                }
                else if(board[flag_rightup_x - 3][flag_rightup_y + 3] == enemy && board[flag_rightup_x - 2][flag_rightup_y + 2] == player && board[flag_rightup_x - 1][flag_rightup_y + 1]== 0 && board[flag_leftdown_x + 1][flag_leftdown_y - 1] == 0 && board[flag_leftdown_x + 2][flag_leftdown_y - 2] == enemy) {
                    SumValue += 4.5;    // 第十三種變形：OX.XX.O
                }
            }
        }
    }
    return SumValue;
}

int minimax(Node *node, int depth, bool now_player) {
    if(depth == 0 || node->next.empty()) {
        return node->value;
    }
    if(now_player) {
        int cur_value = INT_MIN;
        for(auto child : node->next) {
            cur_value = max(cur_value, minimax(child, depth - 1, false));
        }
        return cur_value;
    }
    else {
        int cur_value = INT_MAX;
        for(auto child : node->next) {
            cur_value = min(cur_value, minimax(child, depth - 1, true));
        }
    }
}

int alphabeta(Node *node, int depth, int Alpha, int Beta, bool now_player) {
    if(depth == 0 || node->next.empty()) {
        return node->value;
    }
    if(now_player) {
        int cur_value = INT_MIN;
        for(auto child : node->next) {
            cur_value = max(cur_value, alphabeta(child, depth - 1, Alpha, Beta, false));
            Alpha = max(Alpha, cur_value);
            if(Alpha >= Beta) {
                break;
            }
        }
        return cur_value;
    }
    else {
        int cur_value = INT_MAX;
        for(auto child : node->next) {
            cur_value = min(cur_value, alphabeta(child, depth - 1, Alpha, Beta, true));
            Beta = min(Beta, cur_value);
            if(Beta <= Alpha) {
                break;
            }
        }
        return cur_value;
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
