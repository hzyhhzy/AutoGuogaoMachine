#include "PPSNotation.h"
using namespace std;
const int ENUM_LAYERS=8;
const int CHECK_COPY_NUM=5;

PPSNotation smallest_nonterminate_seq;

static void check_nonterminate_seq(const std::vector<INT>& seq, const std::vector<INT>& loop_dif)
{
    INT loop_len=loop_dif.size();
    PPSNotation copied_seq;
    copied_seq.seq = seq;
    INT loop_start=seq.size()-loop_len;
    //copy CHECK_COPY_NUM times and add loop_dif
    for (INT i=0;i<CHECK_COPY_NUM;i++){
        for (INT j=0;j<loop_len;j++){
            copied_seq.seq.push_back(seq[loop_start+j]+(i+1)*loop_dif[j]);
        }
    }
    assert(copied_seq.seq.size()==seq.size()+CHECK_COPY_NUM*loop_len);
    vector<bool> res;
    bool is_standard=copied_seq._checkStandardAndNonMaximum(res);
    if(!is_standard)return;
    //check whether the last 2 loops of result are non-empty and the same
    INT last_loop_start=copied_seq.seq.size()-2*loop_len;
    bool is_same=true;
    bool is_empty=true;
    for (INT i=0;i<loop_len;i++){
        if(res[last_loop_start+i]!=res[last_loop_start+loop_len+i]){
            is_same=false;
            break;
        }
        if(res[last_loop_start+i]){
            is_empty=false;
        }
    }
    if(is_same&& !is_empty){//if the last 2 loops are the same and non-empty, then the sequence is non-terminate
        //cout << "Found nonterminate seq: ";
        copied_seq.print(cout);
        cout << endl;
        if(smallest_nonterminate_seq.seq.size()==0||smallest_nonterminate_seq>copied_seq){
            smallest_nonterminate_seq=copied_seq;
            cout << "Found smaller nonterminate seq: ";
            copied_seq.print(cout);
            cout << endl;
        }
    }
}

static void check_loop_dif(const std::vector<INT>& seq)
{
    for(int loop_len=1;loop_len<=seq.size();loop_len++){
        vector<INT> loop_dif;
        //enum all binary sequences of length loop_len
        for(uint64_t i=0;i<(uint64_t(1)<<loop_len);i++){
            loop_dif.clear();
            for(int j=0;j<loop_len;j++){
                loop_dif.push_back(loop_len*((i>>j)&1));
            }
            check_nonterminate_seq(seq,loop_dif);
        }
    }
}
static void enum_pps_recursive(std::vector<INT>& prev)
{
    //check standard
    PPSNotation pps;
    pps.seq = prev;
    vector<bool> res;
    bool is_standard=pps._checkStandardAndNonMaximum(res);
    if(!is_standard)return;
    //check loop dif
    if(!pps._isSuccessor())
        check_loop_dif(pps.seq);
    //pps.print(cout);
    //enum next layer
    if(prev.size()<ENUM_LAYERS){
        INT max_i = prev.size() ;
        for(int i=0;i<= max_i;i++){
            prev.push_back(i);
            enum_pps_recursive(prev);
            prev.pop_back();
        }
    }
}

int main_enumpps1()
{
    vector<INT> prev;
    enum_pps_recursive(prev);
    return 0;
}
