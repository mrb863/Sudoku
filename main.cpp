#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include<cstdlib>
#include <chrono>

using namespace std;

const int SIZE = 9;

bool isValid(int row, int col, char val, vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == val) {
            return false;
        }
    }
    for (int j = 0; j < 9; j++) {
        if (board[j][col] == val) {
            return false;
        }
    }
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (board[i][j] == val ) {
                return false;
            }
        }
    }
    return true;
}

bool backtracking(vector<vector<char>>& board) {
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != '_') continue;
            for (char k = '1'; k <= '9'; k++) {
                if (isValid(i, j, k, board)) {
                    board[i][j] = k;
                    if (backtracking(board)) return true;
                    board[i][j] = '_';
                }
            }
            return false;
        }
    }
    return true;
}


void solveSudoku(vector<vector<char>> &board) {
    backtracking(board);
}

void generateSudoku(vector<vector<char>>& board) {
    vector<char> first;
    for (char i = '1'; i < '1' + SIZE; i++)
        first.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(first.begin(), first.end(), std::default_random_engine(seed));
    for(int j = 0;j < SIZE;j++)
        board[0][j]=first[j];
    int shift[9]={0,3,6,1,4,7,2,5,8};
    for(int i = 1;i < SIZE;i++){
        vector<char> temp(first);
        for(int j = 0;j < shift[i];j++){
            char last = temp.back();
            temp.insert(temp.begin(), last);
            temp.erase(temp.end() - 1);
        }
        for(int j = 0;j < SIZE;j++){
            board[i][j]=temp[j];
        }

    }
}

void generateGame(vector<vector<char>>& board, int empty){
    generateSudoku(board);
    vector<int> e;
    for(int i = 0;i < SIZE*SIZE;i++)
        e.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(e.begin(),e.end(), std::default_random_engine(seed));
    for(int i = 0;i < empty;i++){
        int r = e[i] / 9;
        int c = e[i] % 9;
        board[r][c]='_';
    }
}

void printSudoku(const vector<vector<char>>& board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<char>> readSudoku(char* filePath){
    ifstream in;
    in.open(filePath,ios::in);
    if(!in.is_open()){
        std::cerr<<"cannot open the file";
        vector<vector<char>> empty;
        return empty;
    }
    vector<vector<char>> board(SIZE, vector<char>(SIZE, '_'));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            in >> board[i][j];
        }
    }
    in.close();
    return board;
}

void writeSudoku(vector<vector<char>>& board,char* filePath){
    ofstream out;
    out.open(filePath,ios::trunc);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            out << board[i][j];
        }
        out << '\n';
    }
    out.close();
}



int main(int argc, char* argv[]) {
    //setbuf(stdout, NULL);
    if(argc < 2){
        cout<<"too few arguments\n";
        return -1;
    }

    if(strncmp(argv[1],"-c",2) == 0){
        int num = atoi(argv[2]);
        if(num<=0||num>=1000000){
            cout<<"invalid number\n";
            return -1;
        }
        for (int i = 0; i < num; i++) {
            vector<vector<char>> board(SIZE, vector<char>(SIZE, '_'));
            generateSudoku(board);
            char fileName[18]="sudoku_";
            char numStr[8];
            itoa(i+1,numStr,10);
            strncpy(&fileName[7],numStr,9);
            writeSudoku(board,fileName);
        }
    }
    else if(strncmp(argv[1],"-s",2) == 0){
        auto board = readSudoku(argv[2]);
        if(board.empty())
            return -1;
        solveSudoku(board);
        printSudoku(board);
    }
    else if(strncmp(argv[1],"-n",2) == 0){
        int num = atoi(argv[2]);
        if(num<=0||num>10000){
            cout<<"invalid number\n";
            return -1;
        }
        int empty = 25;
        if(argc > 4){
            if(strncmp(argv[3],"-m",2) == 0){
                int difficulty = atoi(argv[4]);
                if(difficulty<=0||difficulty>3){
                    cout<<"invalid number\n";
                    return -1;
                }
                if(difficulty == 1)
                    empty = 20;
                else if(difficulty == 2)
                    empty = 30;
                else if(difficulty == 3)
                    empty = 40;
            }
            else if(strncmp(argv[3],"-r",2) == 0){
                int emptyNum = atoi(argv[4]);
                if(emptyNum<=0||emptyNum>3){
                    cout<<"invalid number\n";
                    return -1;
                }
                empty = emptyNum;
            }
            else{
                cout<<"invalid input\n";
                return -1;
            }
        }

        for (int i = 0; i < num; i++) {
            vector<vector<char>> board(SIZE, vector<char>(SIZE, '_'));
            generateGame(board,empty);
            char fileName[18]="sudoku_";
            char numStr[8];
            itoa(i+1,numStr,10);
            strncpy(&fileName[7],numStr,9);
            writeSudoku(board,fileName);
        }
    }
    else{
        cout<<"invalid input\n";
        return -1;
    }
    return 0;
}
