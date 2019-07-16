#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string.h>

#define GEOID 10
#define GEOTIME 10
#define MAXITEM 100

using namespace std;

unordered_map<long long, string> mp;

// resultの数
int result_num = 0;

void get_picture_info(unordered_map<long long, string>& mp);

//void print_result(unordered_map& mp);

int main (int argc, char *argv[]){

//geotag.csvの例
//8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
//tag.csvの例
//8376611070,sketch

    if (argc != 2){
        cout << "引数の数が間違っています" << endl;
        return -1;
    }
    get_picture_info(mp);
    cout << mp[8376611070] << endl;
    //cout << "print" << endl;
    //print_result(results);

    return 0;
}


void get_picture_info(unordered_map<long long, string>& mp){
//8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
    FILE *geotag_fp;
    char info[512] = "";
    int count = 0;

    if((geotag_fp = fopen("geotag.csv", "r")) == NULL){
        cout << "file open error in geotag.csv" << endl;
        return;
    }
    fgets(info, sizeof(info), geotag_fp);
    cout << info << "\n" << endl;
    //cout << id << endl;
    do {
        long long key;
        char *begin = info;
        char *end;
        end = strchr(begin, ',');
        *end = '\0';
        key = strtoq(begin, NULL, 10);
        begin = ++end;
        end = strchr(begin, ',');
        *end = '\0';
        mp[key] = begin;
 //        begin = ++end;
 //        end = strchr(begin, ',');
 //        *end = '\0';
//        res.latitude = begin;
//        begin = ++end;
//        end = strchr(begin, ',');
//        *end = '\0';
 //       res.longitude = begin;
//        begin = ++end;
//        end = strchr(begin, '\n');
//        *end = '\0';
//        res.src = begin;
        count++;
        if(count%10000 == 0){
            cout<<count<<endl;
        }
    } while (fgets(info, sizeof(info), geotag_fp) != NULL);
}

void print_result(unordered_map<long long, string>& mp){
    cout << flush;
}

