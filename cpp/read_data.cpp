#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

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

void read_data(string fn, int students, int exams)
{
    // ανάγνωση δεδομένων
    vector<set<int>> exam_students(exams + 1);
    fstream fs(fn);
    if (!fs.is_open())
    {
        cerr << "Could not open file " << fn << std::endl;
        exit(-1);
    }
    int student_id = 0;
    string line;
    while (getline(fs, line))
    {
        if (line.empty())
            continue;
        student_id++;
        istringstream iss(line);
        int exam_id;
        while (iss >> exam_id)
        {
            exam_students[exam_id].insert(student_id);
            cout << "Student " << student_id << " is enrolled in exam " << exam_id << endl;
        }
    }
    fs.close();

    cout << "#################################" << endl;

    for (int i = 1; i <= exams; i++)
    {
        cout << "Exam " << i << " Enrolled students: ";
        for (int student_id : exam_students[i]) // range based for
        {
            cout << student_id << " ";
        }
        cout << endl;
    }

    // δημιουργία πίνακα γειτνίασης
    cout << "Adjacency Matrix" << endl;
    int *adj_matrix = new int[exams * exams];
    for (int i = 0; i < exams; i++)
    {
        for (int j = 0; j < exams; j++)
        {
            if (i == j){
                adj_matrix[i * exams + j] = 0; // adj_matrix[i][j]
                continue;
            }
            int c = count_common_elements(exam_students[i + 1], exam_students[j + 1]);
            if (c > 0)
                cout << i + 1 << " " << j + 1 << " " << c << endl;
            adj_matrix[i * exams + j] = c;
        }
    }

    // Υπολογισμός συντελεστή πυκνότητας
    int c = 0;
    for (int i = 0; i < exams; i++)
    {
        for (int j = 0; j < exams; j++)
        {
            if (adj_matrix[i * exams + j] > 0)
            {
                c++;
            }
        }
    }

    double cd = double(c) / double(exams * exams);
    cout << "Conflict Density: " << cd << endl;

    delete[] adj_matrix;
}

int main()
{
    int a;
    cout << "Read Data:"<<endl;
    cout << "Give number:" << endl;
    cout << "1: car-f-92.stu" << endl;
    cout << "2: car-s-91.stu" << endl;
    cout << "3: ear-f-83.stu" << endl;
    cout << "4: hec-s-92.stu" << endl;
    cout << "5: kfu-s-93.stu" << endl;
    cout << "6: lse-f-91.stu" << endl;
    cout << "7: pur-s-93.stu" << endl;
    cout << "8: rye-s-93.stu" << endl;
    cout << "9: sta-f-83.stu" << endl;
    cout << "10: tre-s-92.stu" << endl;
    cout << "11: uta-s-92.stu" << endl;
    cout << "12: ute-s-92.stu" << endl;
    cout << "13: yor-f-83.stu" << endl;
 
    cin >> a;
    
    if(a == 1)
    {
        read_data("../datasets/car_f_92.stu", 18419, 543);
    }
    else if(a == 2)
    {
        read_data("../datasets/car_s_91.stu", 16925, 682);
    }
    else if(a == 3)
    {
        read_data("../datasets/ear_f_83.stu", 1125, 190);
    }
    else if(a == 4)
    {
        read_data("../datasets/hec_s_92.stu", 2823, 81);
    }
    else if(a == 5)
    {
        read_data("../datasets/kfu_s_93.stu", 5349, 461);
    }
    else if(a == 6)
    {
        read_data("../datasets/lse_f_91.stu", 2726, 381);
    }
    else if(a == 7)
    {
        read_data("../datasets/pur_s_93.stu", 30029, 2419);
    }
    else if(a == 8)
    {
        read_data("../datasets/rye_s_93.stu", 11483, 486);
    }
    else if(a == 9)
    {
        read_data("../datasets/sta_f_83.stu", 611, 139);
    }
    else if(a == 10)
    {
        read_data("../datasets/tre_s_92.stu", 4360, 261);
    }
    else if(a == 11)
    {
        read_data("../datasets/uta_s_92.stu", 21266, 622);
    }
    else if(a == 12)
    {
        read_data("../datasets/ute_s_92.stu", 2749, 184);
    }
    else if(a == 13)
    {
        read_data("../datasets/yor_f_83.stu", 941, 181);
    }
}   