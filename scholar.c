//Caleb Harris, ID 5439602, P4

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void quicksort(int* numbers, int low, int high);
int partition(int* vals, int low, int high);
void swap(int* a, int* b);
int is_sorted(int values[], int low, int high);
void insertionSort(int* array, int low, int high);
int randMedian(int* array, int low, int high);
long long bookCombo(int* bookList, int numBooks, long long maxPages);
void print(int* numbers, int length);

//Main
int main()
{
    //Seed random for median-finding in partition function
    srand(time(0));

    //Input initialization
    int numBooks;
    long long maxPages;
    scanf("%d", &numBooks);
    scanf("%lld", &maxPages);

    //Array malloc
    int* bookList;
    bookList = malloc(numBooks * sizeof(int));

    //Get number of pages for each book
    for(int i = 0; i < numBooks; i++) {
        scanf("%d", &bookList[i]);
    }

    //Sort from lowest to highest
    quicksort(bookList, 0, numBooks-1);

    //Debug print
    //print(bookList, numBooks);

    //Calculate max number of different books that can be read within the page limit
    long long booksRead;
    booksRead = bookCombo(bookList, numBooks, maxPages);

    //Free
    free(bookList);

    //Print result
    printf("%lld", booksRead);
}

//Quicksort function
void quicksort(int* numbers, int low, int high) {

    //Check if array is already sorted
    if(is_sorted(numbers, low, high)) return;

    //If array chunk is small enough, just do an insertion sort and collapse the recursion
    if((high - low) <= 10) {
        insertionSort(numbers, low, high);
        return;
    }

    //Recursively sort the array until it's broken into a small enough segment to do an insertion sort
    int split = partition(numbers,low,high);
    quicksort(numbers,low,split-1);
    quicksort(numbers,split+1,high);
}

//Function to do an insertion sort for small arrays
void insertionSort(int* array, int low, int high) {

     int i,j;

     //Loop through each element to insert.
     for(i=(low+1); i<=high; i++) {
         j=i;
         //Continue swapping the element until it hits the correct location.
         while (j > 0 && array[j] < array[j-1]) {
               swap(&array[j], &array[j-1]);
               j--;
         }
     }
}

//Function to find/use partition to sort array
int partition(int* vals, int low, int high) {

    int temp;
    int i, lowpos;

    //Call function to find median value of random indices and set it as new low
    i = randMedian(vals, low, high);
    swap(&vals[low], &vals[i]);

	//Store the index of the partition element.
	lowpos = low;

	//Update the low pointer.
	low++;

	//Run the partition so long as the low and high counters don't cross.
	while (low <= high) {

		//Move the low pointer until we find a value too large for this side.
		while (low <= high && vals[low] <= vals[lowpos]) low++;

		//Move the high pointer until we find a value too small for this side.
		while (high >= low && vals[high] > vals[lowpos]) high--;

		//Now that we've identified two values on the wrong side, swap them.
		if (low < high)
		   swap(&vals[low], &vals[high]);
	}

	//Swap the partition element into it's correct location.
	swap(&vals[lowpos], &vals[high]);

    //Return the index of the partition element.
	return high;
}

//Function to check if an array is in ascending order or all the same value
int is_sorted(int values[], int low, int high) {

    int i;

    // Return false if any adjacent pair is out of order.
    for (i=low; i<high-1; i++)
        if (values[i] > values[i+1])
            return 0;

    return 1;
}

//Function to pull a median of three random indices for use as partition
int randMedian(int* array, int low, int high) {

    //Generate random indices
    int r1, r2, r3;
    r1 = low + rand()%(high-low+1);
    r2 = low + rand()%(high-low+1);
    r3 = low + rand()%(high-low+1);

    //Check to find median value in each corresponding index
    if((array[r1] <= array[r2] && array[r1] >= array[r3]) || (array[r1] <= array[r3] && array[r1] >= array[r2])) {
        return r1;
    }
    else {
        if((array[r2] <= array[r1] && array[r2] >= array[r3]) || (array[r2] <= array[r3] && array[r2] >= array[r1])) {
            return r2;
        }
        else {
            return r3;
        }
    }
}

//Function to swap two pointers
void swap(int* a, int* b) {

     int temp = *a;
     *a = *b;
     *b = temp;
}

//Calculate max number of different books that can be read within the page limit
long long bookCombo(int* bookList, int numBooks, long long maxPages) {

    long long sum = 0;
    long long temp = 0;
    long long testval = 0;
    int i = 0;

    //While inside the array
    while(i < numBooks) {
        //Store index as temp
        temp = bookList[i];
        testval = sum + temp;

        //If current sum of pages is more than the max, return.
        if(testval > maxPages) return i;

        //Increment
        sum = testval;
        i++;
    }

    //Default return if no maximum is reached from the array of books
    return i;
}

//Debug function
void print(int* numbers, int length) {

    int i;
    for (i=0;i<length;i++)
        printf("%d ", numbers[i]);
    printf("\n");
}
