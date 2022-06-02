#include <iostream>
#include <cassert>
#include "leveldb/db.h"

using namespace leveldb;

int main(){
	DB* db;
	Options options;
	options.error_if_exists = true;
	Status status = DB::Open(options, "/tmp/testdb", &db);
	if (!status.ok()){
		std::cerr << status.ToString() << std::endl;
	}
	delete db;
}
