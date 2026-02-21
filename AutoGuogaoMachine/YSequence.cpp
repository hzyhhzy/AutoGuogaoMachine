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
    s.clear();
  }
  else {
      seq.resize(seq.size() - n);
      s.resize(s.size() - n);
  }
}

void YSequence::expand_single_col(INT root, INT orig_len, INT idx_since_orig) //չ�������еĸ���һ�С�
{
    assert(s.size() == orig_len + idx_since_orig);
    INT copy_period = orig_len - root - 1;
    INT ref_idx = idx_since_orig % copy_period + root + 1;
    INT fill_num = idx_since_orig / copy_period + 1;
    s.push_back({});
    Ycol& ref_col = s[ref_idx];
    Ycol& new_col = s.back();
    for (INT wrow = 0; wrow < (INT)ref_col.mountain.size(); wrow++)
    {
        new_col.mountain.push_back({});
        //����Ƿ���Ҫ��ȫ
        INT lift_end = 0;
        INT lift_start = 0;
        INT lift_layers = 0;
        bool maybe_lift = true;//��Ҫ������ȫ
        if ((INT)s[orig_len - 1].mountain.size() > wrow)
        {
            lift_end = s[orig_len - 1].mountain[wrow].size();
            lift_start = 0;
            if ((INT)s[root].mountain.size() > wrow)
                lift_start = s[root].mountain[wrow].size();
            lift_layers = lift_end - lift_start;
            assert(lift_layers >= 0);
            //assert(lift_layers == 0);
        }
        if (lift_layers == 0)
            maybe_lift = false;
        if (maybe_lift)//��Ҫ��ȫ������һ�в�һ��������Ƿ�Ϊ����Ԫ��
        {
            INT idx_cur = ref_idx;
            //���ŵ�wrow���lift_start�еĸ��ڵ���ǰ��鿴���ܷ񵽴�root������ܵ���root˵����Ҫ������������Ҫ����
            while (true)
            {
                if ((INT)s[idx_cur].mountain.size() <= wrow || (INT)s[idx_cur].mountain[wrow].size() <= lift_start) //û�и��ڵ��ˣ�˵������Ҫ����
                {
                    maybe_lift = false;
                    break;
                }
                INT next_idx = s[idx_cur].mountain[wrow][lift_start].second;
                if (next_idx == root) //�ܵ���root�ˣ�˵����Ҫ����
                {
                    break;
                }
                else if (next_idx > root) //������ǰ��
                {
                    idx_cur = next_idx;
                }
                else //����root����û�����ˣ�˵������Ҫ����
                {
                    maybe_lift = false;
                    break;
                }
            }
        }

        //�������ȫ��ֱ�Ӹ��ơ����ڵ���root��parent��Ҫ����fill_num*copy_period
        if (!maybe_lift)
        {
            for (INT row = 0; row < (INT)ref_col.mountain[wrow].size(); row++)
            {
                INT parent = ref_col.mountain[wrow][row].second;
                if (parent >= root)
                    parent = parent + fill_num * copy_period;
                new_col.mountain[wrow].push_back({ -1, parent }); //��ֵ����
            }
        }
        else //��Ҫ��ȫ
        {
            //[0,lift_start)��ֱ�Ӹ���
            //[lift_start,lift_start+lift_layers*fill_num)���á����Ʊߡ���䣬���Ʊ�Ϊref_col[wrow][lift_start]������lift_layers*fill_num��
            //[lift_start+lift_layers*fill_num, end)���ǲο��е�[lift_start, end)��row��������lift_layers*fill_num�Ľ��
            INT lift_rows = lift_layers * fill_num;
            for (INT row = 0; row < lift_start; row++)
            {
                INT parent = ref_col.mountain[wrow][row].second;
                if (parent >= root)
                    parent = parent + fill_num * copy_period;
                new_col.mountain[wrow].push_back({ -1, parent }); //��ֵ����
            }
            for (INT row = lift_start; row < lift_start + lift_rows; row++)
            {
                INT parent = ref_col.mountain[wrow][lift_start].second;
                if (parent >= root)
                    parent = parent + fill_num * copy_period;
                new_col.mountain[wrow].push_back({ -1, parent }); //��ֵ����
            }
            for (INT row = lift_start + lift_rows; row < (INT)ref_col.mountain[wrow].size() + lift_rows; row++)
            {
                INT parent = ref_col.mountain[wrow][row - lift_rows].second;
                if (parent >= root)
                    parent = parent + fill_num * copy_period;
                new_col.mountain[wrow].push_back({ -1, parent }); //��ֵ����
            }
        }

    }

    //ɽ��ͼ�������ˣ����ڴӶ������»�ԭ��ֵ�������1��Ȼ����parent����ֵ�ۼ�
    INT x_now = 1;
    for (INT wrow = new_col.mountain.size() - 1; wrow >= 0; wrow--)
    {
        for (INT row = new_col.mountain[wrow].size() - 1; row >= 0; row--)
        {
            INT parent_idx = new_col.mountain[wrow][row].second;
            assert(parent_idx >= 0);
            const Ycol& parent_col = s[parent_idx];
            INT value_parent = -1;
            if (row == 0)//����parent����һ��wrow��ȡ����������
            {
                INT parent_wrow = wrow - 1;
                if (INT(parent_col.mountain.size()) <= parent_wrow)//������ȡ�Ĳ�ֹһ�㣨ע��������벢û�а���ȡ��������һ����ʽ��¼������1�������ֿ��ܿ�ü��㣩
                    parent_wrow = parent_col.mountain.size() - 1;
                if (parent_wrow == -1)//ԭ����
                {
                    value_parent = parent_col.x;

                }
                else
                {
                    value_parent = parent_col.mountain[parent_wrow].back().first;
                }
            }
            else
            {
                //���������parent��ͬһ�����һ��
                assert((INT)parent_col.mountain.size() > wrow);
                assert((INT)parent_col.mountain[wrow].size() > row - 1);
                value_parent = parent_col.mountain[wrow][row - 1].first;
            }

            new_col.mountain[wrow][row].first = x_now;
            x_now = x_now + value_parent;
        }
    }
    new_col.x = x_now;
    new_col.cachedMax = new_col.x;//��չ����һ�������ֵ
    seq.push_back(new_col.x);
}

// Expand non-successor n times
// Fundamental sequence [n] expands to the y + nL - 1 term
void YSequence::_expand(INT n) {
    assert(!_isSuccessor());
    INT root = s.back().mountain.back().back().second;
    INT orig_len = s.size();
    seq.back() = seq.back() - 1; // -1 and rebuild
    build_col(orig_len - 1);
    if (n == 0)
        return;

    INT copy_period = orig_len - root - 1;
    for (int i = 0; i < n * copy_period; i++) //build i+orig_len+1 th row
    {
        expand_single_col(root, orig_len, i);
    }
    assert(checkConsistency());
    return;

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

    for (int i = 0; i < n; i++) //build i+orig_len+1 th row
    {
		expand_single_col(root, orig_len, i);
    }

    assert(checkConsistency());

    return;
}

// Same as _expandLen(0)
void YSequence::_reduce() {
    assert(false);
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
    if (n == 0)
        set_and_build({ 1 });
    else 
        set_and_build({ 1, n });
}

// Find standard form and check non-maximum
bool YSequence::_checkStandardAndNonMaximum(std::vector<bool>& res) {
    // res must be same size as current seq and initially false
    res.resize(seq.size());
    std::fill(res.begin(), res.end(), false);
    if (seq.size() == 0)
    {
		return true; //empty sequence is 0
    }
    else if (seq.size() == 1)
    {
        res[0] = true;
        return seq[0] == 1;
    }
    else if (seq.size() == 2)
    {
        res[0] = true;
        res[1] = false;
        return seq[0] == 1 && seq[1] >= 1;
    }



    YSequence work;
    // We don't know the exact length to start with.
    // But based on "find the first difference", we can compare [0, 1, 2, ...] with `seq`.

    work._init(seq[1] + 1);

    INT idx = 0;

    while (idx < (INT)seq.size()) {
        if (work.seq[idx] < seq[idx]) //non-standard
            return false;
        if (work.seq[idx] == seq[idx])
        {
            idx++;
            continue;
        }
        assert(work.seq[idx] > seq[idx]);
        res[idx] = true;

        //cut
        //work.seq.resize(idx + 1);
        assert(work.seq.size() == idx + 1);

        INT expanded_terms = work._expandUntilLarger(*this, false);
        if (expanded_terms == -1)
        {
            return false;
        }
        idx += expanded_terms;
    }

    //standard, copy the cachedMax
	for (INT i = 2; i < (INT)seq.size(); i++)
    {
       s[i].cachedMax=work.s[i].cachedMax;
    }
    return true;
}

INT YSequence::_expandUntilLarger(const SequenceNotation& target, bool selfcheck) {
    if (selfcheck) {
        if (seq.size() > target.seq.size()) {
            throw std::runtime_error("Sequence length larger than target length");
        }
        for (size_t i = 0; i < seq.size() - 1; ++i) {
            if (seq[i] != target.seq[i]) {
                throw std::runtime_error("Sequence prefix does not match target");
            }
        }
        if (seq.empty())
        {
            throw std::runtime_error("Sequence is empty");
        }
        if (_isSuccessor()) {
            throw std::runtime_error("Sequence is a successor");
        }


    }


    INT orig_len = (INT)seq.size();


    INT val = seq.back();
    INT target_val = target.seq[orig_len - 1];

    // If lengths are equal and we match exactly
    if (orig_len == target.seq.size() && val == target_val) return 0;

	if (val <= target_val) return -1; //smaller than target, no need to expand

    if (val > target_val + 1)
    {
		seq.back() = target_val + 1; //ֱ�ӽ�����target��1������չ��һ�����target
        build_col(orig_len - 1);
    }

    INT root = s.back().mountain.back().back().second;
    seq.back() = seq.back() - 1; // -1 and rebuild
    build_col(orig_len - 1);

    for (int i = 0; i < target.seq.size()-orig_len; i++) //build i+orig_len+1 th row
    {
        expand_single_col(root, orig_len, i);
        INT idx = orig_len + i;
		assert(seq.size() == idx + 1);
        if (seq[idx] > target.seq[idx])
        {
			return seq.size() - orig_len; //larger
        }
        else if (seq[idx] < target.seq[idx]) //non-standard
        {
            return -1;
        }
        else
        {
			continue; //same, continue
        }

    }

	//if reach here, completely same as target in the prefix, now check length
    assert(seq.size() == target.seq.size());
    assert(seq.back() == target.seq.back());

    // If we finished loop, we are exactly equal to target
    return seq.size() - orig_len;

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
        if (row == 0)//��Ҫ��ȡ
        //  if (parent.mountain.size() <= wrow)//��Ҫ��ȡ
        {
          //wrow_parent = parent.mountain.size() - 1;
          if (wrow_parent > INT(parent.mountain.size() - 1))
            wrow_parent = INT(parent.mountain.size() - 1);
          if (wrow_parent >= 0) //��ȡ
          {
            value_parent = parent.mountain[wrow_parent].back().first;
            next_p = parent.mountain[wrow_parent].back().second;
          }
          else //������
          {
            value_parent = s[p].x;
            next_p = p - 1;
            //assert(seq[p] == 1);
          }
        }
        else
        {
          if (INT(parent.mountain.size()) <= wrow || INT(parent.mountain[wrow].size()) <= row - 1)//the mountain edge
          {
            reachEdge = true;
            break;
          }
          else //�������
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
bool YSequence::checkConsistency() const
{
  YSequence tmp;
  tmp.set_and_build(seq);
  //tmp.s should be completely same as seq.s
  if (s.size() != tmp.s.size())
      return false;
  for (size_t i = 0; i < s.size(); ++i)
  {
      if (s[i].x != tmp.s[i].x)
          return false;
      if (s[i].mountain != tmp.s[i].mountain)
          return false;
  }
  return true;
}