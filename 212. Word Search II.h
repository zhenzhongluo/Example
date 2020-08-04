#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
using namespace std;

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
}
/*
    Microsoft 2020 2nd Round
    07/21/2020

    June 2020 LeetCoding Challenge.
    06/30/2020
*/
