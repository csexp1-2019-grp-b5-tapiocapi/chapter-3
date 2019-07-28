#ifndef LASTA_SERVER_H_
#define LASTA_SERVER_H_

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
#include <map>
#include <vector>
#include <limits.h>

using namespace std;
struct info{
    time_t time;
    double latitude;
    double longitude;
    char *src;
};
extern unordered_map<string, vector<unsigned long >> tag_map;
extern unordered_map<unsigned long, info*> info_map;

void create_tagDB (unordered_map<string, vector<unsigned long>>& tagmap);
void create_geotagDB (unordered_map<unsigned long, info*>&);
void get_info(unordered_map<unsigned long, info*>& mp);
void get_tag(unordered_map<string, vector<unsigned long>>& tagmap);
void print_tagmap(string key, unordered_map<string, vector<unsigned long>> tagmap);
string fetch_info(unordered_map<unsigned long, info*>&, unordered_map<string, vector<unsigned long>>&, string);
void sort_infos_by_time(unordered_map<string, vector<unsigned long>>&, unordered_map<unsigned long, info*>&, string&, vector<unsigned long>&);
void remove_not_appear_info(unordered_map<string, vector<unsigned long>>& tagmap, unordered_map<unsigned long, info*>&);
#endif /* LIBSERVER_H_ */
