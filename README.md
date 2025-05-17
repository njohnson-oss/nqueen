# nqueen
Solves the n-queen puzzle using the hill climbing algorithm. User may specify n (the board size), the number of desired solutions, and which column the queen in the first row is placed.

## Requirements
- gnu c++ compiler

## Compile & Run
Compile:
```
git clone https://github.com/njohnson-oss/nqueen
cd nqueen
make
```

Run:
```
./a.out
```

## Info
- Solutions may not be unique
- Program will not terminate if no solution exists
- Initial queen column begins at 1
- Can quickly solve for n > 1000 on a modern consumer cpu

## Example Solution
```
..O..
....O
.O...
...O.
O....
```
