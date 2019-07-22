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


void get_picture_info(unordered_map<unsigned long long, info*>& mp);

//void print_result(unordered_map& mp);

int main (int argc, char *argv[]){

//geotag.csvの例
//8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
//tag.csvの例
//8376611070,sketch

    unordered_map<unsigned long long, info*> mp;
    get_picture_info(mp);
    //cout << "print" << endl;
    //print_result(results);
    unsigned long long d = 8376611070;
    printf("key = %llu i->time = %s i->latitude = %lf i->longitude = %lf i->src = %s\n", d, mp[d]->time, mp[d]->latitude, mp[d]->longitude, mp[d]->src);

    return 0;
}


void get_picture_info(unordered_map<unsigned long long, info*>& mp){
//8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
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
    int count = 0;
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
        cout << key << endl;
        buf.erase(buf.end()-4, buf.end()); // .jpgを消す
        i->src = (char *)malloc(buf.size() * sizeof(char));
        strcpy(i->src, buf.c_str());
        mp[key] = i;
      //   printf("key = %lu i->time = %s i->latitude = %lf i->longitude = %lf i->src = %s\n", key, i->time, i->latitude, i->longitude, i->src);
        
        if (count %10000 == 0){
            cout << count++ << endl;
        }else {count+=1;}
    }
}
