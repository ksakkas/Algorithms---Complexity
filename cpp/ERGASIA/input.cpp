#include "input.hpp"
#include <thread>

vector<string> files;
vector <int> file_exams;
string menutab="\n\n";
string message="\n\n************** Statistics **************";
string center="\t\t\t";

//-------------------------------------------------------------------------------------
//class functions
input::input(int number_of_exams)       //input funct
{
   for(int i=0;i<number_of_exams;i++)
   {
       this->exam_students.push_back(set <int>());
       this->adj.push_back(vector <int>());
   }
}

void input::openfile(string filename)       //open file
{
    int i;
    // read data
    stringstream  ss(filename);
    getline(ss,this->filename,'/');
    getline(ss,this->filename,'/');
    getline(ss,this->filename,'/');
    fstream fs;
    fs.open(filename,ios::in);
    if (!fs.is_open())
    {
        cerr << "Could not open file " << filename << endl;
        exit(-1);
    }
    int student_id = 0;
    string line, word;
    while (getline(fs, line))
    {
        if (line.empty())
            continue;
        student_id++;
        istringstream iss(line);
        int exam_id;
        while (iss>>exam_id)
        {
            this->exam_students[exam_id-1].insert(student_id);
        }
    }
    fs.close();
    cout<<endl<<"Selected File:"<<this->filename<<endl <<"File Exams:"<<this->exam_students.size();
    this->fill_adjacency();
}

void input::printexams()           //print exams
{
    for(auto &x:this->exams)
    {
        cout<<x<<endl;
    }
}

//Save to file
void input::save_to_file(string filename,double density,int max,int median,int min,double mean,double cv)
{
    ifstream is;
    is.open("statistics.txt");
    vector <string> data;
    string line;
    while(is>>line)
    {
        data.push_back(line);
    }
    is.close();
    string selectedfile;
    for(auto &x:data)
    {
        stringstream ss(x);
         getline(ss,selectedfile,',');
         if(selectedfile==filename)
         {
             return;
         }
    }
    ofstream out;
    out.open("statistics.txt",ios::app);
    out.precision(4);
    out<<filename<<","<<density<<","<<max<<","<<median<<","<<min<<","<<mean<<","<<cv<<endl;
    out.close();
}

//Size of input
int input::size()const
{
    return this->adj.size();
}

//Adjance of input
bool input::is_adjacence(int s,int t)const
{
    return this->adj[s][t]!=0;
}

//fill_adjacency of input
void input::fill_adjacency()
{
    const int size=this->exam_students.size();
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            if(i==j)
            {
                adj[i].push_back(0);
                continue;
            }
            adj[i].push_back(count_common_elements(this->exam_students.at(i),this->exam_students.at(j)));
        }
    }
}

double input::density()
{
    cout<<message<<endl;
    int counter=0;
    const int size=this->exam_students.size();
    for(int i=0;i<size;i++)
    {
        counter+=count_if(this->adj[i].begin(),this->adj[i].end(),[](int &a) {return a!=0;});
    }
    return (double)counter/pow(size,2);
}

string input::getFilename()const{
    return this->filename;
}

//**********************************Statistics******************************************
void input::stats()
{
    int max,min,median;
    double mean,cv,dens;
    int *sum;
    const int size=this->exam_students.size();
    sum=new int[size];
    for(int i=0;i<size;i++)
    {
        sum[i]=count_if(this->adj[i].begin(),this->adj[i].end(),[](int &a) {return a!=0;});
    }
    max=*max_element(sum,sum+size);
    min=*min_element(sum,sum+size);
    mean=accumulate(sum,sum+size,0)/(size*1.0);
    sort(sum,sum+size);
    median=sum[size/2];
    cv=0.0;
    for(int i=0;i<size;i++)
    {
        cv+=pow((double)sum[i]-mean,2);
    }
    cv/=size-1;
    cv=(sqrt(cv)/mean)*100.0;
    dens=this->density();
    cout.precision(4);
    cout<<"\tDensity:"<<dens<<endl;
    cout<<"\tMax:"<<max<<endl;
    cout<<"\tMin:"<<min<<endl;
    cout<<"\tMedian:"<<median<<endl;
    cout<<"\tMean:"<<mean<<endl;
    cout<<"\tCoeffience of variation:"<<cv<<endl;
    string date=__DATE__;
    string time=__TIME__;
    statsstring=center+date+"--"+time+"\n";
    statsstring+=center+"©KSAKKAS 2020\n"+center+"GRAPH COLORING PROBLEM\n"+center+"STATISTICS PER FILE\n"+center+"FILE USED:"+this->filename+"\n";
    string header="############################# STATISTICS #############################";
    statsstring+=header+"\n";
    statsstring+="\tDensity:"+to_string(dens)+"\n";
    statsstring+="\tMax:"+to_string(max)+"\n";
    statsstring+="\tMin:"+to_string(min)+"\n";
    statsstring+="\tMedian:"+to_string(median)+"\n";
    statsstring+="\tMean:"+to_string(mean)+"\n";
    statsstring+="\tCoeffience of variation:"+to_string(cv)+"\n";
    this->save_to_file(this->filename,dens,max,median,min,mean,cv);
    for(int i=0;i<header.length();i++)
    {
        statsstring+="#";
    }
    statsstring+"\n";
    delete []sum;
    for_each(message.begin(),message.end(),[](char &k) {cout<<"*";});
    cout<<endl<<endl;
}

//*****************************************Save Statistics*************************************
void input::save_statistics()
{
   if(this->statsstring.empty()) {cout<<endl<<"****************Warning********************"<<endl<<"The statistics should be displayed first"<<endl;  return;}
   fstream fs;
   fs.precision(4);
   fs.open("statistics.out",ios::out);
   fs<<this->statsstring<<endl;
   fs.close();
   vector <string> output{"********","File statistics"," created ","!!!!","********"};
   for(int i=0,t=output.size();i<t;i++)
   {
       cout<<output.at(i);
       this_thread::sleep_for(chrono::seconds(1));
   }
}


//-------------------------------------------------------------------------
//functions 

int count_common_elements(set<int> s1, set<int> s2)
{
    int c = 0;
    for (int x : s1) // range based for
    {
        for (int y : s2)
        {
            if (x == y)
            {
                c++;
                break;
            }
        }
    }
    return c;
}

int menu()
{
    cout<<menutab;
    int option;
    cout<<"------------MENU-------------"<<endl; 
    cout<<"1-Print Statistics"<< ::endl;
    cout<<"2-Save statistics to file"<<endl;
    cout<<"3-First Fit"<<endl;
    cout<<"4-DSatur"<<endl;
    cout<<"5-Show Algorithms execution stats"<<endl;
    cout<<"6-Exit"<<endl;
    cout<<"Select an option:";
    cin>>option;
    return option;
}

void source()
{
    fstream fs;
    fs.open("input.txt",ios::in);
    string line,word;
    while (getline(fs, line))
    {
        stringstream ss(line);
        getline(ss,word,',');
        files.push_back(word);
        getline(ss,word,',');
        file_exams.push_back(stoi(word));
    }
    fs.close();
}

string fileselection()
{
    int a;
    if (files.empty())
    {
      source();
    }
    for (int i = 0; i < files.size(); i++)
    {
        cout << i + 1 << "." << files.at(i) << endl;
    }
    cout << "Select your input file:";
    cin >> a;
    return files.at(a-1);
}

int number_of_vertices(string fn)
{
    int i=0;
    for(auto &x:files)
    {
        if(x==fn)
        {
           return file_exams[i];
        }
        i++;
    }
    return -1;
}

string input::statistics_table()
{
    string htmlcontent="<marquee><h1 style=\"font-family:calibri; color:#cc0066; background-color:white;\"><b>Τμήμα Πληροφορικής και Τηλεπικοινωνιών</b></h1></marquee><u><h1 style=\"margin-bottom:35px; color: #3333cc; \">STATISTICS TABLE BY FILE</h1></u><table id=\"customers\" border=\"3\" style=\" width:60%; backgound-color:; color:; font-size:20px; font-weight:bold;\"><tr style=\"background-color:#006666; color:white; font-size:22px;\"><td>FILENAME</td><td>DENSITY</td><td>MAX</td><td>MEDIAN</td><td>MIN</td><td>MEAN</td><td>CV</td></tr>";
    ifstream is;
    is.open("statistics.txt");
    string line,word;
    vector <string> data;
    while(is>>line)
    {
        data.clear();
        stringstream ss(line);
        while(getline(ss,word,','))
        {
            data.push_back(word);
        }
        htmlcontent+="<tr>";
        for(auto &x:data)
        {
           htmlcontent+="<td>"+x+"</td>";
        }
        htmlcontent+="</tr>";
    }
    is.close();
    htmlcontent+="</table><br><hr style=\"border-top:3px solid blue;\">";
    return htmlcontent;
}

