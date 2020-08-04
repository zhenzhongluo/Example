#include <vector>
using namespace std;

namespace Island_Perimeter_463
{
    /*
        Idea: 
        - Count the total neighbor cells AND total valid cells.
        - [total valid cells *4 == one island (if one cell is an island itself)] -
          [total neighbor cells]

        Time:  O(ROWS *COLS)
        Space: O(1)
    */
    class Solution_Math_Counting_Easy_To_Read_Version
    {
    public:
        int islandPerimeter(vector<vector<int>>& grid)
        {
            if (grid.empty())
            {
                return 0;
            }

            const int ROWS = grid.size();
            const int COLS = grid[0].size();

            int actual_num_of_neighbors_including_overlapped_neighbors = 0;
            int num_valild_cells = 0;

            for (int row = 0; row < ROWS; ++row)
            {
                for (int col = 0; col < COLS; ++col)
                {
                    if (grid[row][col] == 1)
                    {
                        ++num_valild_cells;

                        actual_num_of_neighbors_including_overlapped_neighbors += ((row - 1) >= 0 && grid[row - 1][col] == 1);
                        actual_num_of_neighbors_including_overlapped_neighbors += ((row + 1) < ROWS && grid[row + 1][col] == 1);
                        actual_num_of_neighbors_including_overlapped_neighbors += ((col + 1) < COLS && grid[row][col + 1] == 1);
                        actual_num_of_neighbors_including_overlapped_neighbors += ((col - 1) >= 0 && grid[row][col - 1] == 1);
                    }
                }
            }

            /*
                NOTE:
                island perimeter = [(number of valid cells * 4 expected neighbors) -
                                    (actual number of neighbors (including overlapped neighbors))].
            */
            int island_perimeter = (num_valild_cells * 4 -
                actual_num_of_neighbors_including_overlapped_neighbors);
            return island_perimeter;
        }
    };

    class Solution_Counting
    {
    public:
        int islandPerimeter(vector<vector<int>>& grid)
        {
            if (grid.empty())
            {
                return 0;
            }

            const int ROWS = grid.size();
            const int COLS = grid[0].size();

            int numCells = 0;
            int totalNeighbors = 0;

            for (int row = 0; row < ROWS; ++row)
            {
                for (int col = 0; col < COLS; ++col)
                {
                    // NOTE: Only check valid cells.
                    if (grid[row][col] == 1)
                    {
                        // Count the number of cells.
                        // At the end, we will find the perimeter by
                        // perimeter = (numCells *4),
                        // assuming each cell is an island.
                        ++numCells;

                        // NOTE:
                        // Count the total neighbors 
                        // among all grid[row][col] == 1.
                        // That means these neighbors can have overlapping edges.
                        if (row > 0 && grid[row - 1][col] == 1)
                        {
                            ++totalNeighbors;
                        }
                        if (row < (ROWS - 1) && grid[row + 1][col] == 1)
                        {
                            ++totalNeighbors;
                        }
                        if (col > 0 && grid[row][col - 1] == 1)
                        {
                            ++totalNeighbors;
                        }
                        if (col < (COLS - 1) && grid[row][col + 1] == 1)
                        {
                            ++totalNeighbors;
                        }
                    }
                }
            }
            return (numCells * 4) - (totalNeighbors);
        }
    };
}
