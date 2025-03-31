#include <iostream>
#include <vector>
#include <string>
#include<chrono>

using namespace std;

struct ColumnNode;

struct Node {
	Node* left, * right, * up, * down;
	int rowId;
	ColumnNode* column;

	Node() : left(this), right(this), up(this), down(this), rowId(-1), column(nullptr) {}
};

//struct ColumnNode : public Node {
//	int size;
//	string name;
//
//	ColumnNode(string n) : size(0), name(n) {
//		column = this;
//	}
//};

struct ColumnNode : public Node {
	int size;
	//string name;

	ColumnNode() : size(0) {
		column = this;
	}
};

class DLX {
private:
	vector<ColumnNode*> columns;
	ColumnNode* root;

public:

	DLX(int numCols) {
		//root = new ColumnNode("Root");
		root = new ColumnNode();
		root->left = root;
		root->right = root;

		columns.resize(numCols, nullptr);

		for (int i = 0; i < numCols; ++i) {
			//columns[i] = new ColumnNode(to_string(i));
			columns[i] = new ColumnNode();
			columns[i]->right = root;
			columns[i]->left = root->left;
			root->left->right = columns[i];
			root->left = columns[i];
		}
	}

	~DLX() {
		Node* current = root->right;
		while (current != root) {
			ColumnNode* col = static_cast<ColumnNode*>(current);
			Node* rowNode = col->down;
			while (rowNode != col) {
				Node* temp = rowNode;
				rowNode = rowNode->down;
				delete temp;
			}
			Node* temp = current;
			current = current->right;
			delete temp;
		}

		delete root;
	}

	void addRow(int rowID, vector<int> cols) {
		Node* first = nullptr;
		for (int col : cols) {
			Node *newNode = new Node();
			newNode->rowId = rowID;
			newNode->column = columns[col];

			newNode->down = columns[col];
			newNode->up = columns[col]->up;
			columns[col]->up->down = newNode;
			columns[col]->up = newNode;
			columns[col]->size++;

			if (!first) {
				first = newNode;
			}
			else {
				newNode->left = first->left;
				newNode->right = first;
				first->left->right = newNode;
				first->left = newNode;
			}
		}
	}

	void cover(ColumnNode* col) {
		col->right->left = col->left;
		col->left->right = col->right;

		for (Node* row = col->down; row != col; row = row->down) {
			for (Node* node = row->right; node != row; node = node->right) {
				node->down->up = node->up;
				node->up->down = node->down;
				node->column->size--;

			}
		}
	}

	void uncover(ColumnNode* col) {
		for (Node* row = col->up; row != col; row = row->up) {
			for (Node* node = row->left; node != row; node = node->left) {
				node->column->size++;
				node->down->up = node;
				node->up->down = node;
			}
		}

		col->right->left = col;
		col->left->right = col;
	}

	bool solve(vector<int>& solution) {
		if (root->right == root) {
			return true;
		}

		ColumnNode* col = static_cast<ColumnNode*>(root->right);
		
		for (ColumnNode* c = static_cast<ColumnNode*>(col->right); c != root; c = static_cast<ColumnNode*>(c->right)) {
			if (c->size < col->size) col = c;
		}
		cover(col);

		for (Node* row = col->down; row != col; row = row->down) {
			solution.push_back(row->rowId);

			for (Node* node = row->right; node != row; node = node->right) {
				cover(node->column);
			}

			if (solve(solution)) return true;
			solution.pop_back();

			for (Node* node = row->left; node != row; node = node->left) {
				uncover(node->column);
			}
		}

		uncover(col);
		return false;
	}

	int getRowId(int r, int c, int v) {
		return (81 * r) + (9 * c) + v;
	}

	int getCellConstraint(int r, int c) {
		return (9 * r) + c;
	}

	int getRowConstraint(int r, int v) {
		return 81 + (r * 9) + v;
	}

	int getColumnConstraint(int c, int v) {
		return 162 + (c * 9) + v;
	}

	int getSubgridConstraint(int subgridId, int v) {
		return 243 + (subgridId * 9) + v;
	}
	
	bool isValidValue(int r, int c, int v, vector<vector<int>>& puzzle) {
		// Check if the value is already in the same row
		for (int i = 0; i < 9; ++i) {
			if (puzzle[r][i] == v) return false;
		}

		// Check if the value is already in the same column
		for (int i = 0; i < 9; ++i) {
			if (puzzle[i][c] == v) return false;
		}

		// Check if the value is already in the same 3x3 subgrid
		int subgridRow = (r / 3) * 3;
		int subgridCol = (c / 3) * 3;
		for (int i = subgridRow; i < subgridRow + 3; ++i) {
			for (int j = subgridCol; j < subgridCol + 3; ++j) {
				if (puzzle[i][j] == v) return false;
			}
		}

		return true;
	}

	vector<int> getPossibleValues(int r, int c, vector<vector<int>>& puzzle) {
		vector<int> possibleValues;

		for (int v = 0; v < 9; ++v) {
			if (isValidValue(r, c, v + 1, puzzle)) {
				possibleValues.push_back(v);
			}
		}

		return possibleValues;
	}

	vector<int> propagateConstraints(int r, int c, vector<vector<int>>& puzzle) {
		return getPossibleValues(r, c, puzzle);
	}

	void setupSudokuMatrix(vector<vector<int>>& puzzle) {
		vector<int> possibleValues;
		possibleValues.reserve(9);

		for (int r = 0; r < 9; ++r) {
			for (int c = 0; c < 9; ++c) {
				int subgridId = (r / 3) * 3 + (c / 3);
				if (puzzle[r][c] != 0) {
					int v = puzzle[r][c] - 1;
					addRow(getRowId(r, c, v), {
						getCellConstraint(r,c),
						getRowConstraint(r,v),
						getColumnConstraint(c,v),
						getSubgridConstraint(subgridId, v)
					});
				}
				else {
					//for (int v = 0; v < 9; ++v) {
					possibleValues = propagateConstraints(r, c, puzzle);
					for(int v : possibleValues){
						addRow(getRowId(r, c, v), {
							getCellConstraint(r,c),
							getRowConstraint(r,v),
							getColumnConstraint(c,v),
							getSubgridConstraint(subgridId, v)
						});
					}
				}
			}
		}
	}

};

int x() {

	// ALL TIMING RAN WITHOUT CONSOLE OUTPUT STATEMENTS - EXCEPT TIMING

	/*
	HARDEST PUZZLE(based on typical backtracking) - 1 iteration
	Typical backtracking algorithm (approx 6500ms)
	Backtracking with heuristics (under 400ms)
	DLX (approx 1.5ms)
	*/

	/* 
	HARDEST PUZZLE(based on typical backtracking)- 100 continuous iteration
	Typical backtracking algorithm (approx 62ms)
	Backtracking with heuristics (approx 4ms)
	DLX (approx 1.5ms)
	*/

	vector<vector<int>> puzzle = {
        {0,0,0,0,0,0,0,0,0},
        {0,0,3,6,0,0,0,0,0},
        {0,7,0,0,9,0,2,0,0},
        {0,5,0,0,0,7,0,0,0},
        {0,0,0,0,4,5,7,0,0},
        {0,0,0,1,0,0,0,3,0},
        {0,0,1,0,0,0,0,6,8},
        {0,0,8,5,0,0,0,1,0},
        {0,9,0,0,0,0,4,0,0}
    };

	/* 
	HARDEST PUZZLE for DLX - 1 iteration
	Typical backtracking algorithm (approx 200ms)
	Backtracking with heuristics (approx 130ms)
	DLX (approx 3ms)
	*/

	/* 
	HARDEST PUZZLE for DLX - 100 continuous iteration
	Typical backtracking algorithm (approx 2ms)
	Backtracking with heuristics (approx 1.5ms)
	DLX (approx 3ms) 
	*/

	//vector<vector<int>> puzzle = {
	//	{8,0,0,0,0,0,0,0,0},
	//	{0,0,3,6,0,0,0,0,0},
	//	{0,7,0,0,9,0,2,0,0},
	//	{0,5,0,0,0,7,0,0,0},
	//	{0,0,0,0,4,5,7,0,0},
	//	{0,0,0,1,0,0,0,3,0},
	//	{0,0,1,0,0,0,0,6,8},
	//	{0,0,8,5,0,0,0,1,0},
	//	{0,9,0,0,0,0,4,0,0}
	//};

	// EASY PUZZLE - 1 iteration
	// Typical backtracking algorithm (approx 3ms)
	// Backtracking with heuristics (approx 1.6ms)
	// DLX (approx 1ms)
    //vector<vector<int>> puzzle = {
    //    {3, 0, 6, 5, 0, 8, 4, 0, 0},
    //    {5, 2, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 8, 7, 0, 0, 0, 0, 3, 1},
    //    {0, 0, 3, 0, 1, 0, 0, 8, 0},
    //    {9, 0, 0, 8, 6, 3, 0, 0, 5},
    //    {0, 5, 0, 0, 9, 0, 6, 0, 0},
    //    {1, 3, 0, 0, 0, 0, 2, 5, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 7, 4},
    //    {0, 0, 5, 2, 0, 6, 3, 0, 0} };

    // Total number of columns for the exact cover matrix (for sudoku: 324 constraints).
    DLX dlx(324);
    dlx.setupSudokuMatrix(puzzle);

    vector<int> solution;
    if (dlx.solve(solution)) {
		// COMMENT THIS BLOCK WHEN TESTING DURATION
        cout << "Sudoku Solved:" << endl;
        vector<vector<int>> solvedPuzzle(9, vector<int>(9, 0));
        for (int rowId : solution) {
            int r = (rowId / 81) % 9;
            int c = (rowId / 9) % 9;
            int v = rowId % 9;
            solvedPuzzle[r][c] = v + 1;
        }
        for (auto& row : solvedPuzzle) {
            for (int num : row) {
                cout << num << " ";
            }
            cout << endl;
        }
    }
    else {
        cout << "No solution found." << endl;
    }

    return 0;
}

int main() {
    auto start = chrono::high_resolution_clock::now();

	//for (int i = 0; i < 100; ++i) {
	x();
	//}

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    cout << elapsed.count() << endl;
    return 0;
}
