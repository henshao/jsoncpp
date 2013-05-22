#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <json/json.h>
#include <sstream>
 
using namespace std;
 
int64_t getCurrentTime()
{
    struct timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000000LL + tval.tv_usec);
}
 
char * key = "abcdef";
char * str = "abcdefghijklmnopqrstuvwxyz'fuck CPC";
 
void test()
{
    int doc_count = 40;
    int outer_field_count = 80;
 
    std::vector<std::string> vec;
 
    for( size_t j=0 ; j<outer_field_count; ++j )
    {
        std::stringstream ss;
        ss << key << j;
        vec.push_back(ss.str());
    }
 
    int64_t start_time = getCurrentTime();
 
    Json::Value res;
    Json::Value& auction_info = res["main"];
    Json::Value& common_info  = auction_info["auctions"];
 
    for(int i=0; i<doc_count; ++i)
    {
        Json::Value& auc_info = common_info[i];
 
        for( size_t j=0 ; j<outer_field_count; ++j )
        {
            auc_info[Json::StaticString(vec[j].c_str())].setValue(Json::StaticString(str));
            //auc_info[Json::StaticString(vec[j].c_str())] = Json::StaticString(str);
        }
    }
 
    Json::FastWriter writer;
    std::string str2 = writer.write(res);
 
    int64_t end_time = getCurrentTime();
 
    cerr << "time: " << end_time - start_time << endl;
    cerr << "size: " << str2.size() << endl;
}
 
int main(int argc, const char *argv[])
{
    test();
 
    return 0;
}
