#ifndef EX_12_30_H
#define EX_12_30_H
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <map>
#include <set>
#include "ex_16_21.h"
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::set;
using std::map;
using std::shared_ptr;
using std::getline;

using line_no = vector<string>::size_type;
class QueryResult;
class TextQuery {
public:
    TextQuery(std::ifstream &is);
    QueryResult query(const string &s) const;
private:
    shared_ptr<vector<string>> file;
    map<string, 
        shared_ptr<set<line_no>>> wm;
};

class QueryResult {
    friend std::ostream& print(std::ostream &os, const QueryResult &qr);
public:
    QueryResult(string s,
                shared_ptr<set<line_no>> p,
                shared_ptr<vector<string>> f):
                sought(s), lines(p), file(f) {}
private:
    string sought;
    shared_ptr<vector<string>> file;
    shared_ptr<set<line_no>> lines;
};
#endif

inline  string make_plural(size_t ctr, const string &word, const string &ending) {
    return (ctr > 1)? word + ending : word;
}

TextQuery::TextQuery(std::ifstream &is) : file(new vector<string>, DebugDelete()){
    string text;
    while (getline(is, text)) {
        file->push_back(text);
        int n = file->size() - 1;
        std::istringstream line(text);
        string word;
        while (line >> word) {
            auto &lines = wm[word];
            if (!lines)
                lines.reset(new set<line_no>, DebugDelete());
            lines->insert(n);
        }
    }
}
QueryResult TextQuery::query(const string &s) const{
    static shared_ptr<set<line_no>> nodata(new set<line_no>, DebugDelete());
    auto loc = wm.find(s);
    if (loc == wm.end())
        return QueryResult(s, nodata, file);
    return QueryResult(s, loc->second, file);
}

std::ostream& print(std::ostream& os, const QueryResult &qr) {
    os << qr.sought << " occurs " << qr.lines->size() <<" "
    <<  make_plural(qr.lines->size(), "time", "s") << endl;
    for (auto num :*qr.lines) {
        os << "\t(line " << num + 1 << ") " <<  *(qr.file->begin() + num) << endl;
    }
    return os;
}

int main()
{
    std::ifstream is("../ch12/infile");
    if (!is)
        cout << "infile Not Found" << endl; 
    TextQuery t(is);
    print(std::cout, t.query("On"));
    return 0;
}