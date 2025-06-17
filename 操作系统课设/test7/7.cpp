#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstdio>
using namespace std;

const int MAX_REQUESTS = 9;
int requests[MAX_REQUESTS] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
const int start_pos = 100;

// ��ӡ���ȹ���
void print_schedule(int path[], int moves[], int count, int total_seek) {
    printf("���ȹ���:\n");
    printf("��ʼ�ŵ�: %d\n", path[0]);
    
    for (int i = 1; i < count; i++) {
        printf("�ƶ����ŵ�: %-3d \t���ŵ���: %-3d\n", path[i], moves[i-1]);
    }
    
//    printf("��Ѱ������: %d\n", total_seek);
    printf("ƽ��Ѱ������: %.2f\n\n", (double)total_seek / (count - 1));
}

// 1. �����ȷ����㷨 (FIFO)
void fifo() {
    printf("============== FIFO�㷨 ==============\n");
    int path[MAX_REQUESTS + 1] = {0}; // ·������
    int moves[MAX_REQUESTS] = {0};    // ÿ���ƶ�����
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    
    // ��¼��ʼλ��
    path[count++] = current;
    
    // ������˳����
    for (int i = 0; i < MAX_REQUESTS; i++) {
        int distance = abs(current - requests[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = requests[i];
        path[count++] = current;
    }
    
    print_schedule(path, moves, count, total_seek);
}

// 2. ���Ѱ��ʱ�������㷨 (SSTF)
void sstf() {
    printf("============== SSTF�㷨 ==============\n");
    int path[MAX_REQUESTS + 1] = {0};
    int moves[MAX_REQUESTS] = {0};
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    int processed[MAX_REQUESTS] = {0}; // ����Ѵ��������
    
    path[count++] = current;
    
    // ������������
    for (int i = 0; i < MAX_REQUESTS; i++) {
        int min_dist = INT_MAX;
        int next_index = -1;
        
        // �������������
        for (int j = 0; j < MAX_REQUESTS; j++) {
            if (!processed[j]) {
                int dist = abs(current - requests[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    next_index = j;
                }
            }
        }
        
        // �����ҵ�������
        moves[count - 1] = min_dist;
        total_seek += min_dist;
        current = requests[next_index];
        path[count++] = current;
        processed[next_index] = 1; // ���Ϊ�Ѵ���
    }
    
    print_schedule(path, moves, count, total_seek);
}

// 3. ����ɨ���㷨 (SCAN)
void scan() {
    printf("============== SCAN�㷨 ==============\n");
    int path[MAX_REQUESTS + 1] = {0};
    int moves[MAX_REQUESTS] = {0};
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    
    path[count++] = current;
    
    //����С�ںʹ��ڵ�ǰ�ŵ�������
    int left[MAX_REQUESTS] = {0}, right[MAX_REQUESTS] = {0};
    int left_count = 0, right_count = 0;
    
    for (int i = 0; i < MAX_REQUESTS; i++) {
        if (requests[i] < current) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }
    
    //����
    sort(left, left + left_count, greater<int>());
    sort(right, right + right_count);              
    
    //�����ӷ����ƶ��������ұ�����
    for (int i = 0; i < right_count; i++) {
        int distance = abs(current - right[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = right[i];
        path[count++] = current;
    }
    
    // ת����������󣨽�����
    for (int i = 0; i < left_count; i++) {
        int distance = abs(current - left[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = left[i];
        path[count++] = current;
    }
    
    print_schedule(path, moves, count, total_seek);
}

// 4. ѭ��ɨ���㷨 (C-SCAN)
void c_scan() {
    printf("============== C-SCAN�㷨==============\n");
    int path[MAX_REQUESTS + 1] = {0};
    int moves[MAX_REQUESTS] = {0};
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    
    path[count++] = current;
    
    // ����С�ںʹ��ڵ�ǰ�ŵ�������
    int left[MAX_REQUESTS] = {0}, right[MAX_REQUESTS] = {0};
    int left_count = 0, right_count = 0;
    
    for (int i = 0; i < MAX_REQUESTS; i++) {
        if (requests[i] < current) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }
    
    // ��������
    sort(left, left + left_count); // �������򣨴�С����
    sort(right, right + right_count); // �������򣨴�С����
    
    // �����ӷ����ƶ��������ұ�����
    for (int i = 0; i < right_count; i++) {
        int distance = abs(current - right[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = right[i];
        path[count++] = current;
    }
    
    // ������������ֱ����ת����С����
    if (left_count > 0) {
        // ��ת�������С����
        int jump = abs(current - left[0]);
        moves[count - 1] = jump;
        total_seek += jump;
        current = left[0];
        path[count++] = current;
        
        // �������Ҵ���ʣ���������
        for (int i = 1; i < left_count; i++) {
            int distance = abs(current - left[i]);
            moves[count - 1] = distance;
            total_seek += distance;
            current = left[i];
            path[count++] = current;
        }
    }
    
    print_schedule(path, moves, count, total_seek);
}

int main() {
	while(1)
	{
		cout<<"\n��ѡ��һ���㷨����չʾ��FIFO��SSTF��SCAN��C-SCAN��������Q/q���˳�"<<endl;
		string s;
		cin>>s;
		if(s=="FIFO") fifo();    // �����ȷ���
	    else if(s=="SSTF") sstf();    // ���Ѱ��ʱ������
	    else if(s=="SCAN") scan();    // ����ɨ���㷨
	    else if(s=="C-SCAN") c_scan();  // ѭ��ɨ���㷨
	    else if(s=="Q"||s=="q")break;
		else cout<<"�������������ѡ��"<<endl<<endl; 
	}

    return 0;
}
