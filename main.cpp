#include <iostream>
#include <string>
using namespace std;

// dummy class
class Restraunt
{
private:
public:

    int rating;
    float distance;
    string cuisine;
    string name;
    float simScore;


    // passes in the data set, the searched values, and the preferred way of sorting (distanceOrRating is true if distance is preferred)
    void setSimScores(Restraunt array[], float prefDistance, int prefRating, string prefCuisine, bool distanceOrRating)
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

int partition(Restraunt array[], int low, int high)
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

void QuickSort(Restraunt array[], int low, int high)
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

int main()
{
    return 0;
}
