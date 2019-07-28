#include "ex_13_42.h"


inline  string make_plural(size_t ctr, const string &word, const string &ending) {
    return (ctr > 1)? word + ending : word;
}

TextQuery::TextQuery(std::ifstream &is) : file(new StrVec){
    string text;
    while (getline(is, text)) {
        file->push_back(text);
        int n = file->size() - 1;
        std::istringstream line(text);
        string word;
        while (line >> word) {
            auto &lines = wm[word];
            if (!lines)
                lines.reset(new set<line_no>);
            lines->insert(n);
        }
    }
}
QueryResult TextQuery::query(const string &s) const{
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
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

void runQueries(std::ifstream &infile) {
    TextQuery tq(infile);

    while (true) {
        cout << "enter word to look for, or q to quit: ";
        string s;
        if (!(std::cin >> s) || s == "q") break;
        print(cout, tq.query(s)) <<endl;
    }
}

int main() {
    std::ifstream infile("../ch12/infile");
    if (!infile.is_open())
        return -1;
    runQueries(infile);
    return 0;
}