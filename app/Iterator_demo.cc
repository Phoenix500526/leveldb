#include <iostream>
#include "leveldb/db.h"
#include "leveldb/iterator.h"
#include "leveldb/write_batch.h"

using namespace leveldb;

int main(){
	DB* db;
	Options options;
	options.create_if_missing = true;
	Status status = DB::Open(options, "/tmp/testdb", &db);
	if(!status.ok()){
		std::cerr << "Open DB failed: " << status.ToString() << std::endl;
		return -1;
	}

	WriteBatch batch;
	char index[10];
	std::string pre = "k";
	for(int i = 10;i < 20; ++i){
		sprintf(index, "%d", i);
		std::string key = pre + index;
		batch.Put(key, index);
	}
	status = db->Write(WriteOptions(), &batch);
	if(!status.ok())
		std::cerr << "batch write is failed: " << status.ToString() << std::endl;
	
	Iterator* i = db->NewIterator(ReadOptions());
	for(i->SeekToFirst(); i->Valid();i->Next())
		std::cout << i->key().ToString() << ": " << i->value().ToString() << std::endl;
	
	for(i->SeekToLast(); i->Valid();i->Prev())
		std::cout << i->key().ToString() << ": " << i->value().ToString() << std::endl;

	for(i->Seek("k12"); i->Valid() && i->key().ToString() < "k18"; i->Next())
		std::cout << i->key().ToString() << ": " << i->value().ToString() << std::endl;

	delete(i);

	for(int i = 10;i < 20; ++i){
		sprintf(index, "%d", i);
		std::string key = pre + index;
		batch.Delete(key);
	}
	status = db->Write(WriteOptions(), &batch);
	if(!status.ok())
		std::cerr << "batch write is failed: " << status.ToString() << std::endl;
	delete(db);
}
