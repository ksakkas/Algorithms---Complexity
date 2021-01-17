#include "greedy.hpp"
#include <sstream>
#include <windows.h>

string Graph::first_fit_table="";
string Graph::dsatur_table="";

//-------------------------------------------------------
//Εύρεση τρέχοντος φακέλου εργασίας
//ref::https://stackoverflow.com/questions/875249/how-to-get-current-directory
//--------------------------------------------------------

string GetExeFileName()          //File name
{
  char buffer[MAX_PATH];
  GetModuleFileName( NULL, buffer, MAX_PATH );
  return string(buffer);
}

//-----------------------------------------------------------

string GetExePath()        //Path
{
  string f = GetExeFileName();
  return f.substr(0, f.find_last_of( "\\/" ));
}

//-------------------------------------------------------

void Graph::fill_Graph(const input &in)         //Fill Graph
{
    const int size = in.size();
    this->V = size;
    this->adj = new  list<int>[size];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j)
                continue;
            if (in.is_adjacence(i, j))
            {
                this->adj[i].push_back(j);
            }
        }
    }
}

//------------------------------------------------------

void Graph::save_to_file(int numbers_of_colors, string alco)    //Save to txt
{
     vector < string> vec;
     ifstream is;
    is.open("results.txt");
     string line;
    while(is>>line)
    {
        vec.push_back(line);
    }
    is.close();
     string fn;
     string alc;
    for(auto &x:vec)
    {
         stringstream ss(x);
         getline(ss,fn,',');
         getline(ss,alc,',');
         getline(ss,alc,',');
        if(fn==filename && alco==alc)
        {
            return;
        }
    }
     ofstream out;
    out.open("results.txt", ios::app);
    out<<this->filename<<","<<numbers_of_colors<<","<<alco<< endl;
    out.close();
}

Graph::Graph(const input &in)          //Graph from in
{
    this->fill_Graph(in);
    this->filename=in.getFilename();
}

// Assigns colors (starting from 0) to all vertices and prints
// the assignment of colors
void Graph::greedyColoring()
{
    int *result = new int[this->V];
    bool *available = new bool[this->V];

    // Assign the first color to first vertex
    result[0] = 0;

    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++)
        result[u] = -1; // no color is assigned to u

    // A temporary array to store the available colors. True
    // value of available[cr] would mean that the color cr is
    // assigned to one of its adjacent vertices;
    for (int cr = 0; cr < V; cr++)
        available[cr] = true;

    // Assign colors to remaining V-1 vertices
    for (int u = 1; u < V; u++)
    {
        // Process all adjacent vertices and flag their colors
        // as unavailable

        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = false;

        // Find the first available color
        int cr;
        for (cr = 0; cr < V; cr++)
            if (available[cr])
                break;

        result[u] = cr; // Assign the found color

        // Reset the values back to true for the next iteration
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = true;
    }

    // print the result
    for (int u = 0; u < V; u++)
        cout << "\tVertex:" << u << " --->  Color:" << result[u] << endl;
    this->save_to_file(* max_element(result,result+this->V)+1,"FIRST_FIT");

    delete[] result;
    delete[] available;
}

void Graph::DSatur()                    //Dsatur alg
{
    vector<a_data> coloring;
    vector<int> saturationlevel;
    vector<int> saturcolors;
    vector<int> todo;
    int maxvertex = 0;
    int degree = this->adj[0].size();
    for (int i = 1; i < this->V; i++)
    {
        if (this->adj[i].size() > degree)
        {
            maxvertex = i;
            degree = this->adj[i].size();
        }
    }
    coloring.push_back(a_data{maxvertex, 0});
    for (int i = 0; i < this->V; i++)
    {
        saturationlevel.push_back(0);
        if (i != maxvertex)
        {
            todo.push_back(i);
        }
    }
    saturationlevel[maxvertex] = INT_MIN;
    for (auto itr = this->adj[maxvertex].begin(); itr != this->adj[maxvertex].end(); itr++)
    {
        saturationlevel[*itr]++;
    }
    while (!todo.empty())
    {
        int saturvertex = -1;
        int saturation = -1;
        for (int i = 0; i < this->V; i++)
        {
            if (saturationlevel.at(i) > saturation)
            {
                saturation = saturationlevel.at(i);
                saturvertex = i;
            }
        }
        saturcolors.clear();
        for (auto itr = this->adj[saturvertex].begin(); itr != this->adj[saturvertex].end(); itr++)
        {
            for (int i = 0; i < coloring.size(); i++)
            {
                if (coloring.at(i).vertex == *itr)
                {
                    saturcolors.push_back(coloring.at(i).color);
                }
            }
        }
        auto itr = todo.begin();
        auto end = todo.end();
        while (itr != end)
        {
            if (*itr == saturvertex)
            {
                todo.erase(itr);
                break;
            }
            itr++;
        }
        int lc = 0;
        bool fin = false;
        while (!fin)
        {
            fin = true;
            for (auto &x : saturcolors)
            {
                if (x == lc)
                {
                    lc++;
                    fin = false;
                }
            }
        }
        coloring.push_back(a_data{saturvertex, lc});
        saturationlevel[saturvertex] = INT_MIN;
        for (auto i = this->adj[saturvertex].begin(); i != this->adj[saturvertex].end(); i++)
        {
            if (saturationlevel[*i] != INT_MIN)
            {
                saturationlevel[*i]++;
            }
        }
    }
    this->save_to_file( max_element(coloring.begin(),coloring.end())->color+1,"DSATUR");
     cout<<"\t\tDSATUR RESULTS::"<<coloring.size()<< endl;
     cout<<"----------------------------------------------"<< endl;
    for(auto &x:coloring)
    {
        x.print();
    }
     cout<<"\t\tcolors used:"<< max_element(coloring.begin(),coloring.end())->color+1<< endl;
}

void Graph::Show()         //Html page
{
    Graph::first_fit_table="<u><h1 style=\"margin-bottom:35px; color: #3333cc; \">FIRST FIT RESULTS BY FILE</h1></u><table id=\"customers\" border=\"3\" style=\" width:60%; backgound-color:; color:; font-size:20px; font-weight:bold;\"><tr style=\"background-color:#006666; color:white; font-size:22px;\"><th>FILENAME</th><th>COLORS USED</th><th>ALGORITHM EXECUTED</th></tr>";
    Graph::dsatur_table="<u><h1 style=\"margin-bottom:35px; color: #3333cc;\">DSATUR RESULTS BY FILE</h1></u><table id=\"customers\" border=\"3\" style=\" width:60%; backgound-color:; color:; font-size:20px; font-weight:bold;\"><tr style=\"background-color:#006666; color:white; font-size:22px;\"><th>FILENAME</th><th>COLORS USED</th><th>ALGORITHM EXECUTED</th></tr>";
     string htmlDoc="<html><head><title>RESULTS DYNAMIC PAGE</title><style>body{font-family:calibri;  background-image: url('https://github.com/ksakkas/ksakkas.github.io/blob/master/img/bg.png?raw=true'); background-repeat: no-repeat; background-attachment: fixed; background-size: cover;;}#customers { font-family: Arial, Helvetica, sans-serif; border-collapse: collapse; width: 100%; } #customers td, #customers th { border: 1px solid #ddd; padding: 8px; } #customers tr:nth-child(even){background-color: #f2f2f2;} #customers tr:hover {background-color: #e6ffff;} #customers tr{background-color: white;} #customers th { padding-top: 12px; padding-bottom: 12px; text-align: left; color: white; }</style></head><body><center>";
     ifstream is;
    is.open("results.txt");
     string line;
     string fn;
     string noc;    //num form colors
     string alc;
    while(is>>line)
    {
         istringstream ss(line);
        getline(ss,fn,',');
        getline(ss,noc,',');
        getline(ss,alc,',');
       if(alc=="DSATUR")
       {
           Graph::dsatur_table+="<tr><td>"+fn+"</td><td>"+noc+"</td><td>"+alc+"</td></tr>";
       }
       else
       {
           Graph::first_fit_table+="<tr><td>"+fn+"</td><td>"+noc+"</td><td>"+alc+"</td></tr>";
       }
    }
    is.close();
    Graph::first_fit_table+="</table><br><hr style=\"border-top:3px solid blue;\">";
    Graph::dsatur_table+="</table><br>";
     string closehtmldoc="</center></body></html>";
     ofstream out;
    out.open("stats.html");
    out<<htmlDoc<<input::statistics_table()<<Graph::first_fit_table<<Graph::dsatur_table<<closehtmldoc<< endl;
    out.close();
     string url=GetExePath()+"//stats.html";

    //code based:https://github.com/vasnastos/Object_Oriented_Programming-CPP-Java/blob/main/Random_Surfer/EXTRA_IMPLEMENTATION/SURFING_RANKING_HTML/driver.cpp
    ShellExecute(NULL,"open", url.c_str(),NULL, NULL, SW_SHOWNORMAL);
}


