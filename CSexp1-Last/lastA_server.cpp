#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unordered_map>
#include <vector>
#include <limits.h>

#define TEST_INPUT "fiet"

using namespace std;
struct info{
    // メモリに乗らないならlongあたりにしたほうがよい。乗るならcharのままが良い?
    time_t time;
    float latitude;
    float longitude;
    //double latitude;
    //double longitude;
    char *src;
};

void get_info(unordered_map<unsigned long, info*>& mp);
void get_tag(unordered_map<string, vector<unsigned long>>& tagmap);
void print_tagmap(string key, unordered_map<string, vector<unsigned long>> tagmap);
void server_start(unordered_map<unsigned long, info*>&, unordered_map<string, vector<unsigned long>>&);
bool info_cmp(info* left, info* right){
    return left->time < right->time;
}

int main (int argc, char *argv[]){

    cout << ULONG_MAX << endl;
    unordered_map<unsigned long, info*> mp;
    unordered_map<string, vector<unsigned long>> tagmap;
    get_info(mp);
    // unsigned long long d = 8376611070;
    // printf("key = %llu i->time = %s i->latitude = %lf i->longitude = %lf i->src = %s\n", d, mp[d]->time, mp[d]->latitude, mp[d]->longitude, mp[d]->src);
    get_tag(tagmap);

    // server_start(mp, tagmap);

    return 0;
}


void get_info(unordered_map<unsigned long, info*>& mp){
    //geotag.csvの例
    //8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
    ifstream ifs("geotag_a.csv");
    // ifstream ifs("geotag_test.csv");
    if(!ifs){
        cerr << "fail" << endl;
        return;
    }
    info *i;
    string line, buf;
    unsigned long key;
    float latitude,longitude; 
    // double latitude,longitude; 
    tm t = {};
    // int count = 0;
    while(getline(ifs, line)){
        stringstream ss(line);
        getline(ss, buf, ',');
        key = stoull(buf);
        i = (info *)malloc(sizeof(info));
        getline(ss, buf, ',');
        istringstream stime(buf);
        stime >> get_time(&t, "\"%Y-%m-%d %H:%M:%S\"");
        i->time = mktime(&t);
        // i->time = (char *)malloc(buf.size() * sizeof(char) + 1);
        // strcpy(i->time, buf.c_str());
        getline(ss, buf, ',');
        latitude = stof(buf);
        i->latitude = latitude;
        getline(ss, buf, ',');
        longitude = stof(buf);
        i->longitude = longitude;
        getline(ss, buf, ',');
        buf.erase(buf.begin(), buf.begin() + 11); // http://farmまで消す
        buf.erase(buf.begin() + 1, buf.begin() + 20); // farmX以下.static.flickr.com/まで消す
        buf.erase(buf.begin() + 1 + 4, buf.begin() + buf.find("_") + 1); // 4桁飛ばしてIDを消す
        buf.erase(buf.end()-4, buf.end()); // .jpgを消す
        i->src = (char *)malloc(buf.size() * sizeof(char) + 1);
        strcpy(i->src, buf.c_str());
        mp[key] = i;
        /// printf("key = %lu i->time = %d i->latitude = %lf6 i->longitude = %lf6 i->src = %s\n", key, (int)mp[key]->time, mp[key]->latitude, mp[key]->longitude, mp[key]->src);
        // cout << "latitude" << i->latitude << endl;
    }
}
void get_tag(unordered_map<string, vector<unsigned long>>& tagmap){ 
    //tag.csvの例
    //8376611070,sketch
    ifstream ifs("tag_a.csv");
    // ifstream ifs("tag_test.csv");
    if(ifs.fail()){
        cerr << "fail" << endl;
        return;
    }
    string line, buf;
    unsigned long id;
    while(getline(ifs,line)){
        stringstream ss(line);
        getline(ss, buf, ',');
        id = stoull(buf);
        getline(ss, buf, ',');
        if(tagmap.find(buf) == tagmap.end()){ // map空ならベクターを代入しているが必要なのか?
            vector<unsigned long> v;
            tagmap[buf] = v;
        }
        tagmap[buf].push_back(id);
    }
    // string test = string("car");
    // print_tagmap(test, tagmap);
}
void print_tagmap(string key, unordered_map<string, vector<unsigned long>> tagmap){
    // use debug!!
    cout << key << "\n";
    for(auto i : tagmap[key]){
        cout << i << "\n";
    }
}
void server_start(unordered_map<unsigned long, info*>& map, unordered_map<string, vector<unsigned long>>& tagmap){
    // #TODO server



    string input_tag = string(TEST_INPUT);
    // TODO 今の実装だと入力を受け取ってからsortしていて遅い
    vector<unsigned long> IDs = tagmap[input_tag];
    vector<info*> infos;
    for(auto id : IDs){
        infos.push_back(map[id]);
    }
    sort(infos.begin(), infos.end(), info_cmp);


}
