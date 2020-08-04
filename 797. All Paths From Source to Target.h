#include <vector>
using namespace std;

namespace All_Paths_From_Source_to_Target_797
{
    /*
        Idea:  DFS
        Time:  O(pow(2, n) * n)
        Space: O(pow(2, n) * n)
        n = number of nodes in the directed graph.
    */
	class Solution_DFS_Version
    {
    public:
        vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& directed_graph)
        {
            if(directed_graph.empty())
            {
                return {};
            }

            vector<vector<int>> result_paths;
            vector<int> cur_path;
        
            DFS(directed_graph, 0, cur_path, result_paths);
            return result_paths;
        }
    
    
    
    private:
        void DFS(vector<vector<int>>& directed_graph,
                 int parent_node,
                 vector<int>& cur_path,
                 vector<vector<int>>& result_paths)
        {
            cur_path.push_back(parent_node);
        
            if(directed_graph[parent_node].empty())
            {
                result_paths.push_back(cur_path);
                return;
            }
        
            for(int child_node : directed_graph[parent_node])
            {
                DFS(directed_graph, child_node, cur_path, result_paths);
            
                cur_path.pop_back();
            }
        }
    };
}
/*
    July 2020 LeetCoding Challenge.
    07/24/2020
*/