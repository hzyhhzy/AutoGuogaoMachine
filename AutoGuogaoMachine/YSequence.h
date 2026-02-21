#pragma once
#include "SequenceNotation.h"
#include <vector>

class YSequence : public SequenceNotation
{
  void build_col(INT idx);//build idx row, assume all previous cols are built
  void expand_single_col(INT root, INT orig_len, INT idx_since_orig); //չ�������еĸ���һ�С�

  

public:
  struct Ycol //one column of y sequence
  {
    INT x;//main number
    std::vector<std::vector<std::pair<INT, INT>>> mountain;//mountain[a][b] -> w*a+b th row. first is number, second is parent idx.
    INT cachedMax;//maximum number of this column, 0 if unknown
    void clear();
    Ycol();
  };

  std::vector<Ycol> s;
  bool checkConsistency() const;//check mountain
  void set_and_build(std::vector<INT> seq0);


  // Check if successor ordinal (last term is 1)
  bool _isSuccessor() override;

  // Cut the last number
  void _cut() override;

  // Cut the last n numbers
  void _cut(INT n) override;

  // Expand non-successor n times
  void _expand(INT n) override;

  // Expand until length is at least x + n, then cut to x + n
  void _expandLen(INT n) override;

  // Same as _expandLen(0)
  void _reduce() override;

  // Dictionary order comparison
  int _compare(const SequenceNotation& other) const override;

  // Initialize to [0, 1, ..., n-1]
  void _init(INT n)  override;

  // Find standard form and check non-maximum
  bool _checkStandardAndNonMaximum(std::vector<bool>& res) override;
  INT _expandUntilLarger(const SequenceNotation& target, bool selfcheck = true) override;
  void print(std::ostream& os) const override;
};
