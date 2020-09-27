#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iomanip> 


using namespace std;



bool CheckInvalidDate(istream& stream) {
    string InvalidSymvol = "0123456789";
    char Peek = stream.peek();
    for (auto a : InvalidSymvol) {
        if (a == Peek) {
            return false;
        }
    }
    return true;
}


int CheckDate(istream& stream) {
    int Num = 0;
    if (stream.peek() == '-') {
        stream.ignore(1);
        if (CheckInvalidDate(stream)) {
            throw domain_error("111");
        }
        else {
            stream >> Num;
            Num *= -1;
        }
    }
    else {
        stream >> Num;
    }
    return Num;
    }


class Date { // Класс даты.
public:
    Date() { // Конструктор.
        year = 0;
        month = 0;
        day = 0;
    }
    Date (istream& stream) { // Ещё один конструктор.)
         stream >> year;
         stream.ignore(1);
         month = CheckDate(stream);
         stream.ignore(1);
         day = CheckDate(stream);
         if (month < 1 or month > 12) {
             throw domain_error("Month value is invalid: " + to_string(month));
         }
         if (month < 1 or month > 31) {
             throw domain_error("Day value is invalid: " + to_string(day));
         }
    }
    stringstream PrintDate() {
        stringstream ss;
        ss << setfill('0') << setw(4) << year << "-"
           << setfill('0') << setw(2) << month << "-"
           << setfill('0') << setw(2) << day;
        return ss;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year;
    int month;
    int day;
};


bool operator==(const Date& date1, const Date& date2) { // Проверяем, равны ли 2 даты.
    if (date1.GetYear() == date2.GetYear() and date1.GetMonth() == date2.GetMonth() and date1.GetDay() == date2.GetDay()) {
        return true;
    }
    return false;
}


struct DateAndEvent {
    Date date;
    string Event;
};


bool DelEvent(vector<DateAndEvent>& Vec, const Date& date, const string& event) {
    int k = 0;
    for (auto i : Vec) {
        k++;
        if (i.date == date and i.Event == event) {
            Vec.erase(Vec.begin() + k);
            return true;
        }
    }
    return false;
}


int DelAllEvent(vector<DateAndEvent>& Vec, const Date& date) {
    int k = 0, QuantiObj = 0;
    for (auto i : Vec) {
        k++;
        if (i.date == date) {
            Vec.erase(Vec.begin() + k);
            QuantiObj++;
        }
    }
    return QuantiObj;
}


void PrintEvents(const vector<DateAndEvent>& Vec, const Date& date) {
    vector<string> VectorString;
    for (auto item : Vec) {
        if (item.date == date) {
            VectorString.push_back(item.Event);
        }
    }
    sort(VectorString.begin(), VectorString.end());
    for (auto i : VectorString) {
        cout << i << endl;
    }
}


bool CheckingAdd(const vector<DateAndEvent>& VectorEvent, const Date& date, const string& Event) { // Проверяем, есть ли уже это событие в базе.
    for (auto i : VectorEvent) {
        if (i.date == date and i.Event == Event) {
            return false; // Если такое событие уже есть в базе - возвращаем false.
        }
    }
    return true; // Если такого события ещё нет в базе - возвращаем true.
}
bool operator<(Date& dt1, Date& dt2) {
    if (dt1.GetYear() < dt2.GetYear()) {
        return true;
    }
    else if (dt1.GetYear() > dt2.GetYear()) {
        return false;
    }
    else {
        if (dt1.GetMonth() < dt2.GetMonth()) {
            return true;
        }
        else if (dt1.GetMonth() > dt2.GetMonth()) {
            return false;
        }
        else {
            if (dt1.GetDay() < dt2.GetDay()) {
                return true;
            }
            
            
        }
    }
    return false;
}

ostream& operator<<(ostream& stream, stringstream& ss) {
    string tt;
    while (ss) {
        ss >> tt;
        stream << tt;
    }
    return stream;
}

class Database { // Класс базы, ключевой объект - вектор DataAndEvent.
public:
    Database() {};
    void AddEvent(const Date& date, const string& event) {
        if (CheckingAdd(VectorEvent, date, event)) {
            VectorEvent.push_back({ date, event });
        }
    }
    void DeleteEvent(const Date& date, const string& event) {
        if (DelEvent(VectorEvent, date, event)) {
            cout << "Deleted successfully" << endl;
        }
        else {
            cout << "Event not found" << endl;
        }
    }
    void  DeleteDate(const Date& date) {
        cout << "Deleted " + to_string(DelAllEvent(VectorEvent, date)) + " events" << endl;
    }

    void Find(const Date& date) const {
        PrintEvents(VectorEvent, date);
    }

    void Print()  {

        sort(VectorEvent.begin(), VectorEvent.end(), [](DateAndEvent& dt1, DateAndEvent& dt2) {
            return dt1.date < dt2.date;
            });
        sort(VectorEvent.begin(), VectorEvent.end(), [](DateAndEvent& dt1, DateAndEvent& dt2) {
            if (dt1.date == dt2.date) {
                return dt1.Event < dt2.Event;

            }
            return true;
            });
        stringstream ss;
        for (auto& item : VectorEvent) {
            ss = item.date.PrintDate();
            cout << ss << " " << item.Event;
        }
        /*vector<string> VectorAllEvents;
        for (auto& i : VectorEvent) {
            VectorAllEvents.push_back(i.Event);
        }
        (VectorAllEvents.begin(), VectorAllEvents.end());
        for (auto item : VectorAllEvents) {
            cout << item << endl;
        }*/
    }
private:
    vector<DateAndEvent> VectorEvent;
};


int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        stringstream ss;
        ss << command;
        ss >> command;
        string AddData;
        while (command[0] == 10) { // 10 == '/n'.
            string CommandNoN;
            for (int a = 1; a < command.size(); a++) {
                CommandNoN += command[a];
            }
            command = CommandNoN;
            }
        try {
            if (command == "Add") {
                Date date = Date(ss);
                string Event;
                ss.ignore(1);
                getline(ss, Event);
                db.AddEvent(date, Event);
            }
            else if (command == "Find") {
                Date date = Date(ss);
                db.Find(date);
            }
            else if (command == "Del") {
                Date date = Date(ss);
                if (ss.peek() == ' ') {
                    string event;
                    ss >> event;
                    db.DeleteEvent(date, event);
                }
                else {
                    db.DeleteDate(date);
                }

            }
            else if (command == "Print") {
                db.Print();
            }
            else {
                cout << "Unknown command :" + command << endl;
                getline(ss, command);
            }
        }
        catch (domain_error& exc) {
            cout << exc.what() << endl;
            return 0;
        } 
    }

    return 0;
}
/*int main()
{
    while (true) {
        string Operat;
        cin >> Operat;
        string AddData;
        if (Operat == "Add") {
            cout << "Add OK" << endl;
            string AddDataTwo;
            Data date = Data(cin);
            cin >> AddDataTwo;
            AddData = to_string(date.hour) + "-" + to_string(date.month) + "-" + to_string(date.day) + " " + AddDataTwo;
        }
        if (CheckingAdd(AddData) == false) {
            output << AddData;
        }
        cout << AddData;
        
    }
    return 0;
}
*/