#include <vector>
using namespace std;

namespace Single_Number_III_260
{
    /*
        Idea:  Bit Manipulation XOR Two Pass.
        Time:  O(n)
        Space: O(1)
    */
    class Solution_XOR_Two_Pass
    {
    public:
        vector<int> singleNumber(vector<int>& nums)
        {

            /*
                NOTE:
                bit_mask will contain all the 1 bits
                for the two numbers in the array
                which appear only once.
            */
            int bit_mask = 0;
            for (int num : nums)
            {
                bit_mask ^= num;
            }

            // Get the rightmost 1-bit from the bit mask.
            int rightmost_1_bit = (bit_mask & (-bit_mask));

            int X = 0;

            // Get the first number.
            // It's also a bit mask which will contain ONLY X.
            for (int num : nums)
            {
                if ((num & rightmost_1_bit) != 0)
                {
                    X ^= num;
                }
            }

            /*
                NOTE:
                At this point, X becomes a new_bit_mask for Y.
                Get the second number.
            */
            int Y = (bit_mask ^ X);

            return { X, Y };
        }
    };
}

