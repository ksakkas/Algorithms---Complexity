#include "greedy.hpp"

int main()
{
    int op;                                                                          
    string actualname=fileselection();                  //check file
    std::string fn="../datasets/"+actualname;          //add path folder
    input in(number_of_vertices(actualname));         //return from file number of vertices
    in.openfile(fn);                                 //read data and print 
    Graph g(in);                                    //object class
    while (true)                                    
    {
        op = menu();                              //print menu
        switch (op)                             
        {
        case 1:
         in.stats();                            //statistics
           break;
        case 2:
           in.save_statistics();             //save statistics
           break;
        case 3:
         g.greedyColoring();                //first fit
         break;
        case 4:
         g.DSatur();                       //dsatur
           break;
        case 5:
         g.Show();                       //html page
         break;
        case 6:
            exit(EXIT_SUCCESS);        //exit
        default:
            break;
        }
    }
}