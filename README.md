# klotski
A [Klotski](https://en.wikipedia.org/wiki/Klotski) solver that I created after being stuck on `Set 1 - Level 22` of [this Klotski app](https://play.google.com/store/apps/details?id=com.alcamasoft.juegos.klotski.android&hl=en).

## Build
```
g++ -std=gnu++17 klotski.cpp -o klotski
```

## Run

The programs reads from standard input. You must enter the board position by using the following characters for the pieces:  
 - `.` for an empty square
 - `0` for a 1x1 piece
 - 2 `|` characters in the same column, for a 1x2 piece
 - 2 `-` characters in the same row, for a 2x1 piece.
 - 4 `@` characters making a 2x2 square, for the 2x2 piece.
 
 ### Sample input (representing [this position](https://en.wikipedia.org/wiki/File:Quo_Vadis-HABA.jpg) of the board):
 ```
|@@|
|@@|
|--|
|00|
0..0
 ```
 
 ### Sample output
 
 The output for the above mentioned sample input is in [sample-output.txt](https://github.com/dryairship/klotski/blob/main/sample-output.txt).
 
 ## Comments
 
  - The program doesn't necessarily find the shortest possible solution.
  - The numbers mentioned in the program output are the sequence numbers of the distinct board positions. This may not be equal to the number of moves, depending on the definition of a move. (eg: Moving the same piece twice is sometimes considered a single move).
  - The program assumes that the dimensions of the board are 5x4. I believe the program should work for other dimensions too, by changing the constants on lines 12 and 13. Though it would take longer to solve larger boards.
