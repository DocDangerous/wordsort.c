//Caleb Harris, ID 5439602, P5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct tree{
    char word[21];
    int freq;
    int depth;
    int arraytag;
    struct tree *left;
    struct tree *right;
} tree;

tree* insert(tree* root, char newword[], int depthtracker, int nodect[]);
void query(tree* root, char newword[]);
void buildArray(tree** wordlist, tree* root);
void printArray(tree** wordlist, int nodect);
void freeArray(tree** wordlist, int nodect);
void swap(tree* a, tree* b);
int randMedian(tree** array, int low, int high);
int partition(tree** vals, int low, int high);
int is_sorted(tree** array, int low, int high);
void quicksort(tree** array, int low, int high);
void insertionSort(tree** array, int low, int high);
void insertionSort2(tree** array, int low, int high);

int main()
{
    //Seed random for median-finding in partition function
    srand(time(0));

    //Get number of words
    int n;
    scanf("%d", &n);
    tree* root = NULL;

    //Tracks number of unique words
    int nodect[1];
    nodect[0] = 0;

    //Loop for Part I
    for(int i=0; i<n; i++) {

        //Get query type and corresponding word
        int a;
        char newword[21];
        scanf("%d", &a);
        scanf("%s", newword);

        //Type 1 request
        if(a == 1) {
            root = insert(root, newword, 0, nodect);
        }

        //Type 2 request
        else {
            if(nodect[0] == 0) {
              printf("-1 -1\n");
            }
            else {
              query(root, newword);
            }
        }
    }

    //Part 2, first build an array of pointers to each node

    //Case where no nodes were inserted
    if(nodect[0] == 0) {
      return 0;
    }

    //Build array
    tree** wordlist;
    wordlist = malloc(nodect[0] * sizeof(tree *));
    buildArray(wordlist, root);

    //Sort by frequency lowest to smallest
    quicksort(wordlist, 0, nodect[0]-1);

    //Alphabetize contents if tied for frequency
    insertionSort2(wordlist, 0, nodect[0]-1);

    //Print results in reverse
    printArray(wordlist, nodect[0]);

    //Free memory
    freeArray(wordlist, nodect[0]);

}

//Insert a word into the tree, tracking how deep into the tree each recursive call is
tree* insert(tree* root, char newword[], int depthtracker, int nodect[])
{
    //Create new node if spot is empty
    if(root==NULL) {
        tree* temp = malloc(sizeof(tree));
        strcpy(temp->word, newword);
        temp->freq = 1;
        temp->depth = depthtracker;
        temp->arraytag = nodect[0];
        temp->left = NULL;
        temp->right = NULL;
        nodect[0]++;
        return temp;
    }

    //If word is already present, simply increment its frequency and return
    if(strcmp(newword, root->word) == 0) {
        root->freq++;
        return root;
    }

    //Recursively insert on left if word is alphabetically lower
    if(strcmp(newword, root->word) < 0) {
        root->left = insert(root->left, newword, depthtracker+1, nodect);
    }

    //Recursively insert on right if word is alphabetically greater
    else {
        root->right = insert(root->right, newword, depthtracker+1, nodect);
    }

    //Collapse recursion
    return root;
}

//Searches for a word and prints its frequency and depth
void query(tree* root, char newword[])
{
    //If word does not exist
    if(root==NULL) {
        printf("-1 -1\n");
        return;
    }

    //Print word's frequency and depth
    if(strcmp(newword, root->word) == 0) {
        printf("%d %d\n", root->freq, root->depth);
        return;
    }

    //Search left
    if(strcmp(newword, root->word) < 0) {
        query(root->left, newword);
    }

    //Search right
    else {
        query(root->right, newword);
    }

    //Collapse recursion
    return;
}

//Gets a pointer to each node in index corresponding to the node's arraytag value
void buildArray(tree** wordlist, tree* root)
{
    if(root!=NULL) {
        buildArray(wordlist, root->left);
        wordlist[root->arraytag] = root;
        buildArray(wordlist, root->right);
    }

    return;
}

//Prints word and frequency for each node
void printArray(tree** wordlist, int nodect)
{
    for(int i=(nodect-1); i>=0; i--) {
        printf("%s %d\n", wordlist[i]->word, wordlist[i]->freq);
    }
}

//Free memory
void freeArray(tree** wordlist, int nodect)
{
    for(int i =0; i<nodect; i++) {
        free(wordlist[i]);
    }
}

//Function to swap two pointers
void swap(tree* a, tree* b) {

     tree temp = *a;
     *a = *b;
     *b = temp;
}

//Function to check if an array is in ascending order or all the same value
int is_sorted(tree** array, int low, int high) {

    int i;

    // Return false if any adjacent pair is out of order.
    for (i=low; i<high-1; i++) {
        if (array[i]->freq > array[i+1]->freq) {
            return 0;
        }
    }

    return 1;
}

//Function to pull a median of three random indices for use as partition
int randMedian(tree** array, int low, int high) {

    //Generate random indices
    int r1, r2, r3;
    r1 = low + rand()%(high-low+1);
    r2 = low + rand()%(high-low+1);
    r3 = low + rand()%(high-low+1);

    //Check to find median value in each corresponding index
    if((array[r1]->freq <= array[r2]->freq && array[r1]->freq >= array[r3]->freq) || (array[r1]->freq <= array[r3]->freq && array[r1]->freq >= array[r2]->freq)) {
        return r1;
    }
    else {
        if((array[r2]->freq <= array[r1]->freq && array[r2]->freq >= array[r3]->freq) || (array[r2]->freq <= array[r3]->freq && array[r2]->freq >= array[r1]->freq)) {
            return r2;
        }
        else {
            return r3;
        }
    }
}

//Function to find/use partition to sort array
int partition(tree** vals, int low, int high) {

    int temp;
    int i, lowpos;

    //Call function to find median value of random indices and set it as new low
    i = randMedian(vals, low, high);
    swap(vals[low], vals[i]);

	//Store the index of the partition element.
	lowpos = low;

	//Update the low pointer.
	low++;

	//Run the partition so long as the low and high counters don't cross.
	while (low <= high) {

		//Move the low pointer until we find a value too large for this side.
		while (low <= high && vals[low]->freq <= vals[lowpos]->freq) low++;

		//Move the high pointer until we find a value too small for this side.
		while (high >= low && vals[high]->freq > vals[lowpos]->freq) high--;

		//Now that we've identified two values on the wrong side, swap them.
		if (low < high)
		   swap(vals[low], vals[high]);
	}

	//Swap the partition element into it's correct location.
	swap(vals[lowpos], vals[high]);

    //Return the index of the partition element.
	return high;
}

//Quicksort function
void quicksort(tree** array, int low, int high) {

    //If array chunk is small enough, just do an insertion sort and collapse the recursion
    if((high - low) <= 10) {
        insertionSort(array, low, high);
        return;
    }

    //Check if array is already sorted
    if(is_sorted(array, low, high)) return;

    //Recursively sort the array until it's broken into a small enough segment to do an insertion sort
    int split = partition(array,low,high);
    quicksort(array,low,split-1);
    quicksort(array,split+1,high);
}

//Function to do an insertion sort for small arrays
void insertionSort(tree** array, int low, int high) {

     int i,j;

     //Case if array is of size 3. Was having fringe issues with that size.
     if(high == 2 && low == 0) {
        if(array[1]->freq > array[0]->freq && array[1]->freq > array[2]->freq) {
            swap(array[1], array[2]);
            if(array[1]->freq < array[0]->freq) {
                swap(array[0], array[1]);
            }
            return;
        }
     }

     //Loop through each element to insert.
     for(i=(low+1); i<=high; i++) {
         j=i;
         //Continue swapping the element until it hits the correct location.
         while (j > 0 && array[j]->freq < array[j-1]->freq) {
               swap(array[j], array[j-1]);
               j--;
         }
     }
}

//Reverse alphabetical insertion sort
void insertionSort2(tree** array, int low, int high) {

     int i,j;

     //Case if array is of size 3. Was having fringe issues with that size.
     if(high == 2 && low == 0) {
        if(strcmp(array[1]->word, array[0]->word) < 0 && strcmp(array[1]->word, array[2]->word) < 0) {
            swap(array[1], array[2]);
            if(strcmp(array[1]->word, array[0]->word) > 0) {
                swap(array[0], array[1]);
            }
            return;
        }
     }

     //Loop through each element to insert.
     for(i=(low+1); i<=high; i++) {
         j=i;
         //Continue swapping the element until it hits the correct location.
        while (j > 0 && array[j]->freq == array[j-1]->freq) {
               if(strcmp(array[j]->word, array[j-1]->word) > 0) {
                    swap(array[j], array[j-1]);
               }
               j--;
         }
     }
}
