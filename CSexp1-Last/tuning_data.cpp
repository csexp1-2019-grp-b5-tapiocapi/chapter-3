#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <cctype>
#include <algorithm>

#define ID_NUM 10

using namespace std;

int read_geotag(unordered_map<unsigned long long, vector<string>>& map);
int read_tag(unordered_map<unsigned long long, vector<string>>&);
void write_tag(unordered_map<unsigned long long, vector<string>>& map);
void write_geotag(string contents);

int main (){
    unordered_map<unsigned long long, vector<string>> v;
    //geotag.csvの例
    //8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
    //tag.csvの例
    //8376611070,sketch
    if (read_tag(v) == -1) { return -1; }
    write_tag(v);
    read_geotag(v);
}

int read_tag(unordered_map<unsigned long long, vector<string>>& map){

    // cout << "read tag" << endl;
    // ifstream ifs("tag_test.csv");
    ifstream ifs("tag.csv");
    if (ifs.fail()){
        cerr << "fail" << endl;
        return -1;
    }

    string s;
    unsigned long long id;
    while (getline(ifs, s)){
        stringstream ss(s);
        string buf;
        getline(ss, buf, ',');
        id = stoull(buf);
        getline(ss, buf, ',');
        if (buf.empty()){
            continue;
        }
        if (map.find(id) == map.end() ){
            vector<string> v;
            map[id] = v;
        }
        map[id].push_back(buf);
    }
    return 0;
}

int read_geotag(unordered_map<unsigned long long, vector<string>>& map){
    // cout << "read geotag" << endl;
    // ifstream ifs("geotag_test.csv");
    ifstream ifs("geotag.csv");

    if (ifs.fail()){
        cerr << "fail" << endl;
        return -1;
    }

    string s, contents;
    unsigned long long id;
    while (getline(ifs, s)){
        stringstream ss(s);
        string buf;
        getline(ss, buf, ',');
        if (!all_of(buf.cbegin(), buf.cend(), [](char ch) { return isdigit(ch);})){
            cout << "invalid id" << endl;
            continue;
        }
        id = stoull(buf);
        if(map.find(id) != map.end()){
            contents.append(s);
            contents.append("\n");
        }
    }
    //contents.erase(--contents.end());
    write_geotag(contents);

    return 0;

}
void write_tag(unordered_map<unsigned long long, vector<string>>& map){
    cout << "write tag" << endl;
    ofstream os("tag_a.csv");
    for (auto i = map.begin(); i != map.end(); ++i){
        for (auto j = i->second.begin(); j != i->second.end(); ++j){
            os << i->first << "," << *j << "\n";
        }
    }
    os.close();
}
void write_geotag(string contents){
    cout << "write geotag" << endl;
    ofstream os("geotag_a.csv");
    os << contents;
    os.close();
}

