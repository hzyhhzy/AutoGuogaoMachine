#include "PPSNotation.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "YSequence.h"

// Check if successor ordinal (last term is 0)
bool YSequence::_isSuccessor() {
  if (seq.empty()) return true;
  return seq.back() == 1;
}

// Cut the last number
void YSequence::_cut() {
  if (!seq.empty()) {
    seq.pop_back();
    s.pop_back();
  }
}

// Cut the last n numbers
void YSequence::_cut(INT n) {
  if (n < 0) return;
  if ((size_t)n >= seq.size()) {
    seq.clear();
  }
  else {
    seq.resize(seq.size() - n);
  }
}

// Expand non-successor n times
// Fundamental sequence [n] expands to the y + nL - 1 term
void YSequence::_expand(INT n) {
}

// Expand until length is at least x + n, then cut to x + n
// Here 'x' refers to the original length (as per user context: "If original length is x...")
void YSequence::_expandLen(INT n) {
  assert(!_isSuccessor());
  INT root = s.back().mountain.back().back().second;
  INT orig_len = s.size();
  seq.back() = seq.back() - 1; // -1 and rebuild
  build_col(orig_len - 1); 
  if (n == 0)
    return;

  INT copy_period = orig_len - root - 1;
  for (int i = 0; i < n; i++) //build i+orig_len+1 th row
  {
    INT ref_idx = i % copy_period + root + 1;
    Ycol& ref_col = s[ref_idx];
    INT fill_num = i / copy_period;
    for (INT wrow = 0; wrow < ref_col.mountain.size(); wrow++)
    {
      //检测是否需要补全
      INT lift_end = 0;
      INT lift_start = 0;
      INT lift_layers = 0;
      if (s[orig_len - 1].mountain.size() > wrow)
      {
        lift_end = s[orig_len - 1].mountain[wrow].size();
        lift_start = 0;
        if (s[root].mountain.size() > wrow)
          lift_start = s[root].mountain[wrow].size(); 
        lift_layers = lift_end - lift_start;
        assert(lift_layers >= 0);
        assert(lift_layers == 0);
      }
      if(wrow)
      INT base_h = s[root].mountain[wrow].size();
    }
  }



  return;
}

// Same as _expandLen(0)
void YSequence::_reduce() {
}

// Dictionary order comparison
int YSequence::_compare(const SequenceNotation& other) const {
  size_t len1 = seq.size();
  size_t len2 = other.seq.size();
  size_t min_len = std::min(len1, len2);

  for (size_t i = 0; i < min_len; ++i) {
    if (seq[i] > other.seq[i]) return 1;
    if (seq[i] < other.seq[i]) return -1;
  }

  if (len1 > len2) return 1;
  if (len1 < len2) return -1;
  return 0;
}

// Initialize to [1, w]
void YSequence::_init(INT n) {
  seq.clear();
  seq.reserve(n);
  for (INT i = 0; i < n; ++i) {
    seq.push_back(i);
  }
}

// Find standard form and check non-maximum
bool YSequence::_checkStandardAndNonMaximum(std::vector<bool>& res) {
  throw "todo";
  return true;
}

INT YSequence::_expandUntilLarger(const SequenceNotation& target, bool selfcheck) {
  throw "todo";
  return 0;
}
void YSequence::print(std::ostream& os) const {
  for (INT i = 0; i < (INT)seq.size(); ++i) {
    os << seq[i] << ",";
  }
}

YSequence::Ycol::Ycol()
{
  clear();
}
void YSequence::Ycol::clear()
{
  x = 0;
  mountain.clear();
  cachedMax = 0;
}


void YSequence::set_and_build(std::vector<INT> seq0)
{
  seq = seq0;

  s.resize(seq0.size());
  for (INT i = 0; i < (INT)seq0.size(); ++i) {
    s[i].clear();
  }
  for (INT i = 0; i < (INT)seq0.size(); ++i) {
    build_col(i);
  }

}
void YSequence::build_col(INT idx)
{
  Ycol& col = s[idx];
  col.mountain.resize(0);
  col.x = seq[idx];


  INT x=s[idx].x; //current mountain top


  for(INT wrow=0;;++wrow)
  {
    if(x==1)
      break;

    col.mountain.push_back({});
    //col.mountain[wrow].resize(1);
    //col.mountain[wrow][0] = { x,p };

    for(INT row=0;;++row) // mountain[wrow][row] is omega*wrow+row th row 
    {
      INT p = idx - 1; //current parent. 0 th row is idx-1
      if (row > 0) //last row
        p = col.mountain[wrow][row - 1].second;
      else if (wrow > 0 && row == 0) //top of the last wrow
        p = col.mountain[wrow - 1].back().second;

      //find the nearest number smaller than x
      bool reachEdge = false;
      while(true)
      {
        assert(p >= 0);
        Ycol& parent=s[p];
        INT wrow_parent = row == 0 ? wrow - 1 : wrow;
        INT value_parent = -1; 
        INT next_p = -1;
        if (row == 0)//需要提取
        //  if (parent.mountain.size() <= wrow)//需要提取
        {
          //wrow_parent = parent.mountain.size() - 1;
          if (wrow_parent > INT(parent.mountain.size() - 1))
            wrow_parent = INT(parent.mountain.size() - 1);
          if (wrow_parent >= 0) //提取
          {
            value_parent = parent.mountain[wrow_parent].back().first;
            next_p = parent.mountain[wrow_parent].back().second;
          }
          else //最下面
          {
            value_parent = seq[p];
            next_p = p - 1;
            //assert(seq[p] == 1);
          }
        }
        else
        {
          if (parent.mountain.size() <= wrow || parent.mountain[wrow].size() <= row - 1)//the mountain edge
          {
            reachEdge = true;
            break;
          }
          else //最常见情形
          {
            value_parent = parent.mountain[wrow][row - 1].first;
            next_p = parent.mountain[wrow][row - 1].second;
          }
        }

        assert(value_parent >= 1);
        if(value_parent < x) //found the real parent of the next layer
        {
          assert(col.mountain[wrow].size() == row);

          x = x - value_parent;
          col.mountain[wrow].push_back({ x,p });
          break;
        }

        //not the real parent, check the previous parent 
        p = next_p;
      }
      if (reachEdge)
        break;
    }
  }

}
