#include <iostream>
#include <list>
#include <fstream>
#include "dsaturstracture.hpp"
#include "input.hpp"
using namespace std;


class Graph                                // A class that represents an undirected graph
{
    int V;                                // No. of vertices
    std::string filename;                // File name
    list<int> *adj;                     // A dynamic array of adjacency lists

    void fill_Graph(const input &in);       //Fill Neighborhood List (Connection tops)
    void save_to_file(int numbers_of_colors,std::string alco);        //Save results to a file with format (FILENAME, COLORS, ALGO)

public:
    static std::string first_fit_table;
    static std::string dsatur_table;
    Graph(const input &in);                  // Constructor and destructor
    ~Graph() {}

    void greedyColoring();                 // Prints greedy coloring of the vertices
     
    void DSatur();                        // Execution of DSatur algorithm

    void Show();                 //Display results in a dynamic html file. (Visualization (use of table))

};