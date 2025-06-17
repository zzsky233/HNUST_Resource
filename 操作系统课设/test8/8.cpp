#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstring>
#include <windows.h>
using namespace std;

const int PFRAMENUM = 3;       // ����ҳ����
const int PNUM = 5;             // ����ҳ��
const int AddressSize = 12;     // ��ַ������

int PageAddress[] = {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2}; // ҳ���������

// ����ȫ��״̬
void resetState(int pageframe[], int pagetime[], int clockRef[], int& pagep, int& isfull, int& lostNum, 
               map<int,int>& timemp, map<int,int>& futuremp) {
    memset(pageframe, -1, sizeof(int) * PFRAMENUM); // ��ʼ��Ϊ-1��ʾ��
    memset(pagetime, 0, sizeof(int) * PFRAMENUM);
    memset(clockRef, 0, sizeof(int) * PFRAMENUM);
    pagep = 0;
    isfull = 0;
    lostNum = 0;
    timemp.clear();
    futuremp.clear();
}

// ����û��㷨(OPT)
void OPT() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM];
    int clockRef[PFRAMENUM]; // δʹ��
    int pagep = 0, isfull = 0, lostNum = 0;
    map<int, int> timemp, futuremp;
    resetState(pageframe, pagetime, clockRef, pagep, isfull, lostNum, timemp, futuremp);

    for (int i = 0; i < AddressSize; i++) {
        bool found = false;
        // ���ҳ���Ƿ������ڴ���
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == PageAddress[i]) {
                found = true;
                break;
            }
        }

        // ȱҳ����
        if (!found) {
            lostNum++;
            // ������п���ҳ��
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                pagep++;
                if (pagep == PFRAMENUM) isfull = 1;
            } 
            // ��Ҫ�滻ҳ��
            else {
                int replaceIdx = -1;
                int farthest = -1;
                
                // �ҳ�δ���ʱ�䲻�����ʵ�ҳ��
                for (int j = 0; j < PFRAMENUM; j++) {
                    int nextUse = INT_MAX;
                    // ������һ�η���λ��
                    for (int k = i + 1; k < AddressSize; k++) {
                        if (PageAddress[k] == pageframe[j]) {
                            nextUse = k;
                            break;
                        }
                    }
                    // ������Զ����λ��
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIdx = j;
                    }
                }
                // ִ���滻
                pageframe[replaceIdx] = PageAddress[i];
            }
        }

        // �����ǰҳ��״̬
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == -1) cout << "- ";
            else cout << pageframe[j] << " ";
        }
        if (!found) cout << "F"; // ���ȱҳ
        cout << endl;
        Sleep(300);
    }
    cout << "ȱҳ����: " << lostNum-PFRAMENUM << endl << endl;
}

// �Ƚ��ȳ��㷨(FIFO)
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
                    pagep = 0; // ����ָ�뿪ʼѭ��
                }
            } else {
                pageframe[pagep] = PageAddress[i];
                pagep = (pagep + 1) % PFRAMENUM; // ѭ��ָ��
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
    cout << "ȱҳ����: " << lostNum-PFRAMENUM << endl << endl;
}

// ������δʹ���㷨(LRU)
void LRU() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM]; // ��¼�������ʱ��
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
            pagetime[foundIdx] = i; // ���·���ʱ��
        } else {
            lostNum++;
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                pagetime[pagep] = i; // ���÷���ʱ��
                pagep++;
                if (pagep == PFRAMENUM) isfull = 1;
            } else {
                // �ҵ����δʹ�õ�ҳ��
                int oldest = INT_MAX;
                int replaceIdx = -1;
                for (int j = 0; j < PFRAMENUM; j++) {
                    if (pagetime[j] < oldest) {
                        oldest = pagetime[j];
                        replaceIdx = j;
                    }
                }
                pageframe[replaceIdx] = PageAddress[i];
                pagetime[replaceIdx] = i; // ���·���ʱ��
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
    cout << "ȱҳ����: " << lostNum-PFRAMENUM << endl << endl;
}

// ʱ���û��㷨(Clock)
void Clock() {
    int pageframe[PFRAMENUM];
    int pagetime[PFRAMENUM];
    int clockRef[PFRAMENUM]; // ����λ (1: ������ʹ�, 0: δ����)
    int pagep = 0, isfull = 0, lostNum = 0;
    map<int, int> timemp, futuremp;
    resetState(pageframe, pagetime, clockRef, pagep, isfull, lostNum, timemp, futuremp);
    int clockPtr = 0; // ʱ��ָ��

    for (int i = 0; i < AddressSize; i++) {
        bool found = false;
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == PageAddress[i]) {
                found = true;
                clockRef[j] = 1; // ���÷���λ
                break;
            }
        }

        if (!found) {
            lostNum++;
            if (!isfull) {
                pageframe[pagep] = PageAddress[i];
                clockRef[pagep] = 1; // ���÷���λ
                pagep++;
                if (pagep == PFRAMENUM) {
                    isfull = 1;
                    clockPtr = 0; // ��ʼ��ʱ��ָ��
                }
            } else {
                // �����滻ҳ��
                while (true) {
                    if (clockRef[clockPtr] == 0) {
                        // �ҵ�����λΪ0��ҳ��
                        pageframe[clockPtr] = PageAddress[i];
                        clockRef[clockPtr] = 1; // ������ҳ��ķ���λ
                        clockPtr = (clockPtr + 1) % PFRAMENUM; // �ƶ�ָ��
                        break;
                    } else {
                        // ����ڶ��λ���
                        clockRef[clockPtr] = 0;
                        clockPtr = (clockPtr + 1) % PFRAMENUM; // �ƶ�ָ��
                    }
                }
            }
        }

        // �����ǰ״̬
        for (int j = 0; j < PFRAMENUM; j++) {
            if (pageframe[j] == -1) cout << "- ";
            else cout << pageframe[j] << "(" << clockRef[j] << ") ";
        }
        if (!found) cout << "F";
        cout << endl;
        Sleep(300);
    }
    cout << "ȱҳ����: " << lostNum-PFRAMENUM << endl << endl;
}

int main() {
    while (true) {
        cout << "\n��ѡ��һ���㷨��OPT��FIFO��LRU��Clock��������q/Q�����˳�: ";
        string choice;
        cin >> choice;
        
        if (choice == "OPT") {
            cout << "\nOPT�㷨ģ��:\n";
            OPT();
        } else if (choice == "FIFO") {
            cout << "\nFIFO�㷨ģ��:\n";
            FIFO();
        } else if (choice == "LRU") {
            cout << "\nLRU�㷨ģ��:\n";
            LRU();
        } else if (choice == "Clock") {
            cout << "\nClock�㷨ģ��(������Ϊ����λ):\n";
            Clock();
        } else if (choice == "q" || choice == "Q") {
            break;
        } else {
            cout << "�������������ѡ��" << endl;
        }
    }
    return 0;
}
