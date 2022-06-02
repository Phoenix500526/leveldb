#include <iostream>
#include "leveldb/db.h"

using namespace leveldb;

int main(){
    DB* db = nullptr;
    Options option;
    option.create_if_missing = true;
    Status status = DB::Open(option, "/tmp/testdb", &db);
    if (!status.ok()){
        std::cerr << "Cannot Open DB: " << status.ToString() << std::endl;
        return -1;
    }
    Slice key("hello");
    std::string value;
    status = db->Get(ReadOptions(), key, &value);
    if (status.ok()){
        std::cout << "key = " << key.data() << ", value = " << value << std::endl;
    }else{
        std::cerr << "Write KV Pair Failed: " << status.ToString() << std::endl;
        delete db;
        return -1;
    }
    delete db;
    return 0;
}