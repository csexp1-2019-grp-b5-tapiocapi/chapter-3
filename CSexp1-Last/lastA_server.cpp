#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <iomanip>

#define GEOID 10
#define GEOTIME 10
#define MAXITEM 100

using namespace std;
struct info{
    char *time;
    double latitude;
    double longitude;
    char *src;
};

void get_info(unordered_map<unsigned long long, info*>& mp);
void get_tag(unordered_map<string, vector<unsigned long long>>& tagmap);
void print_tagmap(string key, unordered_map<string, vector<unsigned long long>> tagmap);

int main (int argc, char *argv[]){

    //geotag.csvの例
    //8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
    //tag.csvの例
    //8376611070,sketch
    unordered_map<unsigned long long, info*> mp;
    unordered_map<string, vector<unsigned long long>> tagmap;
    get_info(mp);
    // unsigned long long d = 8376611070;
    // printf("key = %llu i->time = %s i->latitude = %lf i->longitude = %lf i->src = %s\n", d, mp[d]->time, mp[d]->latitude, mp[d]->longitude, mp[d]->src);
    get_tag(tagmap);

    return 0;
}


void get_info(unordered_map<unsigned long long, info*>& mp){
    ifstream ifs("geotag_a.csv");
    // ifstream ifs("geotag_test.csv");
    if(ifs.fail()){
        cerr << "fail" << endl;
        return;
    }
    info *i;
    string line, buf;
    unsigned long key;
    double latitude,longitude; 
    // tm t = {};
    while(getline(ifs, line)){
        stringstream ss(line);
        getline(ss, buf, ',');
        key = stoull(buf);
        i = (info *)malloc(sizeof(info));
        getline(ss, buf, ',');
        // istringstream stime(buf);
        // stime >> get_time(&t, "\"%Y-%m-%d %H:%M:%S\"");
        i->time = (char *)malloc(buf.size() * sizeof(char));
        strcpy(i->time, buf.c_str());
        getline(ss, buf, ',');
        latitude = stod(buf);
        i->latitude = latitude;
        getline(ss, buf, ',');
        longitude = stod(buf);
        i->longitude = longitude;
        getline(ss, buf, ',');
        buf.erase(buf.begin(), buf.begin() + 11); // http://farmまで消す
        buf.erase(buf.begin() + 1, buf.begin() + 20); // farmX以下.static.flickr.com/まで消す
        // ↓けつの.jpg消そうとするとメモリリークしちゃう 
        // buf.erase(buf.end()-4, buf.end()); // .jpgを消す
        i->src = (char *)malloc(buf.size() * sizeof(char));
        strcpy(i->src, buf.c_str());
        mp[key] = i;
    }
}
void get_tag(unordered_map<string, vector<unsigned long long>>& tagmap){ 
    //tag.csvの例
    //8376611070,sketch
    ifstream ifs("tag_a.csv");
    // ifstream ifs("tag_test.csv");
    if(ifs.fail()){
        cerr << "fail" << endl;
        return;
    }
    string line, buf;
    unsigned long long id;
    while(getline(ifs,line)){
        stringstream ss(line);
        getline(ss, buf, ',');
        id = stoull(buf);
        getline(ss, buf, ',');
        if(tagmap.find(buf) == tagmap.end()){ // map空ならベクターを代入しているが必要なのか?
            vector<unsigned long long> v;
            tagmap[buf] = v;
        }
        tagmap[buf].push_back(id);
    }
    // string test = string("car");
    // print_tagmap(test, tagmap);
}
void print_tagmap(string key, unordered_map<string, vector<unsigned long long>> tagmap){
    cout << key << "\n";
    for(auto i : tagmap[key]){
        cout << i << "\n";
    }
}
