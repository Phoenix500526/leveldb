#include <iostream>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"
using namespace leveldb;

int main(){
	DB* db;
	Options options;
	options.create_if_missing = true;
	Status status = DB::Open(options, "/tmp/testdb", &db);
	if(!status.ok()){
		std::cerr << status.ToString() << std::endl;
		return -1;
	}
	std::string value;
	std::string key = "k1";
	WriteBatch batch;
	char index[10];
	for(int i = 0; i < 10; ++i){
		std::string pre = "k";
		sprintf(index, "%d", i);
		key = pre + index;
		batch.Put(key, index);
	}
	status = db->Write(leveldb::WriteOptions(), &batch);
	if(status.ok())
		std::cout << "batch write is finished!" << std::endl;
	status = db->Get(ReadOptions(), "k3", &value);
	if(status.ok())
		std::cout << "read the value of k3:!" << value << std::endl;

	for(int i = 0; i < 10; ++i){
		std::string pre = "k";
		sprintf(index, "%d", i);
		key = pre + index;
		batch.Delete(key);
	}
	status = db->Write(WriteOptions(), &batch);
	if(status.ok())
		std::cout << "batch delete is finished" << value << std::endl;
	delete db;
}

