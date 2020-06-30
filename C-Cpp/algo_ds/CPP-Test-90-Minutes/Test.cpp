//
//   NVIDIA Development Tools Team
//       C++ Programming Test
//
// ---------------------------------
// 
// This test should be self-explanatory.  All places where you are expected to
// write code are marked with TODO:  Each question is assigned a score.  Please
// see the SCORING comment below.  You are welcome to implement the questions
// in any order.  Keep in mind the goal is to GET AS MANY POINTS AS YOU CAN IN
// THE TIME ALLOTTED.
//
// This .cpp file should compile and run as-is, but all the tests will fail.
// Please implement all the TODOs as described below to get all the tests to
// succeed.
//
// You may use whatever other #includes you need.  You may use the C++ STL
// unless otherwise specified.  If you need help with STL (e.g. using fstream
// to read/write files), use cppreference.com, or read through the exercises
// and their tests, which have some file I/O already implemented.
//
// You're welcome to use stdio.h and C-style programming instead of C++, but
// you'll have to demonstrate that your memory management is valid, no matter
// whether you prefer C or C++.
//
// SCORING
// Total : 60 points
// 1. TestHierarchy                     45 points
//   a. Parser & data structure            15 points
//   b. PrintHierarchyDepthFirst            5 points
//   c. PrintHierarchyDepthFirstSorted      5 points
//   d. PrintHierarchyBreadthFirst         10 points
//   e. Test vectors                       10 points
// 2. TestStringRotation                15 points
//   a. RotateString                       10 points
//   b. Test vectors                        5 points

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>

// Append data filenames to this string to read/write files in the data folder.
const std::string data_location("/Users/eric/Documents/Projects/kickstart/C-Cpp/algo_ds/CPP-Test-90-Minutes/");

#define CHECK(cond, errstream, oss) \
    if (!(cond)) \
    { \
        (oss) << errstream << std::endl; \
        return; \
    }

// -----------------------------------------------------------------------------
// Problem:
// Create a simple data structure to represent a hierarchy.  Write a parser to
// read files in the following format and store them in your hierarchy structure.
// Then write functions that print the hierarchy using multiple traversal
// algorithms.
// 
// Example input file format (note that leading periods specify depth):
//
// grandpa & grandma
// .mom
// ..me
// ...my son
// ...my daughter
// ..my brother
// ..my sister
// ...my nephew
// .aunt gertrude
// ..my annoying cousin
//
// Make sure that you detect nonsensical structures in the input file.  For
// example, the first line should not contain any leading periods, and a line
// with one leading period cannot be followed by a line with more than two
// leading periods.
//
// Because the printing functions are external to your class, you'll need to
// choose what type of interface to expose for those functions to be simple.
//
// The logic for the printing functions should NOT be contained within the class.

class Hierarchy;

class HierNode
{
public:
  friend class Hierarchy;
  HierNode():n_vec_child(0), n_data(""), n_depth(0){}
  HierNode(std::string data, int depth):n_vec_child(0), n_data(data), n_depth(depth){}
	

  std::string GetData() const;
  int GetDepth() const;
  std::vector<HierNode*> GetVecChild() const;

  void SetData(const std::string data);
  void AddChild(HierNode* child);

private:
  std::vector<HierNode*> n_vec_child;
  std::string n_data;
  int n_depth;
};

std::string HierNode::GetData() const{
  return n_data;
}

int HierNode::GetDepth() const{
  return n_depth;
}

std::vector<HierNode*> HierNode::GetVecChild() const{
  return n_vec_child;
}

void HierNode::SetData(const std::string data){
  n_data = data;
}

void HierNode::AddChild(HierNode* child){
  n_vec_child.push_back(child);
}

class Hierarchy 
{
public:
  Hierarchy():proot(NULL){}

  HierNode *GetRoot() const;

private:
  HierNode *proot;
    
public:
    // Return false if any I/O errors occur, otherwise return true.
    // The input file should have only one root node.
    bool LoadFromFile(const std::string& infile)
    {
        // NOTE: Feel free to replace the entire implementation of
        // this function.
        std::ifstream ifs(infile.c_str());
        if (!ifs)
        {
            return false;
        }

        std::stack<HierNode*> node_stack;
        int depth = 0;
        std::string line;
        while (std::getline(ifs, line))
        {
            depth = count(line.begin(),line.end(),'.');
            if (proot == NULL){
                proot = new HierNode(line, 0);
                node_stack.push(proot);
            }
            else{
                while (depth < node_stack.size())
                {
                    node_stack.pop();
                }
                HierNode *current_node = node_stack.top();
                HierNode *new_node = new HierNode(line.substr(depth), depth);
                current_node->AddChild(new_node);
                node_stack.push(new_node);
            }
        }

        return true;
    }

};

HierNode* Hierarchy::GetRoot() const{
	return proot;
}


// TODO: Implement the following Print* functions.
// - Print each node on a separate line, with two leading spaces for each depth
//   level.
// - The depth-first output should match the input order but the prefix should be
//   different.
// - The sorted depth-first output should sort the children of each node before
//   recursing into their children.
// - The breadth-first output should print all nodes at a given depth before
//   printing any deeper nodes.
// - See the files HierarchyTest_expected_*.txt in the
// Data directory for the expected results.
//
// Feel free to use helper functions, but don't change the signatures of these
// functions.

void PrintHierarchyDepthFirst(const Hierarchy& h, std::ostream& output)
{
  int current_depth = 0;
  std::stack<HierNode*> dfs_stack;
  std::stack<int> depth_stack;
  HierNode *current_node = h.GetRoot();
  dfs_stack.push(current_node);
  depth_stack.push(current_depth);
  std::cout<<current_node->GetData()<<std::endl;

  while (!dfs_stack.empty())
  {   
      int indent = depth_stack.top();
      HierNode* top = dfs_stack.top();
      for (unsigned int i = 0; i < top->GetDepth(); i++){
            output << "  ";
      }
      depth_stack.pop();

      output << top->GetData() << std::endl;
      std::cout<<top->GetData()<<std::endl;
      dfs_stack.pop();

      std::vector<HierNode*> vec_child = top->GetVecChild();
      current_depth++;
      for (auto it = vec_child.end(); it != vec_child.begin();){
          dfs_stack.push(*--it);
          depth_stack.push(current_depth);
      }
  }
}

void PrintHierarchyDepthFirstSorted(const Hierarchy& h, std::ostream& output)
{
  int current_depth = 0;
  std::stack<HierNode*> dfs_stack;
  std::stack<int> depth_stack;
  HierNode *current_node = h.GetRoot();
  dfs_stack.push(current_node);
  depth_stack.push(current_depth);
  std::cout<<current_node->GetData()<<std::endl;

  while (!dfs_stack.empty())
  {   
      int indent = depth_stack.top();
      HierNode* top = dfs_stack.top();
      for (unsigned int i = 0; i < top->GetDepth(); i++){
            output << "  ";
      }
      depth_stack.pop();

      output << top->GetData() << std::endl;
      std::cout<<top->GetData()<<std::endl;
      dfs_stack.pop();

      std::vector<HierNode*> vec_child = top->GetVecChild();
      std::sort(std::begin(vec_child), std::end(vec_child), 
                [](HierNode* a, HierNode* b){return a->GetData() < b->GetData();});
      current_depth++;
      for (auto it = vec_child.end(); it != vec_child.begin();){
          dfs_stack.push(*--it);
          depth_stack.push(current_depth);
      }
  }
}

void PrintHierarchyBreadthFirst(const Hierarchy& h, std::ostream& output)
{
  int current_depth = 0;
  std::queue<HierNode*> bfs_queue;
  std::queue<int> depth_queue;
  HierNode *current_node = h.GetRoot();
  bfs_queue.push(current_node);
  depth_queue.push(current_depth);
  std::cout<<current_node->GetData()<<std::endl;

  while (!bfs_queue.empty())
  {   
      int indent = depth_queue.front();
      HierNode* front = bfs_queue.front();
      for (unsigned int i = 0; i < front->GetDepth(); i++){
            output << "  ";
      }
      
      output << front->GetData() << std::endl;
      std::cout<<front->GetData()<<std::endl;
      
      std::vector<HierNode*> vec_child = front->GetVecChild();
      current_depth++;
      for (auto it = vec_child.end(); it != vec_child.begin();){
          bfs_queue.push(*--it);
          depth_queue.push(current_depth);
      }
      depth_queue.pop();
      bfs_queue.pop();
  }
}

// TODO: Improve this test method.
// How could you verify your outputs are correct?
// How could you refactor this function to be less repetitive?
void TestHierarchy(std::ostream& error_output)
{
    const std::string failure_msg("Hierarchy tests failed!");

    bool success = false;

    Hierarchy h;
    std::string infile(data_location + "HierarchyTest.txt");
    success = h.LoadFromFile(infile);
    CHECK(success, failure_msg << "  I/O error.", error_output);

    std::string outfile;
    std::string testfile;
    bool identical(false);

    outfile = data_location + "HierarchyTest_output_DFS.txt";
    {
        std::ofstream ofs(outfile.c_str());
        // If this fails, manually verify that outfile is not read-only on disk
        CHECK(ofs, failure_msg << "  I/O error.", error_output);

        PrintHierarchyDepthFirst(h, ofs);
    }

    outfile = data_location + "HierarchyTest_output_DFS_sorted.txt";
    {
        std::ofstream ofs(outfile.c_str());
        // If this fails, manually verify that outfile is not read-only on disk
        CHECK(ofs, failure_msg << "  I/O error.", error_output);

        PrintHierarchyDepthFirstSorted(h, ofs);
    }

    outfile = data_location + "HierarchyTest_output_BFS.txt";
    {
        std::ofstream ofs(outfile.c_str());
        // If this fails, manually verify that outfile is not read-only on disk
        CHECK(ofs, failure_msg << "  I/O error.", error_output);

        PrintHierarchyBreadthFirst(h, ofs);
    }
}

// -----------------------------------------------------------------------------
// Problem:
// RotateString rotates S by R positions.  If R is positive rotate to the right.
// If R is negative rotate to the left.  "Rotating" means that characters
// shifted off one end roll back around to the other end.
//
// Explain how much time and memory your solution takes based on R and the
// length of S.  Note:  Please start out with a simple algorithm, and only come
// back and optimize it if you have time left after doing the other exercises.
//

void RotateRight(std::string& s){
  int n = s.length();
	char t = s[n-1];
	for(int i = n - 1; i > 0; --i){
		s[i] = s[i-1];	
	}
	s[0] = t;
}

void RotateString(std::string& S, int R)
{
  if(S.empty() || R == 0 || abs(R) == S.length()){
		return;
	}
	
	if(R < 0){
		while (R < 0){
			R += S.length();	
		}
	}

	while(R--)
	{
		RotateRight(S);
	}
}

void TestStringRotation(std::ostream& error_output)
{
    const std::string failure_msg("String rotation failed!");

    std::string s1("sprint");
    RotateString(s1, 5);
    CHECK(s1 == "prints", failure_msg, error_output);

    std::string s2("StringRotated");
    RotateString(s2, 7);
    CHECK(s2 == "RotatedString", failure_msg, error_output);

    std::string s3("");
    RotateString(s3, 5);
    CHECK(s3 == "", failure_msg, error_output);

    std::string s4("looooooooooooooooooooooooooooog");
    RotateString(s4, 10);
    CHECK(s4 == "ooooooooogloooooooooooooooooooo", failure_msg, error_output);
}


// -----------------------------------------------------------------------------

int main()
{
    std::ostringstream oss;

    TestHierarchy(oss);
    TestStringRotation(oss);

    // Print output to console
    std::cout << oss.str();
    
    // Pause...
    std::cout << std::endl
        << std::endl
        << "Press enter to quit..." << std::endl;
    char c;
    std::cin >> std::noskipws >> c;

    return 0;
}
