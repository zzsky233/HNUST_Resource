#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstdio>
using namespace std;

const int MAX_REQUESTS = 9;
int requests[MAX_REQUESTS] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
const int start_pos = 100;

// 打印调度过程
void print_schedule(int path[], int moves[], int count, int total_seek) {
    printf("调度过程:\n");
    printf("起始磁道: %d\n", path[0]);
    
    for (int i = 1; i < count; i++) {
        printf("移动到磁道: %-3d \t横跨磁道数: %-3d\n", path[i], moves[i-1]);
    }
    
//    printf("总寻道长度: %d\n", total_seek);
    printf("平均寻道长度: %.2f\n\n", (double)total_seek / (count - 1));
}

// 1. 先来先服务算法 (FIFO)
void fifo() {
    printf("============== FIFO算法 ==============\n");
    int path[MAX_REQUESTS + 1] = {0}; // 路径数组
    int moves[MAX_REQUESTS] = {0};    // 每次移动距离
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    
    // 记录起始位置
    path[count++] = current;
    
    // 按请求顺序处理
    for (int i = 0; i < MAX_REQUESTS; i++) {
        int distance = abs(current - requests[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = requests[i];
        path[count++] = current;
    }
    
    print_schedule(path, moves, count, total_seek);
}

// 2. 最短寻道时间优先算法 (SSTF)
void sstf() {
    printf("============== SSTF算法 ==============\n");
    int path[MAX_REQUESTS + 1] = {0};
    int moves[MAX_REQUESTS] = {0};
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    int processed[MAX_REQUESTS] = {0}; // 标记已处理的请求
    
    path[count++] = current;
    
    // 处理所有请求
    for (int i = 0; i < MAX_REQUESTS; i++) {
        int min_dist = INT_MAX;
        int next_index = -1;
        
        // 查找最近的请求
        for (int j = 0; j < MAX_REQUESTS; j++) {
            if (!processed[j]) {
                int dist = abs(current - requests[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    next_index = j;
                }
            }
        }
        
        // 处理找到的请求
        moves[count - 1] = min_dist;
        total_seek += min_dist;
        current = requests[next_index];
        path[count++] = current;
        processed[next_index] = 1; // 标记为已处理
    }
    
    print_schedule(path, moves, count, total_seek);
}

// 3. 电梯扫描算法 (SCAN)
void scan() {
    printf("============== SCAN算法 ==============\n");
    int path[MAX_REQUESTS + 1] = {0};
    int moves[MAX_REQUESTS] = {0};
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    
    path[count++] = current;
    
    //分离小于和大于当前磁道的请求
    int left[MAX_REQUESTS] = {0}, right[MAX_REQUESTS] = {0};
    int left_count = 0, right_count = 0;
    
    for (int i = 0; i < MAX_REQUESTS; i++) {
        if (requests[i] < current) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }
    
    //排序
    sort(left, left + left_count, greater<int>());
    sort(right, right + right_count);              
    
    //向增加方向移动（处理右边请求）
    for (int i = 0; i < right_count; i++) {
        int distance = abs(current - right[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = right[i];
        path[count++] = current;
    }
    
    // 转向处理左边请求（降序处理）
    for (int i = 0; i < left_count; i++) {
        int distance = abs(current - left[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = left[i];
        path[count++] = current;
    }
    
    print_schedule(path, moves, count, total_seek);
}

// 4. 循环扫描算法 (C-SCAN)
void c_scan() {
    printf("============== C-SCAN算法==============\n");
    int path[MAX_REQUESTS + 1] = {0};
    int moves[MAX_REQUESTS] = {0};
    int total_seek = 0;
    int current = start_pos;
    int count = 0;
    
    path[count++] = current;
    
    // 分离小于和大于当前磁道的请求
    int left[MAX_REQUESTS] = {0}, right[MAX_REQUESTS] = {0};
    int left_count = 0, right_count = 0;
    
    for (int i = 0; i < MAX_REQUESTS; i++) {
        if (requests[i] < current) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }
    
    // 排序数组
    sort(left, left + left_count); // 升序排序（从小到大）
    sort(right, right + right_count); // 升序排序（从小到大）
    
    // 向增加方向移动（处理右边请求）
    for (int i = 0; i < right_count; i++) {
        int distance = abs(current - right[i]);
        moves[count - 1] = distance;
        total_seek += distance;
        current = right[i];
        path[count++] = current;
    }
    
    // 如果左边有请求，直接跳转到最小请求
    if (left_count > 0) {
        // 跳转到左侧最小请求
        int jump = abs(current - left[0]);
        moves[count - 1] = jump;
        total_seek += jump;
        current = left[0];
        path[count++] = current;
        
        // 继续向右处理剩余左侧请求
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
		cout<<"\n请选择一个算法进行展示（FIFO、SSTF、SCAN、C-SCAN），输入Q/q可退出"<<endl;
		string s;
		cin>>s;
		if(s=="FIFO") fifo();    // 先来先服务
	    else if(s=="SSTF") sstf();    // 最短寻道时间优先
	    else if(s=="SCAN") scan();    // 电梯扫描算法
	    else if(s=="C-SCAN") c_scan();  // 循环扫描算法
	    else if(s=="Q"||s=="q")break;
		else cout<<"输入错误，请重新选择"<<endl<<endl; 
	}

    return 0;
}
