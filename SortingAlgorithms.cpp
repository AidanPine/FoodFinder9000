#include <iostream>
#include <string>
using namespace std;

// dummy class
class Restaurant
{
private:
public:

    int rating;
    float distance;
    string cuisine;
    string name;
    float simScore;


    // passes in the data set, the searched values, and the preferred way of sorting (distanceOrRating is true if distance is preferred)
    void setSimScores(Restaurant array[], float prefDistance, int prefRating, string prefCuisine, bool distanceOrRating)
    {
        // sets sim scores with a preference towards distance
        if(distanceOrRating)
        {
            for(int i = 0; i < sizeof(array); i++)
            {
                int simScoreCuisine = -1;
                if(array[i].name == prefCuisine)
                {
                    simScoreCuisine = 1;
                }
                array[i].simScore = (.75 * ((prefDistance - array[i].distance) / prefDistance)) + (.15 * ((array[i].rating - prefRating) / 5)) + (1 * simScoreCuisine);
            }
        }
        // sets sim scores with a preference towards rating
        else
        {
            for(int i = 0; i < sizeof(array); i++)
            {
                int simScoreCuisine = -1;
                if(array[i].name == prefCuisine)
                {
                    simScoreCuisine = 1;
                }
                array[i].simScore = (.15 * ((prefDistance - array[i].distance) / prefDistance)) + (.75 * ((array[i].rating - prefRating) / 5)) + (1 * simScoreCuisine);
            }
        }
    }
};

int partition(Restaurant array[], int low, int high)
{
    float pivot = array[low].simScore;
    int up = low;
    int down = high;
    while(up < down)
    {
        for(int i = up; i < high; i++)
        {
            if(array[up].simScore > pivot)
            {
                break;
            }
            up++;
        }
        for(int i = high; i > low; i--)
        {
            if(array[down].simScore < pivot)
            {
                break;
            }
            down--;
        }
        if(up < down)
        {
            swap(array[up], array[down]);
        }
    }
    swap(array[low], array[down]);
    return down;
}

void QuickSort(Restaurant array[], int low, int high)
{
    if(low >= high)
    {
        return;
    }
    if(low < high)
    {
        int pivot = partition(array, low, high);
        QuickSort(array, low, pivot - 1);
        QuickSort(array, pivot + 1, high);
    }
}

void Merge(Restaurant array[], int start, int mid, int end){
    int leftCount = mid - start + 1;
    int rightCount = end - mid;
    Restaurant left[leftCount];
    Restaurant right[rightCount];



    //Left and right subarrays
    for(int i = 0; i < leftCount; i++){
        left[i] = array[start + i];
    }
    for(int i = 0; i < rightCount; i++){
        right[i] = array[mid + 1 +i];
    }

    int leftIndex = 0;
    int rightIndex = 0;
    int arrIndex = start;
    //Initial merge instructions
    while(leftIndex < leftCount && rightIndex < rightCount){
        if(left[leftIndex].simScore > right[rightIndex].simScore){
            swap(array[arrIndex], left[leftIndex]);
            leftIndex++;
        }
        else{
            swap(array[arrIndex], right[rightIndex]);
            rightIndex++;
        }
        arrIndex++;
    }
    //Copy leftover values
    while(leftIndex < leftCount){
        swap(array[arrIndex], left[leftIndex]);
        leftIndex++;
        arrIndex++;
    }
    while(rightIndex < rightCount){
        swap(array[arrIndex], right[rightIndex]);
        rightIndex++;
        arrIndex++;
    }
}

void MergeSort(Restaurant array[], int start, int end){
    if(start < end) {
        int mid = (end + start) / 2;
        MergeSort(array, start, mid);
        MergeSort(array, mid + 1, end);
        Merge(array, start, mid, end);
    }
}

int main()
{
    return 0;
}
