#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <Windows.h>

using namespace std;

struct Teacher 
{
    int id;
    string name;
    int maxHoursPerDay;
    set<int> preferredDays; 
};

struct Course 
{
    int id;
    string name;
    int hoursPerWeek;
    int teacherId;
    vector<int> groupIds;
};

vector<Teacher> readTeachers(const string& filename) 
{
    vector<Teacher> teachers;
    ifstream file(filename);
    string line;

    while (getline(file, line)) 
    {
        istringstream iss(line);
        string token;

        Teacher t;
        getline(iss, token, ','); t.id = stoi(token);
        getline(iss, token, ','); t.name = token;
        getline(iss, token, ','); t.maxHoursPerDay = stoi(token);

        string prefDaysStr;
        getline(iss, prefDaysStr, ',');
        stringstream prefs(prefDaysStr.substr(1, prefDaysStr.size() - 2));
        string day;
        while (getline(prefs, day, ',')) 
        {
            t.preferredDays.insert(stoi(day) - 1); 
        }

        teachers.push_back(t);
    }
    return teachers;
}

vector<Course> readCourses(const string& filename) 
{
    vector<Course> courses;
    ifstream file(filename);
    string line;

    while (getline(file, line)) 
    {
        istringstream iss(line);
        string token;

        Course c;
        getline(iss, token, ','); c.id = stoi(token);
        getline(iss, token, ','); c.name = token;
        getline(iss, token, ','); c.hoursPerWeek = stoi(token);
        getline(iss, token, ','); c.teacherId = stoi(token);

        string groupIdsStr;
        getline(iss, groupIdsStr, ',');
        stringstream ids(groupIdsStr.substr(1, groupIdsStr.size() - 2));
        string gid;
        while (getline(ids, gid, ',')) 
        {
            c.groupIds.push_back(stoi(gid));
        }

        courses.push_back(c);
    }
    return courses;
}

vector<vector<int>> scheduleClasses(const vector<Course>& courses, const vector<Teacher>& teachers) 
{
    vector<vector<int>> schedule(5, vector<int>(8, -1)); 

    for (const auto& course : courses) 
    {
        int hoursLeft = course.hoursPerWeek;
        int teacherIndex = course.teacherId;
        const Teacher& teacher = teachers.at(teacherIndex);

        for (int day : teacher.preferredDays) 
        {
            for (int hour = 0; hour < 8; ++hour) 
            {
                if (hoursLeft > 0 && schedule[day][hour] == -1) 
                {
                    schedule[day][hour] = course.id;
                    --hoursLeft;
                }
            }
        }
    }

    return schedule;
}


void printSchedule(const vector<vector<int>>& schedule) 
{
    cout << "------------ Schedule -----------" << "\n";
    for (int day = 0; day < 5; ++day) 
    {
        cout << "Day " << day << ": ";
        for (int hour = 0; hour < 8; ++hour) 
        {
            if (schedule[day][hour] != -1) 
            {
                cout << "Hour " << hour << ": Course " << schedule[day][hour] << " ";
            }
        }
        cout << "\n";
    }
}


int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
   
    vector<Teacher> teachers = readTeachers("teachers.csv");
    vector<Course> courses = readCourses("courses.csv");

    vector<vector<int>> schedule = scheduleClasses(courses, teachers);

    printSchedule(schedule);

    return 0;
}





