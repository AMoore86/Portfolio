# Dynamic Programming - Number Triangle Project
## Goal
The goal of this program is to calculate the maximum product of numbers passed on a route that starts at the top and ends somewhere on the base of a number triangle. Each step can go either diagonally down to the left or down to the right. The following is an example of a number triangle:  

![Number Triangle](https://github.com/AMoore86/Portfolio/new/main/Dynamic%20Programming%20-%20Number%20Triangle/NumberTriangle.png)

## The Program
### Input
The program will read input from a text file (test.txt for our case). The first line contains one integer N which corresponds to the number of rows in the triangle. The following N lines of the text file describe the rows of the triangle. The number of rows in the triangle may be between 1 and 99. Any single integer within the triangle may be between 0 and 99.

For example, one possible input could be as follows:  
5  
7  
3 8  
8 1 0  
2 7 4 4  
4 5 2 6 5  

### Output
The program will write the output into a separate text file (out.txt for our case). The first line will contain the resultant maximum product as an integer calculated from traversing the number triangle. The following line contains the (ordered) sequence of numbers from the triangle that were utilized to produce the maximum product.

For our example, the output would read as follows:  
5880  
7 3 8 7 5  

### More Information
In order to efficiently traverse the triangle, the program recursively iterates through the tree, starting at the bottom) and multiplies the larger of the two children to its parent. Doing this recursively will result in the final maximum product being placed as the root node of the tree/triangle. During the traversal, the program is also keeping a list/array of the factors utilized to produce the maximal product.  

Using dynamic programming, the time complexity of calculating the desired output is reduced from O(2^n) to O(n^2). This is a significant improvement and definitely allows for the calculation of a number triangle with (the desired upper limit of) 99 rows and potentially even more.
