/*
 * nqueen - solves the nqueen puzzle with one queen fixed in the first row.
 * Copyright (c) 2020 Nicholas Johnson.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sstream>
using namespace std;

bool isSolved(const vector<int> &queens, const vector<vector<int>> &attacks);
void writeSolution(vector<int> &queens, ofstream &fout, const int solNumber);
void fillAttacks(vector<int> &queens, vector<vector<int>> &attacks, const int &row, const int &change);
void assignQueen(vector<int> &queens, vector<vector<int>> &attacks, const int &row, const int &initQueenCol);
bool solve(vector<int>& queens, const int &boardSize, vector<vector<int>>& attacks, int initQueenCol, int giveUp);

// driver
int main()
{
    vector<vector<int>> attacks;
    vector<int> queens;
    srand(time(0));
    int initQueenCol, size, numSols, giveUp;
    stringstream out_file;
    giveUp = 100; // magic number

    cout << "Enter the board size: ";
    cin >> size;
    cout << "Enter the col where you would like to place the first queen: ";
    cin >> initQueenCol;
    cout << "Enter the number of expected solutions: ";
    cin >> numSols;

    out_file << "nqueen_size_" << size << "_numsols_" << numSols << "_initcol_" << initQueenCol;
    ofstream fout(out_file.str());

    for (int i = 1; i <= numSols; i++)
    {
        while (!solve(queens, size, attacks, initQueenCol, giveUp));
        writeSolution(queens, fout, i);
    }

    cout << "Solution(s) found." << endl;
    fout.close();
    return 0;
}

// test if n queens is solved
bool isSolved(const vector<int> &queens, const vector<vector<int>> &attacks)
{
    for (int row = 0; row < queens.size(); row++)
    {
        int col = queens[row]; // stores col value of queen at row
        if (attacks[row][col])
        {
            return false;
        }
    }
    return true;
}

// write out solution to fout
void writeSolution(vector<int> &queens, ofstream &fout, const int solNumber)
{
    fout << solNumber << endl;
    for (int row = 0; row < queens.size(); row++)
    {
        int col = queens[row];

        for (int i = 0; i < queens.size(); i++)
        {
            if (i != col)
                fout << ".";
            else
                fout << "O";
        }
        fout << endl;
    }
    fout << endl;
}

// updates the attacks vector
void fillAttacks(vector<int> &queens, vector<vector<int>> &attacks, const int &row,
    const int &change)
{
    int size = queens.size();
    int col = queens[row];

    if (col == -1)
        return;

    // above
    for (int i = 0; i < row; i++)
        attacks[i][col]+=change;
    // below
    for (int i = row + 1; i < size; i++)
        attacks[i][col]+=change;
    // upper left
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
        attacks[i][j]+=change;
    // upper right
    for (int i = row - 1, j = col + 1; i >= 0 && j < size; i--, j++)
        attacks[i][j]+=change;
    // lower left
    for (int i = row + 1, j = col - 1; i < size && j >= 0; i++, j--)
        attacks[i][j]+=change;
    // lower right
    for (int i = row + 1, j = col + 1; i < size && j < size; i++, j++)
        attacks[i][j]+=change;
}

// places initial queen in first column then updates attacks vector
// every other queen placed by minimum amount of attacks
// attack vector updated to remove old attacks
// attack vector updated to add new attacks
void assignQueen(vector<int> &queens, vector<vector<int>> &attacks,
    const int &row, const int &initQueenCol = -1)
{
    if (initQueenCol > -1)
    {
        queens[row] = initQueenCol;
        fillAttacks(queens, attacks, row, 1);
    }
    else
    {
        vector<int> leastAttackSquares;
        int minAttacks = attacks[row][0];

        for (int col = 0; col < queens.size(); col++)
        {
            if (attacks[row][col] < minAttacks)
            {
                minAttacks = attacks[row][col];
                leastAttackSquares.clear();
                leastAttackSquares.push_back(col);
            }
            else if (attacks[row][col] == minAttacks)
            {
                leastAttackSquares.push_back(col);
            }
        }

        int choice = rand() % leastAttackSquares.size();
        int randomCol = leastAttackSquares[choice];

        fillAttacks(queens, attacks, row, -1);
        queens[row] = randomCol; // stores col value for queen for random tiebreaker
        fillAttacks(queens, attacks, row, 1);
    }
}

// solves the board
bool solve(vector<int>& queens, const int &boardSize, vector<vector<int>>& attacks,
    int initQueenCol, int giveUp)
{
    int size = boardSize;

    queens.clear();
    attacks.clear();

    for (int i = 0; i < size; i++)
    {
        queens.push_back(-1);
    }

    vector<int> blankRow;

    for (int i = 0; i < size; i++)
    {
        blankRow.push_back(0);
    }

    for (int i = 0; i < size; i++)
    {
        attacks.push_back(blankRow);
    }

    // first queen
    assignQueen(queens, attacks, 0, initQueenCol - 1);

    // other queens for N
    for (int row = 1; row < size; row++)
    {
        assignQueen(queens, attacks, row);
    }

    while (!isSolved(queens, attacks) && giveUp > 0)
    {
        giveUp--;

        vector<int> attackingQueens;

        for (int row = 1; row < size; row++)
        {
            int col = queens[row];
            if (attacks[row][col] > 0)
            {
                attackingQueens.push_back(row);
            }
        }

        int choice = rand() % attackingQueens.size();
        int randAttackingRow = attackingQueens[choice];

        assignQueen(queens, attacks, randAttackingRow);
    }

    return giveUp > 0;
}

