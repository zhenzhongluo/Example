#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
using namespace std;

/*
    Reference:
    - https://zxi.mytechroad.com/blog/searching/leetcode-212-word-search-ii/
*/
namespace Word_Search_212
{
    class TrieNode
    {
    public:
        TrieNode() : word_ptr(nullptr),
                     children(26)
        {
        }

        ~TrieNode()
        {
            for (TrieNode* child : children)
            {
                child = nullptr;
                delete child;
            }
        }

        const string* word_ptr;
        vector<TrieNode*> children;
    };

    class Trie
    {
    public:
        Trie(const vector<string>& words) : m_root(new TrieNode())
        {
            for (const string& word : words)
            {
                insert(word);
            }
        }

        TrieNode* get_root() const
        {
            return m_root;
        }

    private:
        void insert(const string& word)
        {
            TrieNode* ptr = m_root;

            for (const char& letter : word)
            {
                int index = (letter - 'a');

                if (!ptr->children[index])
                {
                    ptr->children[index] = new TrieNode();
                }
                ptr = ptr->children[index];
            }
            ptr->word_ptr = &word;

            ptr = nullptr;
            delete ptr;
        }

    private:
        TrieNode* m_root;
    };

    /*
        Idea:
        - Store all the words into a trie.
        - Search the board using DFS().
        - Paths must be in the trie,
          otherwise there is no need to explore.

        Time:  O(sum(l) + pow((4 Directions), max(l)))
        Space: O(sum(l) + l)
        l = offset of one word.
    */
    class Solution_Trie_DFS_Lambda_Version
    {
    public:
        vector<string> findWords(vector<vector<char>>& board,
                                 vector<string>& words)
        {
            Trie trie(words);
            TrieNode* root = trie.get_root();

            const int ROWS = board.size();
            const int COLS = board[0].size();

            vector<string> existed_words;

            function<void(int, int, TrieNode*)> DFS = [&](int row,
                                                          int col,
                                                          TrieNode* parent_node)
            {
                if (row < 0 ||
                    col < 0 ||
                    row >= ROWS ||
                    col >= COLS ||
                    board[row][col] == '#') // visited cells
                {
                    return;
                }

                // Make a copy of the current letter.
                const char cur_letter = board[row][col];

                int letter_index = (cur_letter - 'a');
                TrieNode* child_node = parent_node->children[letter_index];

                if (!child_node)
                {
                    return;
                }

                // Update the result since we have found a valid word.
                if (child_node->word_ptr)
                {
                    existed_words.push_back(*child_node->word_ptr);

                    /*
                        NOTE: MUST REMEMBER!!
                        Since we have visited this word, 
                        we don't want to visited it again. 

                        Therefore, we need to remove it from our visiting list.
                    */
                    child_node->word_ptr = nullptr;
                }

                // Set the current cell visited.
                board[row][col] = '#';

                // DFS Process.
                DFS((row + 1), col, child_node);
                DFS((row - 1), col, child_node);
                DFS(row, (col + 1), child_node);
                DFS(row, (col - 1), child_node);

                // Reset the cell because the current stack has been processed.
                board[row][col] = cur_letter;
            };

            // Main Process: Try all possible paths.
            for (int row = 0; row < ROWS; ++row)
            {
                for (int col = 0; col < COLS; ++col)
                {
                    DFS(row, col, root);
                }
            }
            return existed_words;
        }
    };

	namespace Deprecated
	{
		struct TrieNode
		{
			vector<TrieNode*> childList = vector<TrieNode*>(26, nullptr);
			bool isLeaf = false;
		};

		class Solution
		{
		public:
			vector<string> findWords(vector<vector<char>>& board, vector<string>& words)
			{
				vector<string> result;

				if (board.size() == 0 || words.size() == 0)
				{
					return result;
				}

				m_root = new TrieNode();

				CreateTrie(m_root, words);

				int rows = board.size();
				int cols = board[0].size();

				vector<vector<bool>> visited(rows, vector<bool>(cols, false));

				string str = "";

				TrieNode* root = m_root;

				for (int row = 0; row < rows; row++)
				{
					for (int col = 0; col < cols; col++)
					{
						char c = board[row][col];

						int i = c - 'a';

						if (root->childList[i])
						{
							str += c;

							SearchWord(root->childList[i], row, col, rows, cols, visited, str, result, board, words);

							str = "";
						}
					}
				}

				sort(result.begin(), result.end());
				return result;
			}

			void SearchWord(TrieNode* root, int row, int col, int rows, int cols,
				vector<vector<bool>>& visited, string& str,
				vector<string>& result,
				vector<vector<char>>& board, vector<string>& words)
			{

				if (root->isLeaf && !visited[row][col] &&
					find(result.begin(), result.end(), str) == result.end())
				{
					result.push_back(str);
				}

				if (WithinRange(row, col, rows, cols, visited))
				{
					visited[row][col] = true;

					// search each alphabet in the current child list
					for (int i = 0; i < 26; i++)
					{
						char c = (char)i + 'a';

						int index = c - 'a';

						if (root->childList[index])
						{
							str += c;

							// down
							if (WithinRange(row + 1, col, rows, cols, visited) && board[row + 1][col] == c)
							{
								SearchWord(root->childList[index], row + 1, col, rows, cols, visited, str, result, board, words);
							}

							// up
							if (WithinRange(row - 1, col, rows, cols, visited) && board[row - 1][col] == c)
							{
								SearchWord(root->childList[index], row - 1, col, rows, cols, visited, str, result, board, words);
							}

							// left
							if (WithinRange(row, col - 1, rows, cols, visited) && board[row][col - 1] == c)
							{
								SearchWord(root->childList[index], row, col - 1, rows, cols, visited, str, result, board, words);
							}

							// right
							if (WithinRange(row, col + 1, rows, cols, visited) && board[row][col + 1] == c)
							{
								SearchWord(root->childList[index], row, col + 1, rows, cols, visited, str, result, board, words);
							}

							str = str.substr(0, str.length() - 1);
						}
					}

					visited[row][col] = false;
				}
			}

			void CreateTrie(TrieNode* root, vector<string>& words)
			{
				for (auto& s : words)
				{
					Insert(root, s);
				}
			}

			void Insert(TrieNode* node, string& s)
			{
				for (auto& c : s)
				{
					int index = c - 'a';

					if (!node->childList[index])
					{
						node->childList[index] = new TrieNode();
					}

					node = node->childList[index];
				}

				node->isLeaf = true;
			}

			bool WithinRange(int row, int col, int rows, int cols, vector<vector<bool>>& visited)
			{
				return (row >= 0 && row < rows) && (col < cols&& col >= 0) && (!visited[row][col]);
			}

		private:
			TrieNode* m_root;
		};
	}
}
/*
    Microsoft 2020 2nd Round
    07/21/2020

    June 2020 LeetCoding Challenge.
    06/30/2020
*/