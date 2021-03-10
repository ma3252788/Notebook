
#include <iostream>
#include <string>

using namespace std;

class Solution {

public:

    // 获取 next 数组，注意其长度等于 s.size() + 1
    void getNext(string s, int *next) {

        int i = 0, j = -1;

        // next 的 0 位必须是 -1
        next[0] = -1;

        while (i < s.size()) {

            if (j == -1 || s[i] == s[j]) {
                next[++i] = ++j;
            } else {
                j = next[j];
            }
        }
    }

    // 利用 next 数组进行匹配
    bool isMatch(string target, string pattern) {

        // 分配模式字符串 next 数组的空间
        int *next = new int[pattern.size() + 1]();

        // 获取模式字符串的 next 数组
        getNext(pattern, next);

        // 输出 next 数组
        for (int i = 0; i < pattern.size() + 1; ++i) {
            cout << next[i] << '\t';
        }
        cout << endl;

        // 利用 next 数组匹配主串
        int i = 0, j = 0;
        int len_t = target.size(), len_p = pattern.size();
        while (i < len_t && j < len_p) {
            if (j == -1 || target[i] == pattern[j]) {
                i++;
                j++;
            } else {
                j = next[j];
            }
        }

        bool res = (j == pattern.size());

        // 删除 next 数组空间
        delete[] next;

        // 返回结果
        return res;
    }

};

int main() {

    Solution s;

    bool res = s.isMatch("abcadaabababc", "abababc");
    if (res) {
        cout << "matched" << endl;
    } else {
        cout << "not matched" << endl;
    }

    return 0;
}

