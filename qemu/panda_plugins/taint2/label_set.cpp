extern "C" {
#include <sys/mman.h>
}

#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include "label_set.h"

template<typename T>
class ArenaAlloc {
private:
    uint8_t *next = NULL;
    std::vector<std::pair<uint8_t *, size_t>> blocks;
    size_t next_block_size = 1 << 15;

    void alloc_block() {
        //printf("taint2: allocating block of size %lu\n", next_block_size);
        next = (uint8_t *)mmap(NULL, next_block_size, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        assert(next);
        blocks.push_back(std::make_pair(next, next_block_size));
        next_block_size <<= 1;
    }

    T *alloc_imp() {
        assert(blocks.size() > 0);
        std::pair<uint8_t *, size_t>& block = blocks.back();
        if (next + sizeof(T) > block.first + block.second) {
            alloc_block();
            assert(next != NULL);
        }

        T *result = new(next) T;
        next += sizeof(T);
        return result;
    }

public:
    ArenaAlloc() {
        alloc_block();
    }

    const T *alloc() {
        return alloc_imp();
    }

    const T *alloc(T &old) {
        T *result = alloc_imp();
        result->swap(old);
        return result;
    }

    ~ArenaAlloc() {
        for (auto&& block : blocks) {
            munmap(block.first, block.second);
        }
    }
};

static ArenaAlloc<LabelSet> LSA;

namespace std {
template<>
class hash<LabelSet> {
  public:
    size_t operator()(const LabelSet &labels) const {
        uint64_t result = 0;
        //pretty shitty hash function?
        for (uint32_t l : labels) {
            result ^= l;
            result = result << 11 | result >> 53;
        }
        return result;
    }
};

template<>
class hash<pair<LabelSetP, LabelSetP>> {
  public:
    size_t operator()(const pair<LabelSetP, LabelSetP> &labels) const {
        return hash<LabelSetP>()(labels.first) ^
            (hash<LabelSetP>()(labels.second) << (sizeof(LabelSetP) / 2));
    }
};
}

LabelSet::LabelSet() : _data(0){
}
LabelSet::LabelSet(const LabelSet &other) : _data(other._data) {
}
LabelSet::LabelSet(uint32_t label) :_data(1,label) {
}
//Count elements in l1 and l2
size_t merged_size(const LabelSet *l1, const LabelSet *l2) {
  size_t s = 0;
  auto i1 = l1->begin(), i2 = l2->begin(), end1 = l1->end() , end2 = l2->end();
  while(i1 != end1 && i2 != end2){
    if(*i1 < *i2)
      i1++;
    else if(*i2 < *i1)
      i2++;
    else{
      i1++;
      i2++;
    }
    s++; // assume each array contains no duplicates

  }
  return s + (end2-i2) + (end1-i1);
}
LabelSet::LabelSet(const LabelSet *l1,const  LabelSet *l2): _data(merged_size(l1,l2)){
  auto i1 = l1->begin(), i2 = l2->begin(), end1 = l1->end() , end2 = l2->end();
  auto o = _data.begin();
  while(i1 != end1 && i2 != end2){
    if(*i1 < *i2){
      *o = *i1;
      i1++;
    }
    else if(*i2 < *i1){
      *o = *i2;
      i2++;
    }
    else{
      *o = *i1;
      i1++;
      i2++;
    }
    o++;
  }
  while(i1 != end1)
    *(o++) = *(i1++);
  while(i2 != end2)
    *(o++) = *(i2++);
  assert(o == _data.end());
}
static std::unordered_set<LabelSet> label_sets;
static std::unordered_map<std::pair<LabelSetP, LabelSetP>, LabelSetP> memoized_unions;
//XXX: use skew heaps?
LabelSetP label_set_union(const LabelSetP ls1,const LabelSetP ls2) {
    

    if (ls1 == ls2) {
        return ls1;
    } else if (ls1 && ls2) {
        LabelSetP min, max;
        if(ls1<ls2){ // compare pointers, not sets. All pointers come from label_sets
            min = ls1;
            max = ls2;
        } else {
            min = ls1;
            max = ls2;
        }
        std::pair<LabelSetP, LabelSetP> minmax(min, max);

        {
            auto it = memoized_unions.find(minmax);
            if (it != memoized_unions.end()) {
                return it->second;
            }
        }
        auto it = label_sets.emplace(min,max);
        const LabelSetP result = &(*it.first);

        memoized_unions.insert(std::make_pair(minmax, result));
        return result;
    } else if (ls1) {
        return ls1;
    } else if (ls2) {
        return ls2;
    } else return nullptr;
}

LabelSetP label_set_singleton(uint32_t label) {
    auto i = label_sets.emplace(LabelSet(label));
    return &(*i.first);
}
LabelSet label_set_render_set(LabelSetP ls) {
    if (ls) return *ls;
    else return LabelSet();
}
