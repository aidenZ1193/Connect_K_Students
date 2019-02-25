#include "StudentAI.h"
#include <stdlib.h>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int k,int g)
	:AI(col, row, k, g)
{
	my_board = Board(col, row, k, g);
	min_col = col/2-2;
	max_col = col/2+2;
	min_row = row/2-2;
	max_row = row/2+2;
}

void StudentAI::update_min_max_row(Move m){
	if(m.col < min_col+2)
		min_col = (m.col - 2 > 0)? m.col - 2 : 0;
	if(m.col > max_col-2)
		max_col = (m.col + 2 < my_board.col)? m.col + 2 : my_board.col;
	if(m.row < min_row+2)
		min_row = (m.row - 2 > 0)? m.row - 2 : 0;
	if(m.row > max_row-2)
		max_row = (m.row + 2 < my_board.row)? m.row + 2 : my_board.row;
}

// oponent: 2
// AI: 1
// this function works as the first round's max
Move StudentAI::GetMove(Move board)
{
	
	Move m;
	//cout<<"min col = "<<min_col<<endl;

	// 1. eveluate board, put move in board
	if(board.col == -1 && board.row == -1){
		// start first: return the spot in the middle
		update_min_max_row(board);
		my_board.board[my_board.row/2][my_board.col/2] = 1;
		if(my_board.g == 0){
			m.row = my_board.row/2;
			m.col = my_board.col/2;
		}else{
			m.row = my_board.row-1;
			m.col = my_board.col/2;
		}
		my_board.board[m.row][m.col] = 1;
		// update min, max values
		update_min_max_row(m);
		return m;
	}
	if(my_board.g == 0){
		update_min_max_row(board);
		my_board.board[board.row][board.col] = 2;
	}
	else{
		// gravity mode is on, put the move to the bottom of board
		for(int i = my_board.row-1; i >= 0; i--){
			if(my_board.board[i][board.col] == 0){
				update_min_max_row(Move(i,board.col));
				my_board.board[i][board.col] = 2;
				break;
			}
		}
	}

	// 2. generate all points
	// stores all valid spots
	vector<pair<int, int> > valid;
	// stores all usable spots
	vector<pair<int, int> > usable;
	find_empty(valid);
	int len = valid.size();
	int max_score = INT_MIN;
	int depth = 1;	// max - min - max- min - max

	// for alpha-beta pruning
	int alpha = INT_MIN, beta = INT_MAX;

	//cout<<"Before entering first round of min: "<<endl;
	for(int i = 0; i < len; i++){
		// change color in the spot to 1, which is the AI's move
		my_board.board[valid[i].first][valid[i].second] = 1;

		// perform min-max in here for each possible move
		int temp = min_move(valid[i], depth-1, alpha, beta);
		// check the score for valid[i]
		if(temp > max_score ){
			usable.clear();
			usable.push_back(valid[i]);
			max_score = temp;
		}else if(temp == max_score)
			usable.push_back(valid[i]);

		// change it back
		my_board.board[valid[i].first][valid[i].second] = 0;
	}
	int i = 0;
	if(usable.size() > 1){
		srand(time(0));
		i = rand()% usable.size();
	}
	m.row = usable[i].first;
	m.col = usable[i].second;

	// debug
	//cout<<"the list of usable in get_move:"<<endl;
	//for(int i = 0; i < usable.size(); i++)
	//	cout<<"i = "<<i<<" usable is "<<usable[i].first<<" "<<usable[i].second<<endl;


	my_board.board[m.row][m.col] = 1;
	//my_board.ShowBoard();
	update_min_max_row(m);
	
	return m;

}

// @return: board's evalutation score
// @usage: emulate component's move and the following moves
int StudentAI::min_move(pair<int, int> &spot, int depth, int &alpha, int &beta){
	//int score = evaluate_board(1);
	//int score = evaluate_single_space(spot.first, spot.second, 1);
	int score = evaluate_both(spot.first, spot.second);
	int win = my_board.IsWin();
	if(win != 0 || depth == 0){
		//cout<<"spot "<<spot.first<<" "<<spot.second<<"score from min: "<<score<<endl;
		return score;
	}

	vector<pair<int, int> > valid;
	vector<pair<int, int> > usable;
	find_empty(valid);

	//cout<<"Before entering first round of max: "<<endl;

	int len = valid.size();
	int min_score = INT_MAX;
	for(int i = 0; i < len; i++){
		my_board.board[valid[i].first][valid[i].second] = 2;
		int temp = max_move(valid[i], depth-1, alpha, beta);
		if(temp < min_score){
			usable.clear();
			usable.push_back(valid[i]);
			min_score = temp;
		}else if(temp == min_score)
			usable.push_back(valid[i]);

		my_board.board[valid[i].first][valid[i].second] = 0;

		/*
		if(min_score < beta){
			beta = min_score;
			break;
		}
		*/
	}
	return min_score;
}


// @return: board's evaluation score
// @usage: emulate AI's move and the following moves
int StudentAI::max_move(pair<int, int> &spot, int depth, int &alpha, int &beta){

	// check for end game
	//int score = evaluate_board(2);	// i = AI's turn
	int score = evaluate_both(spot.first, spot.second);
	int win = my_board.IsWin();
	// tie/someone wins/reached max depth
	if(win != 0 || depth == 0){
		return score;
	}

	vector<pair<int, int> > valid;
	vector<pair<int, int> > usable;
	find_empty(valid);

	//cout<<"Before entering second round of min: "<<endl;

	int len = valid.size();
	int max_score = INT_MIN;
	// from all points in valid[][], perform min-max search with depth
	for(int i = 0; i < len; i++){
		my_board.board[valid[i].first][valid[i].second] = 1;
		int temp = min_move(valid[i], depth, alpha, beta);
		if(temp > max_score){
			usable.clear();
			usable.push_back(valid[i]);
			max_score = temp;
		}else if(temp == max_score)
			usable.push_back(valid[i]);
		
		my_board.board[valid[i].first][valid[i].second] = 0;	
		
		/*
		if(max_score > alpha){
			alpha = max_score;
			break;
		}
		*/
	
	}
	return max_score;
}

// @return: evaluation score of the line
// @usage：based on the line'e empty spots and number of same color pieces
int StudentAI::score_dict(int empty, int pieces, int k){
	if(pieces >= k-1) return pow(10, k+5);
	//if(pieces >= k-2) return pow(10, k+3);
	if(pieces == 0) return 0;
	k--;
	while(k > 1){
		if(pieces == k){
			if(empty >= 2) return pow(10, k+1);	// was empty >= 2
			else if(empty == 1) return pow(10, k);
		} 
		k--;
	}
	return 0;
}

// check for non-empty neighbors, return false if all empty
bool StudentAI::non_empty_neighbor(int i, int j){
	if(i > 0){
		if(my_board.board[i-1][j] != 0) return true;
		if(j > 0){
			if(my_board.board[i-1][j-1] != 0) return true;
			if(my_board.board[i][j-1] != 0) return true;
		}
		if(j < my_board.col-1){
			 if(my_board.board[i-1][j+1] != 0) return true;
			 if(my_board.board[i][j+1] != 0) return true;
		}
	}
	if(i < my_board.row-1){
		if(my_board.board[i+1][j] != 0) return true;
		if(j > 0){
			if(my_board.board[i+1][j-1] != 0) return true;
			if(my_board.board[i][j-1] != 0) return true;
		}
		if(j < my_board.col-1){
			if(my_board.board[i+1][j+1] != 0) return true;
			if(my_board.board[i][j+1] != 0) return true;
		}

	}
	return false;
}

// @usage: find all valid moves in the board (within constrains)
void StudentAI::find_empty(vector<pair<int, int> > &valid){
	// disable gravity
	//cout<<"min_row is "<<min_row<<" and min_col is "<<min_col<<endl;
	//cout<<"max_row is "<<max_row<<" and max_col is "<<max_col<<endl;
	if(my_board.g == 0){
		for(int i = min_row; i < max_row; i++){
			for(int j = min_col; j < max_col; j++ ){
				if(my_board.board[i][j] == 0) {
					// optimize: omly count the ones next to non-empty spot
					// horizontal
					if(non_empty_neighbor(i, j))
						valid.push_back(make_pair(i, j));
				}
			}
		}
	}else{
		// enabled gravity, pick from lowest in each column
		
		for(int j = 0; j < max_col; j++){
			for(int i = my_board.row - 1; i >= 0; i--){
				//cout<<"checkinging i, j = "<<i<<" "<<j<<" is"<<my_board.board[i][j]<<endl;
				if(my_board.board[i][j] == 0 && non_empty_neighbor(i,j)){
					valid.push_back(make_pair(i,j));
					//cout<<"pushing i, j = "<<i<<" "<<j<<" in valid."<<endl;
					break;
				}
			}
		}
	}
	// debug
		//cout<<"empty spot: "<<endl;
		//for(int i = 0; i < valid.size(); i++)
		//	cout<<"i = "<<i<<", "<<valid[i].first<<", "<<valid[i].second<<endl;
}

// @return true for break, false for continue
// helper function for evaluate_single_space
bool StudentAI::count_piece(int &empty, int &same, int turn, int i, int j){
	//bool res = false;
	if(my_board.board[i][j] == 0){
		empty++;
		return true;
	}else if(my_board.board[i][j] == turn)
		same++;
	else return true;
	return false;
}

// @return evaluation score for the empty spot valid[i][j]
// @usage: evaluate all four directions for the space
// need to go through all eight directions, stop when seeing another color's piece
int StudentAI::evaluate_single_space(int i, int j, int turn){

	// evaluation score for the single space
	int sum = 0; 	
	// empty spots and pieces with same color
	int empty = 0, same = 0;

	// 1. horizontal, left and right
	for(int y = j+1; y < max_col; y++){
		if(count_piece(empty, same, turn, i, y))
			break;
	}
	for(int y = j-1; y >= min_col; y--){
		if(count_piece(empty, same, turn, i, y))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	
	//cout<<"1. "<<"sum = "<<sum<<" empty = "<<empty<<" pieces = "<<same<<endl;
	empty = same = 0;

	// 2. vertical, up and down
	for(int x = i+1; x < max_row; x++){
		if(count_piece(empty, same, turn, x, j))
			break;
	}
	for(int x = i-1; x >= min_row; x--){
		if(count_piece(empty, same, turn, x, j))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	//cout<<"2. "<<"sum = "<<sum<<" empty = "<<empty<<" pieces = "<<same<<endl;
	empty = same = 0;

	// 3. 45 degree left to right, bottom to top
	for(int x = i-1, y = j+1; x >= min_row && y < max_col; x--, y++){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	for(int x = i+1, y = j-1; x < max_row && y >= min_col; x++, y--){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	//cout<<"3. "<<"sum = "<<sum<<" empty = "<<empty<<" pieces = "<<same<<endl;
	empty = same = 0;

	// 4. 135 degree left to right, top to bottom
	for(int x = i-1, y = j-1; x >= min_row && y >= min_col; x--, y--){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	for(int x = i+1, y = j+1; x < max_row && y < max_col; x++, y++){
		if(count_piece(empty, same, turn, x, y))
			break;
	}
	sum += score_dict(empty, same, my_board.k);
	//cout<<"4. "<<"sum = "<<sum<<" empty = "<<empty<<" pieces = "<<same<<endl;

	
	// if the spot is on the wall, lower the sum level by 10
	if(i == 0) sum = sum/10;
	if(j == 0) sum = sum/10;
	if(i == my_board.row-1) sum = sum/10;
	if(j == my_board.col-1) sum = sum/10;
	

	//cout<<"i & j = "<<i<<" "<<j<<" sum = "<<sum<<endl;
	return sum;
}

// @return: score for the whole board
int StudentAI::evaluate_board(int turn){
	int sum = 0;

	for(int i = min_row; i < max_row; i++){
		for(int j = min_col; j < max_col; j++){
			sum += evaluate_single_space(i, j, turn);
		}
	}
	return sum;
}

// @return: score of ai - opponent
int StudentAI::evaluate_both(int i, int j){
	int ai = 0;
	int opponent = 0;
	ai = evaluate_single_space(i, j, 1);
	opponent = evaluate_single_space(i, j, 2);
	//ai = evaluate_board(1);
	//opponent = evaluate_board(2);

	//cout<<"i = "<<i<<", j = "<<j<<" ai score = "<<ai<<" opponent score = "<<opponent<<endl;
	return ai + opponent;
	// before furture adjusting: this version beats poorAI in no gravity modes mostly.
	// current condition: mostly wins or ties! Need furthur testing
}