# C-robot-navigation
This is a program written in C to navigate through a grid of rectangles collecting markers along the way and avoiding all obstacles in its path.
•	I store the markers and blocks in an array to set their coordinates in the grid and draw them onto the background. 
•	The program uses a triangle to represent the robot. 
•	The user inputs the row, column and direction into the command line. This sets the home position that the robot starts on and this home square is fixed in place. If the users command line inputs were invalid(because they put the home square on top of a marker, block or the input is outside the grid then the program will not proceed, or the direction is not between 0 and 3). All that will happen is the grid will be drawn and then nothing happens. A direction of 0 represents right, a direction of up represents 1, 2 represents left, 3 represents down. For example, if the user inputs 0 0 0 into the command line then the home square is the 0th column, 0th row and the direction the robot starts facing is right. However, if the input happens to be invalid such as putting in 0 0 -1 then the program will draw the grid and then not be able to proceed as the error 1 has been returned to main.
•	The program uses a recursive traversal algorithm using a visited list to try to go forward if it can, if that fails then it tries turning left and going forward, it repeats this turning left 3 times in total and if that still fails then it backtracks a square to try a different path with the knowledge that the square it just went to is now ‘visited’. If it doesn’t fail then the recursive call is made to traverse the grid starting at the new position of the robot.
•	Once the robot has visited every single grid in the rectangle then the traversal algorithm terminates. Then the reverse moves function is called to bring it back to its home square.

To run the program in the command line I run:

gcc -o robot robot.c graphics.c
./robot number number number | java -jar drawapp.jar 

where number number number should be 3 valid numbers eg 7 3 3 or 2 1 0
To find a valid number the first 2 numbers should be between 0 to 9 inclusive and not be in the markers array or the blocks array defined on lines 16 and 18. And the direction should be between 0 and 3 inclusive.
