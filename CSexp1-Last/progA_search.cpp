#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>

#define GEOID 10
#define GEOTIME 10
#define MAXITEM 100

using namespace std;

// 表示する文字を保持する構造体
struct result{
    int time;
    string id;
    string src;
    string time_str;
    string latitude;//緯度
    string longitude;//経度
};
// resultの数
int result_num = 0;
vector<result> results;

int liner_search(char arg[256], vector<result>& results);
void get_picture_info(vector<result>& results);
void print_result(vector<result>& results);

int main (int argc, char *argv[]){

//geotag.csvの例
//8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
//tag.csvの例
//8376611070,sketch

    if (argc != 2){
        cout << "引数の数が間違っています" << endl;
        return -1;
    }
    if(liner_search(argv[1], results) == -1){
        return -1;
    }
    //cout << "print" << endl;
    //print_result(results);
    //cout << "get_pic_info" << endl;
    get_picture_info(results);
    //cout << "print" << endl;
    print_result(results);
    return 0;
}

int liner_search(char arg[256], vector<result>& results_local){
    strcat(arg, "\n");
    FILE *tag_fp;
    char id[10] = "";
    char tag[512] = "";

    if((tag_fp = fopen("tag.csv", "r")) == NULL){
        cout << "file open error in tag.csv" << endl;
        return -1;
    }
    fgets(id, GEOID + 1, tag_fp);
    do {
        fseek(tag_fp, 1, SEEK_CUR);
        fgets(tag, sizeof(tag), tag_fp);
        if(strcmp(tag, arg) == 0){
            result r;
            r.id = id;
            results_local.push_back(r);
            //cout << results_local[result_num].id << "\n";
            result_num++;
        }
    } while (result_num < MAXITEM && (fgets(id, GEOID + 1, tag_fp) != NULL));
    //cout << results_local.size() << endl;
    //cout << result_num << endl;
    return 0;
}

void get_picture_info(vector<result>& results_local){
//8376611070,"2013-01-11 19:49:43",53.656745,-2.083711,http://farm9.static.flickr.com/8050/8376611070_aeb13ec0fe.jpg
    FILE *geotag_fp;
    char id[10] = "";
    char tag[512] = "";

    if((geotag_fp = fopen("geotag.csv", "r")) == NULL){
        cout << "file open error in geotag.csv" << endl;
        return;
    }
    fgets(id, GEOID + 1, geotag_fp);
    //cout << id << endl;
    do {
        fseek(geotag_fp, 1, SEEK_CUR);
        fgets(tag, sizeof(tag), geotag_fp);
        for(int i=0;i<result_num;i++){
            if(results_local[i].id == id){
                char *begin = tag;
                char *end;
                end = strchr(begin, ',');
                *end = '\0';
                results_local[i].time_str = begin;
                begin = ++end;
                end = strchr(begin, ',');
                *end = '\0';
                results_local[i].latitude = begin;
                begin = ++end;
                end = strchr(begin, ',');
                *end = '\0';
                results_local[i].longitude = begin;
                begin = ++end;
                end = strchr(begin, '\n');
                *end = '\0';
                results_local[i].src = begin;

                /*
                cout << results_local[i].time_str << "\n";
                cout << results_local[i].latitude << "\n";
                cout << results_local[i].longitude << "\n";
                cout << results_local[i].src << "\n" << i << endl;
                */
                break;
            }
        }
    } while (fgets(id, GEOID + 1, geotag_fp) != NULL);
}

void print_result(vector<result>& results_local){
    for(int i=0;i<(int)results_local.size();i++){
        cout << results_local[i].time_str << "\n" << results_local[i].latitude << "\n" << results_local[i].longitude << "\n" << results_local[i].src << endl;
    }
    //cout << "end" << endl;
}

