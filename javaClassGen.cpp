#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <set>

using namespace std;

enum Access
{
    public_,protected_,private_
};

set<string> headerKeywords = 
{
    "public","private","protected","abstract","interface","implements","extends"
};

struct Field
{
    Access acc;
    string name;
    string type;
    bool get = false;
    bool set = false;
};

struct Param
{
    string name;
    string type;
};


struct Function
{
    Access acc;
    string name;
    string type;
    vector<Param> params;
};

struct Header
{
    string name;
    string declearation;
};

struct JavaClass
{
    Header header;
    vector<Field> fields;
    vector<Function> functions;
};

Access toAccess(char symbol)
{
    if (symbol == '+') return public_;
    if (symbol == '#') return protected_;
    if (symbol == '-') return private_;
}

string access2String(Access acc)
{
    switch (acc)
    {
        case private_ :
            return "private";
        case protected_ :
            return "protected";
        case public_ :
            return "public";
    }
}

//ChatGPT
string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    if (start == s.size()) {
        return ""; // string is all spaces
    }

    size_t end = s.size() - 1;
    while (end > start && isspace(static_cast<unsigned char>(s[end]))) {
        end--;
    }

    return s.substr(start, end - start + 1);
}

//chatGPT
string removeWhitespace(const string& s) {
    string out;
    out.reserve(s.size());

    for (unsigned char c : s) {
        if (!isspace(c)) {
            out.push_back(c);
        }
    }

    return out;
}

vector<string> slice(string s, char splitter)
{
    vector<string> v;
    size_t comma = s.find(splitter);
    size_t commaLast = 0;
    while(comma != string::npos)
    {
        size_t paramSize = comma - commaLast;
        v.push_back(s.substr(commaLast,paramSize));
        
        commaLast = comma+1;
        comma = s.find(splitter,comma+1);
    }
    
    size_t paramSize = s.size() - commaLast;
    v.push_back(s.substr(commaLast,paramSize));
    return v;
}
    
string removem_(string s)
{
    if (s.find("m_") != string::npos)
        s = s.substr(2,s.size()-2);
    return s;
}

string toPascal(string s)
{
    s[0] = toupper(s[0]);
    return s;
}

string tocaMel(string s)
{
    s[0] = tolower(s[0]);
    return s;
}

Header parseHeader(string line)
{
    Header h; 

    // find whitespace
    if (line.find(" ") == string::npos)
    {
        // not found -> empty or name only
        // build string "public [className]"
        string className = line;
        stringstream ss;
        ss << "public class " << className;

        h.name = className;
        h.declearation = ss.str();
    }
    else
    {
        // found -> probably full declearation

        // slice line into tokens by whitespace and look for first of non-keywords token.
        auto tokens = slice(line,' ');
        for (auto t : tokens)
        {
            if (headerKeywords.find(t) != headerKeywords.end())
            {
                h.name = t;
                break;
            }
        }

        h.declearation = line;
    }

    return h;
}

Field makeField(Access acc, string type, string name)
{
    Field f;
    f.acc = acc;
    f.type = type;
    f.name = name;
    return {acc,name,type};
}

Field parseField(string line)
{
    Access acc = toAccess(line[0]);
    
    size_t colon = line.find_first_of(':');
    size_t bar = line.find_first_of('|');

    size_t nameStart = 1;
    size_t nameSize = colon - nameStart;
    string name = line.substr(nameStart, nameSize); 

    size_t typeStart = colon + 1;
    size_t typeSize = bar == string::npos? line.size() - typeStart : bar - typeStart;
    string type = line.substr(typeStart,typeSize);

    size_t getSetSearchSize = line.size() - bar; 
    bool haveGet = line.find("get",bar+1);
    bool haveSet = line.find("set",bar+1);

    return makeField(acc,type,name,haveGet,haveSet);
}

Param makeParam(string type, string name)
{
    Param p;
    p.type = type;
    p.name = name;
    return p;
}

vector<Param> parseParams(string line)
{
    vector<string> sliced = slice(line,',');
    vector<Param> parsed = {};

    for (string s : sliced)
    {
        size_t colon = s.find(':');
        size_t nameSize = colon;
        size_t typeSize = s.size() - colon +1;

        string name = s.substr(0,nameSize);
        string type = s.substr(colon+1,typeSize);
        parsed.push_back(makeParam(type,name));
    }
    return parsed;
}

Function makeFunction(Access acc, string type, string name, vector<Param> params)
{
    Function f;
    f.acc = acc;
    f.type = type;
    f.name = name;
    f.params = params;
    return f;
}

Function parseFunction(string line)
{
    Function f;
    Access acc = toAccess(line[0]);
    size_t leftBracket = line.find('(');
    size_t rightBracket = line.find(')');
    size_t colon = line.find_last_of(':');

    size_t nameSize = leftBracket - 1;
    string name = line.substr(1,nameSize);

    size_t typeSize = line.size()-colon;
    string type = line.substr(colon+1,typeSize);

    size_t paramsSize = rightBracket - leftBracket;
    string params = line.substr(leftBracket+1,paramsSize-1);

    vector<Param> p;
    if (params != "")
        p = parseParams(params);

    f = makeFunction(acc,type,name,p);
    return f;
}

string fieldString(Field f)
{
    stringstream ss;
    ss << access2String(f.acc) << " " << f.type << " " ;

    //private field always start with "m_"
    if (f.acc == private_)
    {
        if (!(f.name[0] == 'm' &&
            f.name[1] == '_'))
            {
                ss << "m_";
            }
    }

    ss << f.name << ";";

    return ss.str();
}

string paramString(Param p)
{
    stringstream ss;
    ss << p.type << " " << p.name;
    return ss.str();
}

string functionBody (Function f)
{
    stringstream ss;
    ss << (f.type == "void"  || f.type == ""?  "" : ("return null;") );
    return ss.str();
}

string functionString(Function f)
{
    stringstream ss;
    ss << "\t" << access2String(f.acc) << " " << f.type << (f.type == "" ? "" : " ") << f.name << "(";

    for (size_t i = 0; i < f.params.size(); i++)
    {
        ss << paramString(f.params[i]) << (i == f.params.size() - 1 ? "" : ", " );
    }
    
    ss << ")\n";
    ss << "\t{\n";
    ss << "\t\t" << functionBody(f) <<"\n";
    ss << "\t}";
    return ss.str();
}

void writeFile(JavaClass jc, string name)
{
    ofstream outfile(name + ".java");
    
    //header
    outfile << jc.header << endl;
    outfile << "{" << endl;
    
    //field
    vector<Field> publicField;
    vector<Field> privateField;
    vector<Field> protectedField;
    for (Field f : jc.fields)
    {
        switch (f.acc)
        {
            case public_:
                publicField.push_back(f);
                break;
            case protected_:
                protectedField.push_back(f);
                break;
            case private_:
                privateField.push_back(f);
                break;
        }
    }

    for (Field f : publicField)
        outfile << "\t" << fieldString(f) << endl;
    outfile << endl;
    for (Field f : protectedField)
        outfile << "\t" << fieldString(f) << endl;
    outfile << endl;
    for (Field f : privateField)
        outfile << "\t" << fieldString(f) << endl;
    outfile << endl;

    //functions
    for (Function f : jc.functions)
    {
        outfile << functionString(f) << "\n\n";
    }

    outfile << "}" << endl;
    outfile.close();

}


void readfile(string filename)
{
    string inFileName = filename + ".txt";
    ifstream infile(inFileName);

    if (!infile.is_open())
    {
        cout << "Failed to open " << inFileName << "\n";
        return;
    }

    JavaClass package;

    string line = "";
    int lineCount = 1;
    string className = "";
    while (getline(infile,line))
    {
        //trim head and tails
        line = trim(line);

        //first line is the header
        if (lineCount == 1)
        {
            Header header = parseHeader(line);
            package.header = header;

            //finished for the 1st line
            lineCount++;
            continue;
        }

        //remove all whitespace
        line = removeWhitespace(line);

        //other lines
        //looks for "("
        if (line.find("(") == string::npos)
        {
            //not found -> field 
            Field f = parseField(line);
            package.fields.push_back(f);
        }
        else 
        {
            //found -> function
            package.functions.push_back(parseFunction(line));
        }

        lineCount++;
    }
    
    writeFile(package,filename);
    cout << "successfully created [" << filename << ".java] from ["<< filename <<".txt]" << endl;
}

int main(int argc, char** argv)
{
    for (int i = 1; i < argc;i++)
    {
        readfile(argv[i]);
    }
    return 0;
}