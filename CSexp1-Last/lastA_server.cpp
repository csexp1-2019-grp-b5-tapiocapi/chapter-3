#include "lastA_server.h"
#define TEST_INPUT "fiet"

using namespace std;

void get_info(unordered_map<uint64_t, info*>& mp);
void get_tag(unordered_map<string, vector<uint64_t>>& tagmap);
void print_tagmap(string key, unordered_map<string, vector<uint64_t>> tagmap);
void server_start(unordered_map<uint64_t, info*>&, unordered_map<string, vector<uint64_t>>&);
void sort_infos_by_time(unordered_map<string, vector<uint64_t>>&, unordered_map<uint64_t, info*>&, string&, vector<uint64_t>&);
string conv_time_to_string(time_t);
// void remove_not_appear_info(unordered_map<string, vector<uint64_t>>& tagmap, unordered_map<uint64_t, info*>&);


void create_tagDB (unordered_map<string, vector<uint64_t>>& tagmap){
    // unordered_map<uint64_t, info*> mp; // id -> info
    // unordered_map<string, vector<uint64_t>> tagmap; // tag -> id
    // get_info(mp); // geotagからinfoとidの対応のmapを取得
    // uint64_t long d = 8376611070;
    // printf("key = %llu i->time = %s i->latitude = %lf i->longitude = %lf i->src = %s\n", d, mp[d]->time, mp[d]->latitude, mp[d]->longitude, mp[d]->src);
    // cout << "get_tag" << endl;
    get_tag(tagmap); // tagとinfoの対応のmapを作成
    // remove_not_appear_info(tagmap, mp); // 出てこないinfoを削除(サーバ実行時は実行不可)
    // server_start(mp, tagmap);
}
void create_geotagDB (unordered_map<uint64_t, info*>& mp){
    // unordered_map<uint64_t, info*> mp; // id -> info
    // unordered_map<string, vector<uint64_t>> tagmap; // tag -> id
    get_info(mp); // geotagからinfoとidの対応のmapを取得
    // uint64_t long d = 8376611070;
    // printf("key = %llu i->time = %s i->latitude = %lf i->longitude = %lf i->src = %s\n", d, mp[d]->time, mp[d]->latitude, mp[d]->longitude, mp[d]->src);
    // cout << "get_tag" << endl;
    // get_tag(tagmap); // tagとinfoの対応のmapを作成
    // remove_not_appear_info(tagmap, mp); // 出てこないinfoを削除(サーバ実行時は実行不可)
    // server_start(mp, tagmap);
}


void get_info(unordered_map<uint64_t, info*>& mp){
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
    uint64_t key;
    double latitude,longitude; 
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
void get_tag(unordered_map<string, vector<uint64_t>>& tagmap){ 
    //tag.csvの例
    //8376611070,sketch
    ifstream ifs("tag_a.csv");
    // ifstream ifs("tag_test.csv");
    if(ifs.fail()){
        cerr << "fail" << endl;
        return;
    }
    string line, buf;
    uint64_t id;
    while(getline(ifs,line)){
        stringstream ss(line);
        getline(ss, buf, ',');
        id = stoull(buf);
        getline(ss, buf, ',');
        if(tagmap.find(buf) == tagmap.end()){ // map空ならベクターを代入しているが必要なのか?
            vector<uint64_t> v;
            tagmap[buf] = v;
        }
        tagmap[buf].push_back(id);
    }
    // string test = string("car");
    // print_tagmap(test, tagmap);
}
void print_tagmap(string key, unordered_map<string, vector<uint64_t>> tagmap){
    // use debug!!
    // cout << key << "\n";
    for(auto i : tagmap[key]){
        cout << i << "\n";
    }
}
/*
void remove_not_appear_info(unordered_map<string, vector<uint64_t>>& tagmap, unordered_map<uint64_t, info*>& info_map){
    cout << "remove_not_appear_info" << endl;
    ifstream ifs("all_tag.dat"); // すべてのタグが列挙されたファイル
    string tag;
    vector<uint64_t> ids; // idの対応
    unordered_map<uint64_t, bool> check_map; // idが一度でも表示されたかどうか管理するmap
    int count = 0;
    while(getline(ifs, tag)){ // tagをすべて取得
        sort_infos_by_time(tagmap, info_map, tag, ids); // info_mapにすべてのtagをkeyとしていれる
        for(auto id : ids){
            check_map[id] = true;// 100番以内なら存在してもよい
        }
        if(count++ % 10000 == 0){
            cout << count << endl;
        }
    }
    cout << "checked!!" << endl;
    vector<uint64_t> not_use_id;
    for (auto i = check_map.begin(); i != check_map.end(); ++i){
        if(!(i->second)){ // いちどでも表示されるのであれば
            not_use_id.push_back(i->first);
        }
    }
    cout << "write tag_b" << endl;
    ofstream os("tag_b.csv");
    for (auto i = tagmap.begin(); i != tagmap.end(); ++i){  //tagmapからidのベクターをとってくる
        for (auto j = i->second.begin(); j != i->second.end(); ++j){ // idのベクターからidをとる
            if(check_map[*j]){ // idがいちどでも表示されることがあれば
                os << i->first << "," << *j << "\n"; // 書き込む
            }
        }
    }

}
*/
void sort_infos_by_time(unordered_map<string, vector<uint64_t>>& tagmap, unordered_map<uint64_t, info*>& info_map, string& input, vector<uint64_t>& ids){

    map<time_t, uint64_t> m;
    for(auto id : tagmap[input]){
        m[info_map[id]->time] = id; // timeを基準にidをソート
    }
    int count = 0;
    for (auto i = m.begin(); i != m.end(); ++i){
        if(count < 100){
            // cout << i->second << endl;
            ids.push_back(i->second);
        }
        count+=1;
    }
   
}
string fetch_info(unordered_map<uint64_t, info*>& map, unordered_map<string, vector<uint64_t>>& tagmap, string input_tag){
    //http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
    // TODO 今の実装だと入力を受け取ってからsortしていて遅い
    // cout << "input:" << input_tag << endl;
    vector<uint64_t> ids;
    string send_str, top, middle, bottom;
    char time_chr[32];
    struct tm tm;
    info* i;
    sort_infos_by_time(tagmap, map, input_tag, ids);
    for (auto id : ids){
        i = map[id];
        string s = i->src;
        strftime(time_chr, sizeof(time_chr), "%Y/%m/%d/ %a %H:%M:%S", localtime_r(&(i->time), &tm));
        top = i->src[0];
        middle = s.substr(1,4);
        s.erase(s.begin(),s.begin()+5);
        // send_str += to_string(id);
        // send_str += "\n";
        send_str += time_chr;
        send_str += "\n";
        send_str += to_string(i->latitude);
        send_str += "\n";
        send_str += to_string(i->longitude);
        send_str += "\nhttp://farm";
        send_str += top;
        send_str += ".static.flickr.com/";
        send_str += middle;
        send_str += "/";
        send_str += to_string(id);
        send_str += "_";
        send_str += s;
        send_str += ".jpg\n";
    }
    return send_str;
}
void write_geotag(unordered_map<uint64_t, info*>& map){
    // cout << "write geotag" << endl;
    ofstream os("geotag_b.csv");
    for (auto i = map.begin(); i != map.end(); ++i){
        os << i->first << "," << i->second->time << "," << i->second->latitude << "," << i->second->longitude << "," << i->second->src << "\n";
    }
}
void write_tag(unordered_map<string, vector<uint64_t>>& tagmap){
    // cout << "write tag" << endl;
    ofstream os("tag_b.csv");
    for (auto i = tagmap.begin(); i != tagmap.end(); ++i){
        for (auto j = i->second.begin(); j != i->second.end(); ++j){
            os << i->first << "," << *j << "\n";
        }
    }
}
