#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include <math.h> 
#include <unordered_map>
#include <algorithm>
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
	StudentAI(int col, int row, int k, int g);
	virtual Move GetMove(Move board);

	//vector<pair<int, int> > valid;
	// keep track of current range of pieces on the board
	// when perform the search, use these +/-2 to lower valid spots
	int min_col, max_col, min_row, max_row;

	// stores the current board with 0 = empty, 1&2 for black&white
	Board my_board;
	// stores the evaluation score for each empty spot, for AI and the other player(rival)
	//unordered_map<pair<int, int>, int> AI_values;
	//unordered_map<pair<int, int>, int> rival_values;

	// update the min_row, etc
	void update_min_max_row(Move m);

	// @return: board's evaluation score
	// @usage: emulate AI's move and the following moves
	int max_move(pair<int, int> &spot, int depth);

	// @return: board's evalutation score
	// @usage: emulate component's move and the following moves
	int min_move(pair<int, int> &spot, int depth);

	// @return: evaluation score of the line
	// @usage：based on the line'e empty spots and number of same color pieces
	int score_dict(int empty, int pieces, int k);

	// @return true for break, false for continue
	// helper function for evaluate_single_space
	bool count_piece(int &empty, int &same, int turn, int i, int j);

	// @return evaluation score for the empty spot valid[i][j]
	// @usage: evaluate all eight directions for the space
	int evaluate_single_space(int i, int j, int turn);

	// @return: score for the whole board
	int evaluate_board(int turn);

	// @usage: find all valid moves in the board (within constrains)
	void find_empty(vector<pair<int, int> > &valid);



};

#endif //STUDENTAI_H