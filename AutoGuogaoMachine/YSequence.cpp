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
  throw "todo";
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
    s[i].x = seq0[i];
  }
  for (INT i = 0; i < (INT)seq0.size(); ++i) {
    build_col(i);
  }

}
void YSequence::build_col(INT idx)
{
  Ycol& col = s[idx];
  col.mountain.resize(0);


  INT x=s[idx].x; //current mountain top
  INT p=idx-1; //current parent. 0 th row is idx-1


  for(INT wrow=0;;++wrow)
  {
    if(x==1)
      break;
    else 
      assert(p>=0);

    col.mountain.push_back({});
    col.mountain[wrow].resize(1);
    col.mountain[wrow][0] = { x,p };

    for(INT row=0;;++row) // mountain[wrow][row] is omega*wrow+row th row 
    {
      //find the nearest number smaller than x
      p = col.mountain[wrow][row].second;
      bool reachEdge = false;
      while(true)
      {
        assert(p>=0);
        Ycol& parent=s[p];
        if(parent.mountain.size() <= wrow)
        {
          assert(parent.mountain.size()==wrow);
          assert(wrow==0 || parent.mountain[wrow-1].back().first==1); 
          parent.mountain.push_back({});
          if (wrow > 0)
            parent.mountain[wrow].push_back(parent.mountain[wrow - 1].back());
          else
          {
            assert(parent.x == 1);
            parent.mountain[wrow].push_back({1, -1});
          }
        }
        if(parent.mountain[wrow].size() <= row)//the mountain edge
        {
          reachEdge = true;
          break;
        }


        if(parent.mountain[wrow][row].first<x) //found the real parent of the next layer
        {
          assert(col.mountain[wrow].size() == row+1);

          x = x - parent.mountain[wrow][row].first;
          col.mountain[wrow].push_back({ x,p });
          break;
        }

        //check the previous parent 
        p=parent.mountain[wrow][row].second;
      }
      if (reachEdge)
        break;
    }
  }

}
