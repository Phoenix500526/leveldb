#include <iostream>
#include "db/dbformat.h"
#include "db/memtable.h"

using namespace leveldb;


static std::string IKey(const std::string& user_key, uint64_t seq,
                        ValueType vt) {
  std::string encoded;
  AppendInternalKey(&encoded, ParsedInternalKey(user_key, seq, vt));
  return encoded;
}

static std::string Shorten(const std::string& s, const std::string& l) {
  std::string result = s;

  InternalKeyComparator cmp = InternalKeyComparator(BytewiseComparator());
  cmp.FindShortestSeparator(&result, l);
  return result;
}

static std::string ShortSuccessor(const std::string& s) {
  std::string result = s;
  InternalKeyComparator cmp = InternalKeyComparator(BytewiseComparator());
  cmp.FindShortSuccessor(&result);
  return result;
}

int main(){
    std::string a = IKey("handwork", 10086, kTypeValue);
    std::string b = IKey("handica", 10010, kTypeValue);
    std::string res_1 = Shorten(b, a);
    std::string res_2 = ShortSuccessor(a);
    std::cout << res_1 << std::endl;
    std::cout << res_2 << std::endl;
    InternalKeyComparator cmp(BytewiseComparator());
    MemTable* memtable = new MemTable(cmp);
    memtable->Ref();
    Slice key("hello");
    Slice value("world");
    memtable->Add(10088, kTypeValue, key, value);
    std::string value_bak;
    Status st;
    const LookupKey& look_up = LookupKey("hello", 10088);
    bool find_res = memtable->Get(look_up, &value_bak, &st);
    if(find_res){
      if(st.ok())
        std::cout << value_bak << std::endl;
      else
        std::cout << "key has been delete" << std::endl;
    }else{
        std::cout << "key not found" << std::endl;
    }
    memtable->Unref();
}