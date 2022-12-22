#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define global variables used for counting quick-sort comparisons
#define INPUT_SIZE_ 10
#define N_OF_STATS_ 3

int total_perms_    = 0;
int counter_1_      = 0;
int counter_2_      = 0;
float percentiles_[N_OF_STATS_] = {0.2, 0.3, 0.4};
int max_comparison_possible_ = 0;

/**
 * Prints array of ints
*/
void printArray(int a[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d  ", a[i]);
    }

    printf("\n");
}

/**
 * Prints array of chars
*/
void printCharArray(char a[]) {
    for (int i = 0; i < strlen(a); ++i) {
        printf("%c", a[i]);
    }
}

/**
 * Swaps two values by pointers
*/
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

/**
 * Enforces that a[r] is not min nor max in sub-array
*/
void organizePivot(int a[], int p , int r) {
    if(p == r-1)
        return;

    if (a[r] == r+1 || a[r] == p+1) {
        if (a[r-1] == p+1 || a[r-1] == r+1){
            swap(&a[r-2], &a[r]);
        } else {
            swap(&a[r-1], &a[r]);
        }
    }
}

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

/**
 * Standard partition subroutine as described in the book
*/
int partition1(int a[], int p , int r) {
    int x = a[r];
    int i = p-1;
    int j,temp; 
    for(j=p; j<r;j++) {
        counter_1_++;

        if(a[j] <= x) {
            i = i + 1;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[r]);
    return i+1; 
}

/**
 * Standard partition subroutine as described in the book
*/
int partition2(int a[], int p , int r) {
    int x = a[r];
    int i = p-1;
    int j,temp; 
    for(j=p; j<r;j++) {
        counter_2_++;

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
        int q = partition1(a, p, r);
        
        // recursively invoke self
        quickSort1(a, p, q-1);
        quickSort1(a, q+1, r);
    }
}

/**
 * Additional Quick-Sort algorithm as specified in maman 13
*/
void quickSort2(int a[], int p , int r) {
    if(p < r) {
        // we want a[r] to not contain the min or max
        organizePivot(a,p,r);

        // standard quick-select procedure
        int q = partition2(a,p,r);
        quickSort2(a, p,q-1);
        quickSort2(a, q+1,r);
    }
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
void copyArray(int a[], int b[], int n) {
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

/**
 * Fill array with 0's
*/
void zeroOutArray(int a[], int size) {
    // int size = sizeof(a) / sizeof(a[0]);
    for (int i = 0; i < size; i++) {
        a[i] = 0;
    }
}

/**
 * Calculate and print the weighted averages
 * we use the known weighted average function that can be found in https://en.wikipedia.org/wiki/Weighted_arithmetic_mean
*/
void printStats(int stats[], float percentage, char algorithmName[]) {
    // define numerators helper variables
    float nAvg = 0, nBestAvg = 0, nWorstAvg = 0;

    // define denominators
    float dAvg = 0, dBestAvg = 0, dWorstAvg = 0;

    int bestAvgCounter = 0, worstAvgCounter = 0, remaining = 0;
    int limit_count = total_perms_ * percentage;  // where to stop counting

    for (int i = 0; i < max_comparison_possible_; i++) {
        // stats[i] = Wi (weight)
        // i        = Xi (value)

        // reverse counter
        int j = max_comparison_possible_ - 1 - i;

        // calculate full weighted average
        nAvg += stats[i] * i;
        dAvg += stats[i];

        // calculate best weighted average
        if (dBestAvg < limit_count) {
            if (dBestAvg + stats[i] > limit_count) {
                // in this case the next permutation_counter will exceed the limit_counter of the desired percentile
                remaining = (limit_count - dBestAvg);
                nBestAvg += remaining * i;
                dBestAvg += remaining;
            } else {
                nBestAvg += stats[i] * i;
                dBestAvg += stats[i];
            }
        }

        // calculate worst weighted average
        if (dWorstAvg < limit_count) {
            if (dWorstAvg + stats[j] > limit_count) {
                // in this case the next permutation_counter will exceed the limit_counter of the desired percentile
                remaining = (limit_count - dWorstAvg);
                nWorstAvg += remaining * j;
                dWorstAvg += remaining;
            } else {
                nWorstAvg += stats[j] * j;
                dWorstAvg += stats[j];
            }
        }
    }

    // print statistics in an elegant way
    printCharArray(algorithmName);
    printf("\t %f \t %f \t %f \t %f \n" , percentage, (nAvg / dAvg), (nBestAvg / dBestAvg), (nWorstAvg / dWorstAvg));
}

int main() {
    // initialize array
    int n = INPUT_SIZE_;
    int a[n];
    initializeArray(a, n);
    int i = 0;

    // initialize global variables
    total_perms_ = factorial(INPUT_SIZE_);

    // create arrays for counting performance statistics
    for (int j = 1; j < n; j++) {
        max_comparison_possible_ += j;
    }

    int stats_1_[max_comparison_possible_];
    int stats_2_[max_comparison_possible_];

    zeroOutArray(stats_1_, max_comparison_possible_);
    zeroOutArray(stats_2_, max_comparison_possible_);

    // execute each algorithm and save results for evaluation
    for (i = 0; i < (factorial(INPUT_SIZE_) - 1); i++) {
        // run algorithms and evaluate performance count
        compareQuickSelectVersions(a, n);

        stats_1_[counter_1_]++;
        stats_2_[counter_2_]++;

        counter_1_ = 0;
        counter_2_ = 0;

        generateNextPermutation(a, n);
    }

    // provide statistics per percentile
    printf("Algorithm \t Percentile \t Average \t\t Best-Average \t Worst-Average \n");
    for (i = 0; i < N_OF_STATS_; i++) {
        printStats(stats_1_, percentiles_[i], "Quick-Sort-1");
        printStats(stats_2_, percentiles_[i], "Quick-Sort-2");
    }

    return 0;
}