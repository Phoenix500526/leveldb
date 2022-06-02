#include <iostream>
#include <cassert>
#include "leveldb/db.h"

using namespace leveldb;

int main(){
	Slice key("k1");
	std::string value;
	DB* db;
	Options options;
	Status status = DB::Open(options, "/tmp/testdb", &db);
	if (!status.ok()){
		std::cerr << status.ToString() << std::endl;
	}
	status = db->Get(ReadOptions(), key, &value);
	if (!status.ok()){
		std::cerr << key.data() << ':' << status.ToString() <<  std::endl;
	}
	
	status = db->Put(WriteOptions(), key, key);
	if(status.ok()){
		std::cout << "Write successfully" << std::endl;
		status = db->Get(ReadOptions(), key, &value);
		if(status.ok())
			std::cout << "the value of " << key.data() << ": " << value << std::endl;
	}
	if(db->Delete(WriteOptions(), key).ok())
		std::cout << key.data() << "-" << value << " is deleted" << std::endl;
	delete db;
}
