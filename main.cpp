#include <iostream>
#include <any>
#include <map>
#include <vector>
#include <fstream>
#include <exception>
#include <string>

using namespace std;

class JSON
{
private:
    map< string, any> object;
    vector< any> vectoR;
public:
    JSON() {};
    JSON(const  string& s, int& number_of_position, bool is_file)
    {
        if (is_file == true)
        {
            parseFile(s);
        }
        else
        {
            parse(s, number_of_position);
        }
    }
    bool is_array()
    {
        if (vectoR.empty())
            return false;
        else
            return true;
    }
    bool is_object()
    {
        if (object.empty())
            return false;
        else
            return true;
    }
    any& operator[](const string& key)
    {
        if (is_object() == true)
            return object[key];
        else
            throw logic_error("is not an object");
    }
    any& operator[](int index)
    {
        if (is_array() == true)
            return vectoR[index];
        else
            throw logic_error("is not an array");
    }
    static  string parse_str(const  string& s, int& number_of_position)
    {
        string result_parsing;
        number_of_position++;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[number_of_position] != '"')
            {
                result_parsing += s[number_of_position];
                number_of_position++;
            }
            else
                return result_parsing;
        }
        throw logic_error("ERROR!");
    }
    static bool parse_bool(const string& s, int& number_of_position)
    {
        bool b = true;
        bool a = true;
        if (s[number_of_position] == 't')
        {
            number_of_position++;
            if (s[number_of_position] == 'r')
            {
                number_of_position++;
                if (s[number_of_position] == 'u')
                {
                    number_of_position++;
                    if (s[number_of_position] == 'e')
                    {
                        a == true;
                    }
                    else b = false;
                }
                else throw b = false;
            }
            else throw b = false;
        }
        else
            if (s[number_of_position] == 'f')
            {
                number_of_position++;
                if (s[number_of_position] == 'a')
                {
                    number_of_position++;
                    if (s[number_of_position] == 'l')
                    {
                        number_of_position++;
                        if (s[number_of_position] == 's')
                        {
                            number_of_position++;
                            if (s[number_of_position] == 'e')
                            {
                                a = false;
                            }
                            else b = false;
                        }
                        else throw b = false;
                    }
                    else throw b = false;
                }
                else throw b = false;
            }
            else throw b = false;

        if (b == true)
        {
            return a;
        }
        else
            throw logic_error("ERROR!");
    }
    static double parse_double(const  string s, int& number_of_position)
    {
        string number;
        number += s[number_of_position];
        number_of_position++;
        for (; number_of_position < s.size(); number_of_position++)
        {
            if (iswdigit(s[number_of_position]) || s[number_of_position] == '.')
            {
                number += s[number_of_position];
            }
            else
            {
                if (s[number_of_position] == ' ' || s[number_of_position] == ',' || s[number_of_position] == ']' || s[number_of_position] == '}') {
                    number_of_position--;
                    return stod(number);
                }
                else throw  logic_error("\nInvalid input format\n");
            }
        }
        throw  logic_error("\nInvalid input format\n");
    }
    static map< string, any> parse_map_object(const  string& s, int& number_of_position)
    {
        map< string, any> result;
        string key;
        any value;
        int state_object = 0;
        number_of_position++;
        for (; number_of_position < s.size(); number_of_position++) {
            if (s[number_of_position] == '"')
            {
                if (state_object == 0)
                {
                    key = parse_str(s, number_of_position);
                    state_object = 1;
                    continue;
                }
                if (state_object == 2)
                {
                    value = parse_str(s, number_of_position);
                    result.insert(pair< string, any>(key, value));
                    state_object = 3;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == ':')
            {
                if (state_object == 1)
                {
                    state_object = 2;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (iswdigit(s[number_of_position]))
            {
                if (state_object == 2)
                {
                    value = parse_double(s, number_of_position);
                    result.insert(pair< string, any>(key, value));
                    state_object = 3;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == 't' || s[number_of_position] == 'f')
            {
                if (state_object == 2)
                {
                    value = parse_bool(s, number_of_position);
                    result.insert(pair< string, any>(key, value));
                    state_object = 3;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == '[')
            {
                if (state_object == 2)
                {
                    JSON buf(s, number_of_position, false);
                    result.insert(pair< string, any>(key, buf));
                    state_object = 3;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == '{')
            {
                if (state_object == 2)
                {
                    JSON buf(s, number_of_position, false);
                    result.insert(pair< string, any>(key, buf));
                    state_object = 3;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == '}')
            {
                if (state_object == 0 or state_object == 3)
                {
                    return result;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == ',')
            {
                if (state_object == 3)
                {
                    state_object = 0;
                    continue;
                }
                else throw  logic_error("\nInvalid input format\n");
            }
        }
        throw  logic_error("\nInvalid input format\n");
    }
    static vector<any> parse_vector(const std::string& s, int& number_of_position)
    {
        vector<any>result;
        any value;
        int stateArray = 0;
        number_of_position += 1;
        for (; number_of_position < s.size(); number_of_position++)
        {
            if (s[number_of_position] == '{')
            {
                if (stateArray == 0) {
                    value = parse_map_object(s, number_of_position);
                    result.push_back(value);
                    stateArray = 1;
                    continue;
                }
                else throw logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == '[')
            {
                if (stateArray == 0) {
                    value = parse_vector(s, number_of_position);
                    result.push_back(value);
                    stateArray = 1;
                    continue;
                }
                else throw logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == '"') {
                if (stateArray == 0) {
                    value = parse_str(s, number_of_position);
                    result.push_back(value);
                    stateArray = 1;
                    continue;
                }
                else throw logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == ',')
            {
                if (stateArray == 1) {
                    stateArray = 0;
                    continue;
                }
                else throw logic_error("\nInvalid input format\n");
            }
            if (iswdigit(s[number_of_position]))
            {
                if (stateArray == 0) {
                    value = parse_double(s, number_of_position);
                    result.push_back(value);
                    stateArray = 1;
                    continue;
                }
                else throw logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == 't' || s[number_of_position] == 'f')
            {
                if (stateArray == 0)
                {
                    value = parse_bool(s, number_of_position);
                    result.push_back(value);
                    stateArray = 1;
                    continue;
                }
                else throw logic_error("\nInvalid input format\n");
            }
            if (s[number_of_position] == ']')
            {
                if (stateArray == 1 or stateArray == 0)
                {
                    return result;
                }
            }
        }
        throw logic_error("\nInvalid input format\n");
    }
    void parse(const string& s, int& number_of_position)
    {
        for (; number_of_position < s.size(); number_of_position++)
        {
            if (s[number_of_position] == '[')
            {
                vectoR = parse_vector(s, number_of_position);
                return;
            }
            if (s[number_of_position] == '{')
            {
                object = parse_map_object(s, number_of_position);
                return;
            }
        }
    }
    void parseFile(const string& path_to_file)
    {
        string line_, file_;
        ifstream File_(path_to_file);
        if (File_.is_open())
        {
            while (!File_.eof())
            {
                getline(File_, line_);
                file_ += line_;
            }
            int position = 0;
            parse(file_, position);
            return;
        }
        else
            throw logic_error("ERROR!");
    }
};
int main()
{
    int p = 0;
    string s = "{\"lastname\" : \"Ivanov\",\"firstname\" : \"Ivan\",\"age\" : 25,\"islegal\" : false,\"marks\" : [    4, 5, 5, 5, 2, 3] ,\"address\" : {\"city\" : \"Moscow\",\"street\" : \"Vozdvijenka\"}}   ";
    JSON a;
    a.parse(s, p);
    cout << any_cast<string>(any_cast<JSON>(a["address"])["city"]) << endl;
    cout << any_cast<double>(any_cast<JSON>(a["marks"])[3]);
    //system("pause");
    return 0;
}
