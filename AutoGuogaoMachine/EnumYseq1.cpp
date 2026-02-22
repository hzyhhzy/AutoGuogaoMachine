#include "YSequence.h"
using namespace std;
const int ENUM_LAYERS = 5;
const int MIN_COPY_NUM = 10;
const int MAX_NUMBER_IN_SEQ = 40;
const int MAX_PRINT_TERM= 4;

YSequence smallest_nonterminate_seq;

static void check_nonterminate_seq(const std::vector<INT>& seq, const std::vector<INT>& loop_dif)
{
    INT loop_len = loop_dif.size();
    std::vector<INT> copied_seq = seq;
    INT loop_start = seq.size() - loop_len;
	INT check_copy_num = MIN_COPY_NUM;
    //larger than seq[i] for any i
    for (INT i = 0; i < (INT)seq.size(); i++)
    {
        if (check_copy_num < seq[i] + 1)
        {
            check_copy_num = seq[i] + 1;
        }
    }
    //copy CHECK_COPY_NUM times and add loop_dif
    for (INT i = 0; i < check_copy_num; i++) {
        for (INT j = 0; j < loop_len; j++) {
            copied_seq.push_back(seq[loop_start + j] + (i + 1) * loop_dif[j]);
        }
    }
    assert(copied_seq.size() == seq.size() + check_copy_num * loop_len);
    vector<bool> res;
	//YSequence copied_seq_obj;
    //copied_seq_obj.set_and_build(copied_seq);
    //bool is_standard = copied_seq_obj._checkStandardAndNonMaximum(res);
    bool is_standard = YSequence::checkStandardAndNonMaximum(copied_seq,res);


    if (!is_standard)return;
    //if (!copied_seq_obj.checkConsistency())
    //{
	//	copied_seq_obj.print(cout);
	//	throw std::runtime_error("Inconsistent Y sequence construction");
    //}
    //check whether the last 2 loops of result are non-empty and the same
    INT last_loop_start = copied_seq.size() - 2 * loop_len;
    bool is_same = true;
    bool is_empty = true;
    for (INT i = 0; i < loop_len; i++) {
        if (res[last_loop_start + i] != res[last_loop_start + loop_len + i]) {
            is_same = false;
            break;
        }
        if (res[last_loop_start + i]) {
            is_empty = false;
        }
    }
    if (is_same && !is_empty) {//if the last 2 loops are the same and non-empty, then the sequence is non-terminate
        cout << "Found nonterminate seq: "; 
        YSequence copied_seq_obj;
        copied_seq_obj.set_and_build(copied_seq);
        copied_seq_obj.print(cout);
        cout << endl;
        if (smallest_nonterminate_seq.seq.size() == 0 || smallest_nonterminate_seq > copied_seq_obj) {
            smallest_nonterminate_seq = copied_seq_obj;
            cout << "Found smaller nonterminate seq: ";
            copied_seq_obj.print(cout);
            cout << endl;
        }
    }
}

static void check_loop_dif(const std::vector<INT>& seq)
{
    for (int loop_len = 1; loop_len <= seq.size(); loop_len++) {
        vector<INT> loop_dif;
        //enum all binary sequences of length loop_len
        for (uint64_t i = 0; i < (uint64_t(1) << (loop_len*2)); i++) {
            loop_dif.clear();
            for (int j = 0; j < loop_len; j++) {
                loop_dif.push_back(2 * ((i >> (j*2)) & 3));
            }
            check_nonterminate_seq(seq, loop_dif);
        }
    }
}
static bool enum_recursive(std::vector<INT>& prev) //return whether standard
{
    //check standard
    YSequence ys;
    ys.set_and_build(prev);
    vector<bool> res;
    bool is_standard = ys._checkStandardAndNonMaximum(res);
    if (!is_standard)return false;
    //check loop dif
    if (!ys._isSuccessor())
        check_loop_dif(ys.seq);
    if (prev.size() <= MAX_PRINT_TERM)
    {
        ys.print(cout);
        cout << endl;
    }
    //enum next layer
    if (prev.size() < ENUM_LAYERS) {
        for (int i = 1; i<= MAX_NUMBER_IN_SEQ; i++) {
            prev.push_back(i);
            bool isMax = !enum_recursive(prev);
            prev.pop_back();
            if (isMax)break;
        }
    }
    return true;
}

int main_enumyseq1()
{
    vector<INT> prev = { 1,4 };
    enum_recursive(prev);
    return 0;
}
