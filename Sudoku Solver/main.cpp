#include <iostream>

using namespace std;

class Sudoku {
public:
   static bool isSafe(int sudoku[][9], int row, int col, int digit) {
       // for column
       for (int i = 0; i < 9; i++) {
           if (sudoku[i][col] == digit) {
               return false;
           }
       }

       // for row
       for (int j = 0; j < 9; j++) {
           if (sudoku[row][j] == digit) {
               return false;
           }
       }

       // for grid
       int sr = (row / 3) * 3;
       int sc = (col / 3) * 3;
       for (int i = sr; i < sr + 3; i++) {
           for (int j = sc; j < sc + 3; j++) {
               if (sudoku[i][j] == digit) {
                   return false;
               }
           }
       }

       return true;
   }

   static bool sudokuSolver(int sudoku[][9], int row, int col) {
       // base case
       if (row == 9 && col == 0) {
           return true;
       }

       // recursion
       int nxtrow = row;
       int nxtcol = col + 1;
       if (col + 1 == 9) {
           nxtrow = row + 1;
           nxtcol = 0;
       }

       if (sudoku[row][col] != 0) {
           return sudokuSolver(sudoku, nxtrow, nxtcol);
       }

       for (int digit = 1; digit <= 9; digit++) {
           if (isSafe(sudoku, row, col, digit)) {
               sudoku[row][col] = digit;
               if (sudokuSolver(sudoku, nxtrow, nxtcol)) {
                   return true;
               }
               sudoku[row][col] = 0;
           }
       }

       return false;
   }

   static void printSudoku(int sudoku[][9]) {
       for (int i = 0; i < 9; i++) {
           for (int j = 0; j < 9; j++) {
               cout << sudoku[i][j] << " ";
           }
           cout << endl;
       }
   }
};

int main() {
   int sudoku[9][9] = {
       {0, 0, 8, 0, 0, 0, 0, 0, 0},
       {4, 9, 0, 1, 5, 7, 0, 0, 2},
       {0, 0, 3, 0, 0, 4, 1, 9, 0},
       {1, 8, 5, 0, 6, 0, 0, 2, 0},
       {0, 0, 0, 0, 2, 0, 0, 6, 0},
       {9, 6, 0, 4, 0, 5, 3, 0, 0},
       {0, 3, 0, 0, 7, 2, 0, 0, 4},
       {0, 4, 9, 0, 3, 0, 0, 5, 7},
       {8, 2, 7, 0, 0, 9, 0, 1, 3}
   };

   if (Sudoku::sudokuSolver(sudoku, 0, 0)) {
       cout << "Solution Exists : " << endl;
       Sudoku::printSudoku(sudoku);
   } else {
       cout << "Solution Does not Exist" << endl;
   }

   return 0;
}
