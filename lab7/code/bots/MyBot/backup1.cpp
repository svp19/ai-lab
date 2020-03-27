/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <chrono>
#include <cstdlib>
#include <limits.h>
#include <fstream>
#define TIME_CONSTRAINT 1800
using namespace std;
using namespace Desdemona;

auto start = chrono::steady_clock::now();

class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread. 
         */
        MyBot( Turn turn );

        /**
         * Play something 
         */
        virtual Move play( const OthelloBoard& board );
        virtual int miniMax( OthelloBoard &board, Move move, int depth, Turn turn );
        virtual int heuristic( OthelloBoard &board );
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

Move MyBot::play( const OthelloBoard& board )
{
    start = chrono::steady_clock::now();
    list<Move> moves = board.getValidMoves( turn );
    Move bestMove = *moves.begin();
    int bestScore = INT_MIN;

    fstream fout("temp2.log", fstream::app);
    int maxDepth = 0;
    while(++maxDepth){// While 2 seconds not over || full board explored

        for(Move move: moves){

            // Minimax from origin move
            OthelloBoard newBoard = OthelloBoard(board);
            int score = miniMax(newBoard, move, maxDepth, this->turn);
            if(score > bestScore){
                bestScore = score;
                bestMove = move;
            }

            fout << move.x << ", " << move.y << "\n";

            if(score == INT_MIN){ // Time Up
                return bestMove;
            }
        }
    }
    fout.close();
    return bestMove;
}


int MyBot::miniMax( OthelloBoard &board, Move move, int depth, Turn turn )
{

    if(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() > TIME_CONSTRAINT){// Check if time up
        return INT_MIN;
    }

    OthelloBoard newBoard = OthelloBoard(board);
    newBoard.makeMove(turn, move);
    list<Move> children = newBoard.getValidMoves( turn );

    if(depth <= 0){
        return heuristic(newBoard);
    }
    
    if(turn == this->turn){ // maximizingPlayer
        int bestScore = INT_MIN;
        
        for(Move child: children){
            int score = miniMax(newBoard, child, depth-1, other(turn));
            if(score == INT_MIN)
                return INT_MIN;
            score = max(score, bestScore);
        }
        
        return bestScore;
    } 
    else // minimizingPlayer 
    {
        int bestScore = INT_MAX;
        
        for(Move child: children){
            int score = miniMax(newBoard, child, depth-1, other(turn));
            score = min(score, bestScore);
        }
        
        return bestScore;
    }
}

int MyBot::heuristic( OthelloBoard &board ) {
    return ( board.getBlackCount() - board.getRedCount() );
}
// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}


