#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// define global variables used for counting quick-sort comparisons
int comparison_count_1_ = 0;
int comparison_count_2_ = 0;
int input_size_         = 4;

int main();

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

void organize_pivot(int a[], int p , int r){
   if(p == r-1);
    return;
    if (a[r] == r+1 || a[r] == p+1){
        if (a[r-1] == p+1 || a[r-1] == r+1){
            swap(&a[r-2], &a[r]);
        }
        else { 
            swap(&a[r-1], &a[r]);
        }
    }
}
int partition(int a[], int p , int r);

/**
 * Reverses sub-array between indices p and r
*/
void reverse(int a[], int p, int r) {
    while (p < r) {
        swap(&a[p], &a[r]);
        p++;
        r--;
    }
}

/**
 * Apply the next permutation on a[]
*/
void generateNextPermutation(int a[], int n) {
    int i, j, c;
    i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) {
        i--;
    }

    if (i < 0) return;

    c = i + 1;
    j = i + 2;
    for (j; j < n; j++) {
        if (a[i] < a[j] && a[j] < a[c]) {
            c = j;
        }
    }

    swap(&a[i], &a[c]);
    reverse(a, i + 1, n - 1);
}



int partition(int a[], int p , int r) {
    int x = a[r];
    int i = p-1;
    int j,temp; 
    for(j=p; j<r;j++) {
        if(a[j] <= x) {
            i = i + 1;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[r]);
    return i+1; 
}

/**
 * Quick-Sort algorithm 1 as specified in the book page 122
*/
void quickSort1(int a[], int p , int r) {
    if(p < r) {
        // partition on pivot
        int q = partition(a, p, r);
        
        // recursively invoke self
        quickSort1(a,p,q-1);
        quickSort1(a,q+1,r);
    }
}

/**
 * Additional Quick-Sort algorithm as specified in maman 13
*/
void quickSort2(int a[], int p , int r) {
    if(p < r) {
        // we want a[r] to not contain the min or max
        organize_pivot(a,p,r);

        // standard quick-select procedure
        int q = partition(a,p,r);
        quickSort2(a, p,q-1);
        quickSort2(a, q+1,r);
    }
}

/**
 * Prints array
*/
void printArray(int a[], int n) {
  for (int i = 0; i < n; ++i) {
    printf("%d  ", a[i]);
  }
  printf("\n");
}

/**
 * Assign values 1,2,..,n to an array of size n
*/
void initializeArray(int a[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        a[i] = i + 1;
    }
}

/**
 * Duplicates contents of a into b (a and b must be of equal size)
*/
void copyArray(int a[], int *b, int n) {
    // copy values
    for (int i = 0; i < n; i++) {
        b[i] = a[i];
    }
}

/**
 * Execute the two quick-sory algorithms on the given permutation
*/
void compareQuickSelectVersions(int a[], int n) {
    // dulicate arrays as new variable input for each algorithm
    int arr_quick1[n];
    int arr_quick2[n];

    copyArray(a, arr_quick1, n);
    copyArray(a, arr_quick2, n);

    quickSort1(arr_quick1, 0, n-1);   // todo make sure indices coorespond to permutation 1..n
    quickSort2(arr_quick2, 0, n-1);
}

/**
 * Calculate and return factorial of n
*/
int factorial(int n) {
    int result = n;

    for (int i = n - 1; i > 0; i--) {
        result *= i;
    }

    return result;
}

int main() {
    // initialize array
    int n = input_size_;
    int a[n];
    initializeArray(a, n);

    // execute each algorithm and save results for evaluation
    for (int i = 0; i < (factorial(input_size_) - 1); i++) {
        compareQuickSelectVersions(a, n);
        generateNextPermutation(a, n);
        printArray(a, n);
    }
    
    return 0;
}