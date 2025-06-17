#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstring>
#include <windows.h>
using namespace std;

const int PFRAMENUM = 3;       // 物理页框数
const int PNUM = 5;             // 虚拟页数
const int AddressSize = 12;     // 地址流长度

int PageAddress[] = {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2}; // 页面访问序列

// 重置全局状态
void resetState(int pageframe[], int pagetime[], int clockRef[], int& pagep, int& isfull, int& lostNum, 
               map<int,int>& timemp, map<int,int>& futuremp) {
    memset(pageframe, -1, sizeof(int) * PFRAMENUM); // 初始化为-1表示空
    memset(pagetime, 0, sizeof(int) * PFRAMENUM);
    memset(clockRef, 0, sizeof(int) * PFRAMENUM);
    pagep = 0;
    isfull = 0;
    lostNum = 0;
    timemp.clear();
    futuremp.clear();
}

// 最佳置换算法(OPT)
void OPT() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM];
    int clockRef[PFRAMENUM]; // 未使用
    int pagep = 0, isfull = 0, lostNum = 0;
    map<int, int> timemp, futuremp;
    resetState(pageframe, pagetime, clockRef, pagep, isfull, lostNum, timemp, futuremp);

    for (int i = 0; i < AddressSize; i++) {
        bool found = false;
        // 检查页面是否已在内存中
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == PageAddress[i]) {
                found = true;
                break;
            }
        }

        // 缺页处理
        if (!found) {
            lostNum++;
            // 如果还有空闲页框
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                pagep++;
                if (pagep == PFRAMENUM) isfull = 1;
            } 
            // 需要替换页面
            else {
                int replaceIdx = -1;
                int farthest = -1;
                
                // 找出未来最长时间不被访问的页面
                for (int j = 0; j < PFRAMENUM; j++) {
                    int nextUse = INT_MAX;
                    // 查找下一次访问位置
                    for (int k = i + 1; k < AddressSize; k++) {
                        if (PageAddress[k] == pageframe[j]) {
                            nextUse = k;
                            break;
                        }
                    }
                    // 更新最远访问位置
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIdx = j;
                    }
                }
                // 执行替换
                pageframe[replaceIdx] = PageAddress[i];
            }
        }

        // 输出当前页框状态
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == -1) cout << "- ";
            else cout << pageframe[j] << " ";
        }
        if (!found) cout << "F"; // 标记缺页
        cout << endl;
        Sleep(300);
    }
    cout << "缺页次数: " << lostNum-PFRAMENUM << endl << endl;
}

// 先进先出算法(FIFO)
void FIFO() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM];
    int clockRef[PFRAMENUM];
    int pagep = 0, isfull = 0, lostNum = 0;
    map<int, int> timemp, futuremp;
    resetState(pageframe, pagetime, clockRef, pagep, isfull, lostNum, timemp, futuremp);

    for (int i = 0; i < AddressSize; i++) {
        bool found = false;
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == PageAddress[i]) {
                found = true;
                break;
            }
        }

        if (!found) {
            lostNum++;
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                pagep++;
                if (pagep == PFRAMENUM) {
                    isfull = 1;
                    pagep = 0; // 重置指针开始循环
                }
            } else {
                pageframe[pagep] = PageAddress[i];
                pagep = (pagep + 1) % PFRAMENUM; // 循环指针
            }
        }

        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == -1) cout << "- ";
            else cout << pageframe[j] << " ";
        }
        if (!found) cout << "F";
        cout << endl;
        Sleep(300);
    }
    cout << "缺页次数: " << lostNum-PFRAMENUM << endl << endl;
}

// 最近最久未使用算法(LRU)
void LRU() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM]; // 记录最近访问时间
    int clockRef[PFRAMENUM];
    int pagep = 0, isfull = 0, lostNum = 0;
    map<int, int> timemp, futuremp;
    resetState(pageframe, pagetime, clockRef, pagep, isfull, lostNum, timemp, futuremp);

    for (int i = 0; i < AddressSize; i++) {
        bool found = false;
        int foundIdx = -1;
        
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == PageAddress[i]) {
                found = true;
                foundIdx = j;
                break;
            }
        }

        if (found) {
            pagetime[foundIdx] = i; // 更新访问时间
        } else {
            lostNum++;
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                pagetime[pagep] = i; // 设置访问时间
                pagep++;
                if (pagep == PFRAMENUM) isfull = 1;
            } else {
                // 找到最久未使用的页面
                int oldest = INT_MAX;
                int replaceIdx = -1;
                for (int j = 0; j < PFRAMENUM; j++) {
                    if (pagetime[j] < oldest) {
                        oldest = pagetime[j];
                        replaceIdx = j;
                    }
                }
                pageframe[replaceIdx] = PageAddress[i];
                pagetime[replaceIdx] = i; // 更新访问时间
            }
        }

        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == -1) cout << "- ";
            else cout << pageframe[j] << " ";
        }
        if (!found) cout << "F";
        cout << endl;
        Sleep(300);
    }
    cout << "缺页次数: " << lostNum-PFRAMENUM << endl << endl;
}

// 时钟置换算法(Clock)
void Clock() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM];
    int clockRef[PFRAMENUM]; // 访问位 (1: 最近访问过, 0: 未访问)
    int pagep = 0, isfull = 0, lostNum = 0;
    map<int, int> timemp, futuremp;
    resetState(pageframe, pagetime, clockRef, pagep, isfull, lostNum, timemp, futuremp);
    int clockPtr = 0; // 时钟指针

    for (int i = 0; i < AddressSize; i++) {
        bool found = false;
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == PageAddress[i]) {
                found = true;
                clockRef[j] = 1; // 设置访问位
                break;
            }
        }

        if (!found) {
            lostNum++;
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                clockRef[pagep] = 1; // 设置访问位
                pagep++;
                if (pagep == PFRAMENUM) {
                    isfull = 1;
                    clockPtr = 0; // 初始化时钟指针
                }
            } else {
                // 查找替换页面
                while (true) {
                    if (clockRef[clockPtr] == 0) {
                        // 找到访问位为0的页面
                        pageframe[clockPtr] = PageAddress[i];
                        clockRef[clockPtr] = 1; // 设置新页面的访问位
                        clockPtr = (clockPtr + 1) % PFRAMENUM; // 移动指针
                        break;
                    } else {
                        // 给予第二次机会
                        clockRef[clockPtr] = 0;
                        clockPtr = (clockPtr + 1) % PFRAMENUM; // 移动指针
                    }
                }
            }
        }

        // 输出当前状态
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == -1) cout << "- ";
            else cout << pageframe[j] << "(" << clockRef[j] << ") ";
        }
        if (!found) cout << "F";
        cout << endl;
        Sleep(300);
    }
    cout << "缺页次数: " << lostNum-PFRAMENUM << endl << endl;
}

int main() {
    while (true) {
        cout << "\n请选择一个算法（OPT、FIFO、LRU、Clock），输入q/Q即可退出: ";
        string choice;
        cin >> choice;
        
        if (choice == "OPT") {
            cout << "\nOPT算法模拟:\n";
            OPT();
        } else if (choice == "FIFO") {
            cout << "\nFIFO算法模拟:\n";
            FIFO();
        } else if (choice == "LRU") {
            cout << "\nLRU算法模拟:\n";
            LRU();
        } else if (choice == "Clock") {
            cout << "\nClock算法模拟(括号内为访问位):\n";
            Clock();
        } else if (choice == "q" || choice == "Q") {
            break;
        } else {
            cout << "输入错误，请重新选择！" << endl;
        }
    }
    return 0;
}
