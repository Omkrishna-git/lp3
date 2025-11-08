#include<bits/stdc++.h>
using namespace std;


/*
    Time complexity for Approach-1 : O(N!)
    Unlike the brute force approach, we will only place queens on squares that aren't under attack.
    For the first queen, we have N options. For the next queen, we won't attempt to place it in the
    same column as the first queen, and there must be at least one square attacked diagonally by the
    first queen as well. Thus, the maximum number of squares we can consider for the second queen is
    (N−2). For the third queen, we won't attempt to place it in 2 columns already occupied by the first
    2 queens, and there must be at least two squares attacked diagonally from the first 2 queens.
    Thus, the maximum number of squares we can consider for the third queen is (N-4).
    This pattern continues, resulting in an approximate time complexity of O(N!)
*/

//Approach-1 (Simple dfs)
//T.C : O(N!) - Read the reason above
//S.C : O(N) to store the result

class NQueenDFS  {
public:
    vector<vector<string>> result;
    bool isValid(vector<string>& board, int row, int col) {
        //look for up
        for(int i = row; i>=0; i--) {
            if(board[i][col] == 'Q')
                return false;
        }
        
        //check left diagonal upwards
        for(int i = row, j = col; i>=0 && j >= 0; i--, j--) {
            if(board[i][j] == 'Q')
                return false;
        }
        
        //check right diagonal upwards
        for(int i = row, j = col; i>=0 && j<board.size(); i--, j++) {
            if(board[i][j] == 'Q')
                return false;
        }
        
    /*
        Wait a second, Why didn't I check any squares downwards ???
        If you notice, every time I am calling dfs(board, row+1); i.e. after
        placing a Queen at a row, I move down. So, It's guaranteed I will
        not get any Queen downwards.
        Example :

        For n = 4

                _   _    _  Q   (Put int the first row)
                Q  _   _   _    (While putting here, I only need to see above of me because I have not populated any Q in downwards)
                _   _   Q  _    (Same,  While putting here, I only need to see above of me because I have not populated any Q in downwards)

                So, on

	*/
        
        
        return true;
    }
    void solve(vector<string>& board, int row) {
        if(row == board.size()) {
            result.push_back(board);
            return;
        }
        
        /*
            place one queen at every row and check before placing
            in every directions where there is risk if being attackes
            i.e. up, diagonally because we are placing queens from
            top row to bottom row, so we need to check if we put a queen
            vertically up in some row or diagonally upwards in some row
        */
        for(int i = 0; i<board.size(); i++) {
            if(isValid(board, row, i)) {
                board[row][i] = 'Q';
                
                solve(board, row+1);
                
                board[row][i] = '.';
            }
        }
    }

    vector<vector<string>> solveNQueens(int n) {
        if(n == 0)
            return {};
        vector<string> board(n, string(n, '.')); 
        //For, n = 3, board = {"...", "...", "..."} initially
        
        solve(board, 0);
        
        return result;
    }
};

//Approach-2
//T.C : O(N!) in worst case it explores all possible configurations
//S.C : O(N) for result and also for storing, cols, diags and antidiags
class NQueenOptimized  {
public:
    vector<vector<string>> result;
    void solve(vector<string>& board, int row, unordered_set<int>& cols, unordered_set<int>& diags, unordered_set<int>& antiDiags) {
        if(row == board.size()) {
            result.push_back(board);
            return;
        }
        
        /*
            For a square (i, j) :
            Diagonally (i-j)      is constant
            Anti diagonally (i+j) is constant
            
            We can use this to find which square(i, j)
            has a risk of being attacked by another queen
            placed already in 'diagonal', or 'anti-diagonal'
            or 'column'
        */
        
        for(int col = 0; col<board.size(); col++) {
            int diag_id       = row-col;
            int anti_diag_id  = row+col;
            
            /*
                If the col, or diagonal or anti_diaonal
                are used, means one of them has a Queen placed
                already which can attack, so look for other column
            */
           
            if(cols.find(col) != cols.end() ||
              diags.find(diag_id) != diags.end() ||
              antiDiags.find(anti_diag_id) != antiDiags.end())
                continue;
            
            cols.insert(col);
            diags.insert(diag_id);
            antiDiags.insert(anti_diag_id);
            board[row][col] = 'Q';
            
            
            solve(board, row+1, cols, diags, antiDiags);

            cols.erase(col);
            diags.erase(diag_id);
            antiDiags.erase(anti_diag_id);
            board[row][col] = '.';
        }
    }

    vector<vector<string>> solveNQueens(int n) {
        if(n == 0)
            return {};
        vector<string> board(n, string(n, '.')); 
        //For, n = 3, board = {"...", "...", "..."} initially
        
        int start_row = 0;
        unordered_set<int> cols;
        unordered_set<int> diags;
        unordered_set<int> antiDiags;
        solve(board, start_row, cols, diags, antiDiags);
        
        return result;
    }
};


// | Approach                   | Validation           | Time per Step | Total Time            | Space   | Remarks                     |
// | -------------------------- | -------------------- | ------------- | --------------------- | ------- | --------------------------- |
// | **1. Simple DFS**          | Scan board each time | `O(N)`        | `O(N × N!)` ≈ `O(N!)` | `O(N²)` | Easy to understand but slow |
// | **2. Optimized with Sets** | O(1) via sets        | `O(1)`        | `O(N!)`               | `O(N²)` | Much faster in real runs    |



int main() {
    int n;
    cout << "Enter the size of chessboard (N): ";
    cin >> n;

    cout << "\nSelect approach:\n";
    cout << "1. Simple DFS Backtracking\n";
    cout << "2. Optimized Backtracking (using sets)\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    vector<vector<string>> result;

    if (choice == 1) {
        NQueenDFS sol;
        result = sol.solveNQueens(n);
    } else if (choice == 2) {
        NQueenOptimized sol;
        result = sol.solveNQueens(n);
    } else {
        cout << "Invalid choice!\n";
        return 0;
    }

    cout << "\nTotal solutions found: " << result.size() << "\n\n";

    int solNo = 1;
    for (auto& board : result) {
        cout << "Solution " << solNo++ << ":\n";
        for (auto& row : board)
            cout << row << "\n";
        cout << "\n";
    }

    return 0;
}
