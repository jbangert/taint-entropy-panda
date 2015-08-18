/* PANDABEGINCOMMENT
 *
 * Authors:
 *  Tim Leek               tleek@ll.mit.edu
 *  Ryan Whelan            rwhelan@ll.mit.edu
 *  Joshua Hodosh          josh.hodosh@ll.mit.edu
 *  Michael Zhivich        mzhivich@ll.mit.edu
 *  Brendan Dolan-Gavitt   brendandg@gatech.edu
 *
 * This work is licensed under the terms of the GNU GPL, version 2.
 * See the COPYING file in the top-level directory.
 *
PANDAENDCOMMENT */

#ifndef __LABEL_SET_H_
#define __LABEL_SET_H_

extern "C" {
#include "cpu.h"
#include "qemu-log.h"
}

#include <cstdio>
#include <cstdint>


#include <map>
#include <vector>
#include <set>
#define CONFIG_INT_LABEL
#ifdef CONFIG_INT_LABEL
typedef uint64_t LabelSetP;
typedef uint64_t LabelSet;

#else 
typedef uint32_t taint_label_t;
class LabelSet;
typedef const LabelSet *LabelSetP;
class LabelSet{
 protected:
  std::vector<uint32_t> _data;
 public:
  std::vector<uint32_t>::const_iterator begin() const { return _data.begin(); }
  std::vector<uint32_t>::const_iterator end() const { return _data.end(); }
  size_t size() { return _data.size();}
  LabelSet();
  LabelSet(const LabelSet &other);
  LabelSet(uint32_t label);
  LabelSet(const LabelSet *l1,const  LabelSet *l2);
  bool operator==(const LabelSet &other) const {
    return _data == other._data;
  } 
};
#endif
LabelSetP label_set_union(const LabelSetP ls1, const LabelSetP ls2);

LabelSetP label_set_singleton(uint32_t label);

//void label_set_iter(LabelSetP ls, void (*leaf)(uint32_t, void *), void *user);
LabelSet label_set_render_set(LabelSetP ls);

#endif
