// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_VERSION_EDIT_H_
#define STORAGE_LEVELDB_DB_VERSION_EDIT_H_

#include <set>
#include <utility>
#include <vector>

#include "db/dbformat.h"

namespace leveldb {

class VersionSet;

struct FileMetaData {
  FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) {}

  int refs;
  int allowed_seeks;  // 代表一个文件允许的最大无效查询次数，通过该变量可以出发一次 compaction 操作
  uint64_t number;
  uint64_t file_size;    // File size in bytes
  InternalKey smallest;  // 保存文件中的最小键
  InternalKey largest;   // 保存文件中的最大键，通过最小键和最大键可以快速判断某个待查询的键是否可能位于该文件中
};

// 描述了两个版本之间的修改差异
class VersionEdit {
 public:
  VersionEdit() { Clear(); }
  ~VersionEdit() = default;

  void Clear();

  void SetComparatorName(const Slice& name) {
    has_comparator_ = true;
    comparator_ = name.ToString();
  }
  void SetLogNumber(uint64_t num) {
    has_log_number_ = true;
    log_number_ = num;
  }
  // 仅作旧版本兼容之用
  void SetPrevLogNumber(uint64_t num) {
    has_prev_log_number_ = true;
    prev_log_number_ = num;
  }
  void SetNextFile(uint64_t num) {
    has_next_file_number_ = true;
    next_file_number_ = num;
  }
  void SetLastSequence(SequenceNumber seq) {
    has_last_sequence_ = true;
    last_sequence_ = seq;
  }
  void SetCompactPointer(int level, const InternalKey& key) {
    compact_pointers_.push_back(std::make_pair(level, key));
  }

  // Add the specified file at the specified number.
  // REQUIRES: This version has not been saved (see VersionSet::SaveTo)
  // REQUIRES: "smallest" and "largest" are smallest and largest keys in file
  void AddFile(int level, uint64_t file, uint64_t file_size,
               const InternalKey& smallest, const InternalKey& largest) {
    FileMetaData f;
    f.number = file;
    f.file_size = file_size;
    f.smallest = smallest;
    f.largest = largest;
    new_files_.push_back(std::make_pair(level, f));
  }

  // Delete the specified "file" from the specified "level".
  void RemoveFile(int level, uint64_t file) {
    deleted_files_.insert(std::make_pair(level, file));
  }

  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(const Slice& src);

  std::string DebugString() const;

 private:
  friend class VersionSet;

  typedef std::set<std::pair<int, uint64_t>> DeletedFileSet;

  std::string comparator_;  // 比较器名称，MemTable 和 SSTable 都是有序排列的，需要一个比较器
  uint64_t log_number_;     // 日志文件序号，Log 文件和 MemTable 一一对应
                            // 当一个MemTable 生成 SSTable 后，就会将旧的日志文件删除并且生成一个新的日志文件，
                            // 其名称由 6 位日志文件序号加 log 后缀组成
  uint64_t prev_log_number_;  
  uint64_t next_file_number_;     // 下一个文件序号
  SequenceNumber last_sequence_;  // 下一个写入序列号，LevelDB 每次写入都会递增序列号

  // has_xxx 系列变量用来表示 xxx 变量是否已经设置
  bool has_comparator_;
  bool has_log_number_;
  bool has_prev_log_number_;
  bool has_next_file_number_;
  bool has_last_sequence_;

  // 对于每个层级 L，会记录该层上次进行 Compaction 操作时最大的键，当 L 层下一次进行 Compaction 操作需要选取文件时，该文件的最小键需要大于记录的最大减
  std::vector<std::pair<int, InternalKey>> compact_pointers_; // 表示 LevelDB 的每个层级下一次进行 Compaction 需要从哪个键开始

  DeletedFileSet deleted_files_;  // 记录每个层级执行 Compaction 操作后需要删除掉的文件，只需要记录删除文件的文件序号即可
  std::vector<std::pair<int, FileMetaData>> new_files_; // 记录每个层级执行 Compaction 操作后新增的文件，新增文件用 FileMetaData 结构体表示
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_VERSION_EDIT_H_
