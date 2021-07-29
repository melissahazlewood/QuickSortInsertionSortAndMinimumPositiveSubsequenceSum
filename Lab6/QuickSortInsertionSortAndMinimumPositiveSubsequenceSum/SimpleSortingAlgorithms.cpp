//
//  CECS328Lab6.cpp
//  Lab6
//
//  Created by Melissa Hazlewood on 12/3/19.
//  Copyright © 2019 Melissa Hazlewood. All rights reserved.
//

#include <iostream>
#include <cmath>

using namespace std;

double MPSS(double arr[], int i, int n); //i = first index; n = num elems
double MPSS_middle(double arr[], int left_i, int left_n, int right_i, int right_n);
double minPos(double a, double b, double c); //returns the minimum positive number
int partition(double * arr, int i, int n, double pivot);
void quickSort(double arr[], int i, int n);
double med_of_3(double n1, double n2, double n3);
void insertionSort(double arr[], int i, int n);
void printArray(double arr[], int i, int n);


int main()
{
    int n = 10;
    double arr[10] = {2, -3, 1, 4, -6, 10, -12, 5.2, 3.6, -8};
    
    cout << MPSS(arr, 0, n) << endl;
    
    
    
    return 0;
}


double MPSS(double arr[], int i, int n)
{
    //indices and subarray sizes so I don't get confused
    int left_i = i;
    int left_n = n/2;
    int right_i = n/2 + i;
    int right_n = n - n/2;
    
    double MPSS_left = 0; //MPSS for left subarray
    double MPSS_right = 0; //MPSS for right subarray
    double MPSS_mid = 0; //MPSS for subarrays that include elems from left and right subarrays
    double MPSS_final = arr[i]; //value to be returned
    
    if (n > 1)
    {
        MPSS_left = MPSS(arr, left_i, left_n);
        MPSS_right = MPSS(arr, right_i, right_n);
        MPSS_mid = MPSS_middle(arr, left_i, left_n, right_i, right_n);
        MPSS_final = minPos(MPSS_left, MPSS_right, MPSS_mid);
    }
    
    return MPSS_final;
}

double MPSS_middle(double arr[], int left_i, int left_n, int right_i, int right_n)
{
    int end;
    double s_min = 100000;
    int l; //left marker; used first to go through left subarr then later to go through left SUM subarr
    int r; //right marker
    int s_l = 1; //index in s_left
    int s_r = 1; //index in s_right
    
    double s_left[left_n]; //left subsequence sums
    double s_right[right_n]; //right subsequence sums
    
    //initialize left and right sum arrs
    l = left_i + left_n - 1; //last elem in left subsequence
    r = right_i; //first elem in right subsequence
    
    s_left[0] = arr[l];
    s_right[0] = arr[r];

    while (s_l < left_n)
    {
        s_left[s_l] = arr[l-s_l] + s_left[s_l-1];
        s_l++;
    }
    while (s_r < right_n)
    {
        s_right[s_r] = arr[r+s_r] + s_right[s_r-1];
        s_r++;
    }
    
    quickSort(s_left, 0, left_n); //get left sum subarr into ascending order
    quickSort(s_right, 0, right_n); //get right sum subarr into ascending order (we want descending ultimately for this one)
    
    //reversing the right subarray so that it's descending
    //actually nvm, let's just have the marker go from right to left for right subarr
    end = right_n+right_i-1;

    l = 0; //reusing these counters to iterate through the sum subarrs
    r = right_n - 1; //starting at the end to make the values descending
    
    while (l < left_n && r >= 0)
    {
        if (s_left[l] + s_right[r] < 0)
        {
            l++;
        }
        else if (s_left[l] + s_right[r] < s_min)
        {
            s_min = s_left[l] + s_right[r];
        }
        else
        {
            r--;
        }
    }
    
    return s_min;
}

double minPos(double a, double b, double c)
{
    double currMin;
    
    if (a <= 0) a = 100000;
    if (b <= 0) b = 100000;
    if (c <= 0) c = 100000;
    
    currMin = min(a, min(b, c));
    
    if (currMin == 100000) //returns 0 if none are positive, if some are greater than 100000 and others are negative (hopefully rare, but still possible), or if the minimum value really does happen to be exactly 100000
        return 0;
    else
        return currMin;
}

void quickSort(double arr[], int i, int n)
{
    double pivot;
    int pivotIndex;
    
    pivot = arr[n-1 + i];
    pivotIndex = partition(arr, i, n, pivot);
    
    if (n <= 5)
    {
        insertionSort(arr, i, n);
    }
    else
    {
        quickSort(arr, i, pivotIndex - i);
        quickSort(arr, pivotIndex, n - (pivotIndex-i));
    }
}


int partition(double * arr, int i, int n, double pivot)
{
    int m1 = i; //marker 1; starts at the beginning going right
    int m2 = i + n-2; //marker 2; starts at the end going left
    double buff; //for swapping
    
    while (m2 > m1) //markers stop moving when they pass or meet each other
    {
        while (arr[m1] <= pivot)
            m1++;
        while (arr[m2] > pivot && m2 > i-1) //marker 2 can't go beyond the beginning
            m2--;
        
        //swap
        if (arr[m1] > pivot && arr[m2] < pivot && m2 > -1 && m2 > m1) //only swap if the elements should be swapped
        {
            buff = arr[m1];
            arr[m1] = arr[m2];
            arr[m2] = buff;
            m1++;
            m2--;
        }
    } //end while
    
    if (arr[m1] > pivot)
    {
        arr[i + n-1] = arr[m1];
        arr[m1] = pivot;
        
    }
    else if (arr[m1 + 1] > pivot && (m1 + 1) < (n + i))
    {
        m1++;
        arr[i + n-1] = arr[m1];
        arr[m1] = pivot;
    }
    return m1;
}

void insertionSort(double arr[], int i, int n)
{
    double temp;
    int ind;
    
    for (int j = i+1; j < n+i; j++)
    {
        ind = j;
        while (arr[ind] < arr[ind-1] && ind > i) //while jth elem is larger than j-1th elem, swap them
        {
            temp = arr[ind];
            arr[ind] = arr[ind-1];
            arr[ind-1] = temp;
            ind--;
        }
    }
}


void printArray(double arr[], int i, int n)
{
    cout << "\t[ ";
    for (int j = i; j < i+n-1; j++)
    {
        cout << arr[j] << ", ";
    }
    cout << arr[i + n-1] << " ]\n";
}
