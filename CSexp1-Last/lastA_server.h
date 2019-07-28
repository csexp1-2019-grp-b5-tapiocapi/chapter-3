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
extern unordered_map<string, vector<uint64_t >> tag_map;
extern unordered_map<uint64_t, info*> info_map;

void create_tagDB (unordered_map<string, vector<uint64_t>>& tagmap);
void create_geotagDB (unordered_map<uint64_t, info*>&);
void get_info(unordered_map<uint64_t, info*>& mp);
void get_tag(unordered_map<string, vector<uint64_t>>& tagmap);
void print_tagmap(string key, unordered_map<string, vector<uint64_t>> tagmap);
string fetch_info(unordered_map<uint64_t, info*>&, unordered_map<string, vector<uint64_t>>&, string);
void sort_infos_by_time(unordered_map<string, vector<uint64_t>>&, unordered_map<uint64_t, info*>&, string&, vector<uint64_t>&);
void remove_not_appear_info(unordered_map<string, vector<uint64_t>>& tagmap, unordered_map<uint64_t, info*>&);
#endif /* LIBSERVER_H_ */
