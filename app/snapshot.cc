#include <iostream>
#include "leveldb/db.h"

using namespace leveldb;

int main(){
    DB* db = nullptr;
    Status status = DB::Open(Options(), "/tmp/testdb", &db);
    if (!status.ok()){
        std::cerr << "Cannot Open DB: " << status.ToString() << std::endl;
        return -1;
    }
    ReadOptions readoption;
    readoption.snapshot = db->GetSnapshot();
    Slice key("Fuck"), value("WuZhou");
    status = db->Put(WriteOptions(), key, value);
    if (!status.ok()){
        std::cerr << "Write KV Pair Failed: " << status.ToString() << std::endl;
        delete db;
        return -1;
    }
    std::cout << "Before iterate snapshot" << std::endl;
    Iterator* biter = db->NewIterator(ReadOptions());
    for(biter->SeekToFirst(); biter->Valid(); biter->Next()){
        std::cout << biter->key().ToString() << ": " << biter->value().ToString() << std::endl;
    }
    delete biter;

    std::cout << "After iterate snapshot" << std::endl;
    Iterator* aiter = db->NewIterator(readoption);
    for(aiter->SeekToFirst(); aiter->Valid(); aiter->Next()){
        std::cout << aiter->key().ToString() << ": " << aiter->value().ToString() << std::endl;
    }
    delete aiter;

    delete db;
    return 0;
}