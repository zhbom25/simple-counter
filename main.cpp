#include "counter.h"

int main()
{
    string str = "((1+2)*4/6-8)*9+2";
    auto *solution = new Solution();
    int ret = solution->calculate(str);
    cout << "calculate ret : " << ret << endl;
    return 0;
}
