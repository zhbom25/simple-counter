#include "counter.h"

int Solution::calculate(string s)
{
    stack<int> stk;
    /*
     * 对运算符号进行替换
     */
    _replace(s, " ", EMPTY_STR);
    // 必须先执行减
    _replace(s, "-", SUB_STR);
    _replace(s, "+", ADD_STR);
    _replace(s, "*", MUL_STR);
    _replace(s, "/", DIV_STR);
    _replace(s, "(", LEFT_STR);
    _replace(s, ")", RIGHT_STR);
    // 替换多余空格
    _replace(s, "  ", " ");
    // 去除首尾空格
    _trim(s);
    vector<string> vec_str = _split(s, SPLIT_STR);
    vector<int> vec_val;
    // 转化为int型，进行计算
    for (size_t i = 0; i < vec_str.size(); i++) {
        int tmp_v = atoi(vec_str[i].c_str());
        vec_val.push_back(tmp_v);
    }
    for (size_t j = 0; j < vec_val.size(); j++) {
        // 元素不是右括号，入栈
        if (vec_val[j] != RIGHT_B) {
            stk.push(vec_val[j]);
        } else { // 是右括号
            // 出栈
            vector<int> tmp_vec;
            int tmp_v = stk.top();
            stk.pop();
            // 不是左括号，提取括号内 局部表达式
            while (tmp_v != LEFT_B) {
                tmp_vec.push_back(tmp_v);
                if (!stk.empty()) {
                    tmp_v = stk.top();
                    stk.pop();
                }
            }
            vector<int> s_n;
            int m = tmp_vec.size() - 1;
            while (m >= 0) {    
                s_n.push_back(tmp_vec[m]);
                m = m-1;
            }
            // 计算局部表达式值
            int ret_ = _cal_a(s_n);
            stk.push(ret_);
        }
    }
    vector<int> t_n;
    // 最后栈内剩余最后一个局部表达式
    while (!stk.empty()) {
        t_n.push_back(stk.top());
        stk.pop();
    }
    vector<int> s_n(t_n.rbegin(), t_n.rend());
    return _cal_a(s_n);
}

int Solution::_cal_a(vector<int> &vec, int start) 
{
    if (vec.size() == 0) {
        return 0;
    }
    if (vec.size() == 1) {
        return vec[0];
    }
    if (start >= vec.size()-1) {
        return vec[start];
    }
    int a = vec[start];
    int opt = vec[start+1];
    // 第二个元素是 +号。例如 a + b -c; a + b * c; a + b / c; a + b +c，均可等价于 a + _cal_a(xxx)
    if (opt == ADD_OPT) {
        return a + _cal_a(vec, start+2);
    } else if (opt == SUB_OPT) {
        // 第二个元素是 -号。等价于 a + (-b) 情况
        vec[start+2] = -1 * vec[start+2];
        return a + _cal_a(vec, start+2);
    } else {
        // 第二个元素是 *号 或 /号，寻找第一个出现的 +或-; 乘和除局部表达式 _cal_d 处理
        for (size_t i = start; i < vec.size(); ++i) {
            if (vec[i] == ADD_OPT) {
                return _cal_d(vec, start, i-1) + _cal_a(vec, i+1);
            } else if (vec[i] == SUB_OPT) {
                vec[i+1] = -1 * vec[i+1];
                return _cal_d(vec, start, i-1) + _cal_a(vec, i+1);
            }
        }
        return _cal_d(vec, start, vec.size()-1);
    }
    return 0;
}

// 处理不存在加减号的表达式
int Solution::_cal_d(vector<int> &vec, int start, int end) 
{
    int sum = vec[start];
    for (size_t i = start+1; i < vec.size()-1; i=i+2) {
        if (vec[i] == MUL_OPT) {
            sum = sum * vec[i+1];
        } else if (vec[i] == DIV_OPT) {
            sum = sum / vec[i+1];
        }
    }
    return sum;
}

void Solution::_trim(string &s)
{
    if (s.empty()) {
        return; 
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
}

vector<string> Solution::_split(string &str, const string &pattern)
{
    string::size_type pos;
    vector<string> result;
    str += pattern;
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void Solution::_replace(string &str, const string &s1, const string &s2)
{
    string::size_type pos = 0;
    string::size_type a = s1.size();
    string::size_type b = s2.size();
    while ((pos = str.find(s1, pos)) != string::npos) {
        str.replace(pos, a, s2);
        pos += b;
    }
}

