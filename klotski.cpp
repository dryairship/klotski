#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };

const int boardHeight = 5;
const int boardWidth = 4;

struct KlotskiPiece
{
     int width, height, r, c;
};

struct KlotskiBoard
{
    vector<KlotskiPiece> pieces;
    string display[boardHeight];

    bool operator < (const KlotskiBoard &board2) const
    {
        for(int i=0; i<boardHeight; ++i) {
            if (this->display[i] != board2.display[i])
                return this->display[i] < board2.display[i];
        }
        return false;
   }
};

void refreshBoard(KlotskiBoard &board) {
    for(int i=0; i<boardHeight; ++i)
        board.display[i] = string(boardWidth, '.');
    for (KlotskiPiece p : board.pieces) {
        if (p.width == 1 && p.height == 1) {
            board.display[p.r][p.c] = '0';
        } else if (p.width == 2 && p.height == 1) {
            board.display[p.r][p.c] = '-';
            board.display[p.r][p.c+1] = '-';
        } else if (p.width == 1 && p.height == 2) {
            board.display[p.r][p.c] = '|';
            board.display[p.r+1][p.c] = '|';
        } else {
            board.display[p.r][p.c] = '@';
            board.display[p.r+1][p.c] = '@';
            board.display[p.r][p.c+1] = '@';
            board.display[p.r+1][p.c+1] = '@';
        }
    }
}

bool isWinningPosition(const KlotskiBoard &board) {
    return (board.display[boardHeight-1][(boardWidth/2)-1] == '@')
        && (board.display[boardHeight-1][(boardWidth/2)] == '@')
        && (board.display[boardHeight-2][(boardWidth/2)-1] == '@')
        && (board.display[boardHeight-2][(boardWidth/2)-1] == '@');
}

bool canMove(const KlotskiBoard &board, const int pieceIndex, const Direction direction) {
    const KlotskiPiece piece = board.pieces[pieceIndex];
    switch(direction) {
        case UP:
            return (piece.r > 0)
                && (board.display[piece.r-1][piece.c] == '.')
                && (piece.width < 2 || board.display[piece.r-1][piece.c+1] == '.');
        case DOWN:
            return (piece.r < boardHeight-piece.height)
                && (board.display[piece.r+piece.height][piece.c] == '.')
                && (piece.width < 2 || board.display[piece.r+piece.height][piece.c+1] == '.');
        case LEFT:
            return (piece.c > 0)
                && (board.display[piece.r][piece.c-1] == '.')
                && (piece.height < 2 || board.display[piece.r+1][piece.c-1] == '.');
        case RIGHT:
            return (piece.c < boardWidth-piece.width)
                && (board.display[piece.r][piece.c+piece.width] == '.')
                && (piece.height < 2 || board.display[piece.r+1][piece.c+piece.width] == '.');
    }
    return false;
}

KlotskiBoard movePiece(const KlotskiBoard &board, const int pieceIndex, const Direction direction) {
    KlotskiBoard newBoard = board;
    switch(direction) {
        case UP:
            newBoard.pieces[pieceIndex].r -= 1;
            break;
        case DOWN:
            newBoard.pieces[pieceIndex].r += 1;
            break;
        case LEFT:
            newBoard.pieces[pieceIndex].c -= 1;
            break;
        case RIGHT:
            newBoard.pieces[pieceIndex].c += 1;
            break;
    }
    refreshBoard(newBoard);
    return newBoard;
}

KlotskiBoard readInitialBoard() {
    int nPieces = 0;
    KlotskiBoard board;
    for(int i=0; i<boardHeight; ++i) {
        board.display[i] = string(boardWidth, '.');
    }
    for(int i=0; i<boardHeight; ++i) {
        string row;
        cin >> row;
        for(int j=0; j<boardWidth; ++j) {
            if (row[j] != '.' && board.display[i][j] == '.') {
                if(row[j] == '0') {
                    board.pieces.push_back(KlotskiPiece{1, 1, i, j});
                    board.display[i][j] = '0';
                } else if(row[j] == '|') {
                    board.pieces.push_back(KlotskiPiece{1, 2, i, j});
                    board.display[i][j] = '|';
                    board.display[i+1][j] = '|';
                } else if(row[j] == '-') {
                    board.pieces.push_back(KlotskiPiece{2, 1, i, j});
                    board.display[i][j] = '-';
                    board.display[i][j+1] = '-';
                } else if(row[j] == '@') {
                    board.pieces.push_back(KlotskiPiece{2, 2, i, j});
                    board.display[i][j] = '@';
                    board.display[i+1][j] = '@';
                    board.display[i][j+1] = '@';
                    board.display[i+1][j+1] = '@';
                }
            }
        }
    }
    return board;
}

void printBoard (KlotskiBoard board) {
    for(int i=0; i<boardHeight; ++i) {
        for(int j=0; j<boardWidth; ++j) {
            cout << board.display[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

vector<KlotskiBoard> solveBoard(const KlotskiBoard &board) {
    map<KlotskiBoard, KlotskiBoard> parent;
    queue<pair<KlotskiBoard, int>> q;
    q.push({board, 0});
    vector<KlotskiBoard> result;
    while (!q.empty()) {
        pair<KlotskiBoard, int> boardAndDepth = q.front();
        KlotskiBoard currentBoard = boardAndDepth.first;
        int currentDepth = boardAndDepth.second;
        q.pop();
        if (isWinningPosition(currentBoard)) {
            stack<KlotskiBoard> boardStack;
            boardStack.push(currentBoard);
            for(int i=0; i<boardAndDepth.second; ++i) {
                currentBoard = parent[currentBoard];
                boardStack.push(currentBoard);
            }
            while(!boardStack.empty()) {
                result.push_back(boardStack.top());
                boardStack.pop();
            }
            break;
        } else {
            for(int i=0; i<currentBoard.pieces.size(); ++i) {
                if(canMove(currentBoard, i, UP)) {
                    KlotskiBoard newBoard = movePiece(currentBoard, i, UP);
                    if (parent.find(newBoard) == parent.end()) {
                        parent[newBoard] = currentBoard;
                        q.push({newBoard, currentDepth+1});
                    }
                }
                if(canMove(currentBoard, i, DOWN)) {
                    KlotskiBoard newBoard = movePiece(currentBoard, i, DOWN);
                    if (parent.find(newBoard) == parent.end()) {
                        parent[newBoard] = currentBoard;
                        q.push({newBoard, currentDepth+1});
                    }
                }
                if(canMove(currentBoard, i, LEFT)) {
                    KlotskiBoard newBoard = movePiece(currentBoard, i, LEFT);
                    if (parent.find(newBoard) == parent.end()) {
                        parent[newBoard] = currentBoard;
                        q.push({newBoard, currentDepth+1});
                    }
                }
                if(canMove(currentBoard, i, RIGHT)) {
                    KlotskiBoard newBoard = movePiece(currentBoard, i, RIGHT);
                    if (parent.find(newBoard) == parent.end()) {
                        parent[newBoard] = currentBoard;
                        q.push({newBoard, currentDepth+1});
                    }
                }
            }
        }
    }
    return result;
}

int main(int argc, char* argv[]){
    KlotskiBoard initialBoard = readInitialBoard();
    vector<KlotskiBoard> solution = solveBoard(initialBoard);
    for(int i=0; i<solution.size(); ++i) {
        cout << i << ":" << endl;
        printBoard(solution[i]);
    }
}
