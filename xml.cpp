//#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

vector<std::string>& split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

struct Node {
    string tag;
    map<string, string> attrs;
    map<string,unique_ptr<Node>> children;
};

class Parser {
    vector<string> xml;
    int xmlIndex;
    unique_ptr<Node> root;

public:
    Parser(vector<string> input):
        xml(input),
        xmlIndex(0),
        root(nullptr)
    {
    }
    static char consumeChar(stringstream& ss, char x){
        char y;
        ss >> y;
        assert(x == y);
        return x;
    }
    void parse(){
        root = make_unique<Node>();
        root->tag = "root";
        root->children = parseNodes();
    }
    string query(string q){
        //tag1.tag2~name
        vector<string> p = split(q,'.');
        string last = p.back();
        vector<string> key_value = split(last, '~');
        p.pop_back();
        p.push_back(key_value[0]);

        Node* tmp = root.get();
        for(auto const& node: p) {
            if( tmp->children.find(node) != tmp->children.end()){
                tmp = tmp->children.at(node).get();
            } else {
                return "Not Found!";
            }
        }

        if(tmp->attrs.find(key_value[1]) == tmp->attrs.end()) {
            return "Not Found!";
        } else {
            return tmp->attrs.at(key_value[1]);
        }
    }

private:
    map<string, string> parseAttrs(stringstream& ss){
        map<string, string> ret;
        string key;
        string value;
        char ch;
        while(ss.good()){
            ss >> key >> ch >> value;
            if(ch != '=') {cerr << "WTF" << endl;}
            value = value.substr(1, value.size() - 2);
            auto attr = make_pair(key,value);
            ret.insert(attr);
        }
        return ret;
    }


    unique_ptr<Node> parseNode() {
        string str = xml[xmlIndex++];
        assert(str.back() == '>');
        str.pop_back();
//        assert(str.front() == '<');
        stringstream ss(str);
        while(ss.get() == ' ') {}//remove space until > (good faith..)
        if(ss.peek() == '/') {
            cerr << "seeing close tag in open" << endl;
        }
        string openTag;
        ss >> openTag;
        //"key=value,key=value,...
        std::map<string, string> attributes = parseAttrs(ss);

        unique_ptr<Node> ret = make_unique<Node>();
        ret->attrs = attributes;
        ret->tag = openTag;

        ret->children = parseNodes();

        string closeNode = xml.at(xmlIndex++);
        assert(closeNode.find(openTag) != std::string::npos);//todo
        return ret;

    }

    map<string,unique_ptr<Node> > parseNodes(){
        map<string,unique_ptr<Node>> ret;
        while ( xmlIndex < xml.size() && xml.at(xmlIndex).find("</") ==std::string::npos) {
            unique_ptr<Node> x = parseNode();
            ret.insert(make_pair(x->tag,move(x)));
        }
        return ret;
    }
};

int main(int argc, char *argv[]) {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */

    int n;
    int q;
    cin >> n >> q;
    cin.ignore();
    string line;
    string query;
    vector<string> input;
    //vector<string> queries;
    for(int i = 0; i<n;i++){
        std::getline(std::cin, line);
        input.push_back(line);
    }
    Parser parser(input);
    parser.parse();

    for(int i = 0; i< q; i++){
        std::getline(std::cin, query);
        cout << parser.query(query) << endl;
    }
    return 0;
}



