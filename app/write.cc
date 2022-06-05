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
    Slice value("world");
    status = db->Put(WriteOptions(), key, value);
    if (!status.ok()){
        std::cerr << "Write KV Pair Failed: " << status.ToString() << std::endl;
        delete db;
        return -1;
    }
    delete db;
    return 0;
}