//Ryan Boscoe
//Lab 8
//April 9, 2023

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void heapify(int arr[], int n, int i)
{
	int largest = i;
	int temp = 0;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[largest])
	{
		largest = left;
	}

	if (right < n && arr[right] > arr[largest])
	{
		largest = right;
	}

	if (largest != i)
	{
		temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;

		heapify(arr, n, largest);
	}
}

void heapSort(int arr[], int n)
{
	int i;

	for (i = n / 2 - 1; i >= 0; i--)
	{
		heapify(arr, n, i);
	}

	for (i = n - 1; i >= 0; i--)
	{
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		heapify(arr, i, 0);
	}
}

void merge(int data[], int l, int m, int r)
{
	int i, j, k;
	int t1 = m - l + 1;
	int t2 = r - m;

	int *tempA = (int*) malloc(t1 * sizeof(int));
	int *tempB = (int*) malloc(t2 * sizeof(int));
	extraMemoryAllocated += t1 * sizeof(int) + t2 * sizeof(int);

	for (i = 0; i < t1; i++)
	{
		tempA[i] = data[l + i];
	}

    for (j = 0; j < t2; j++)
    {
		tempB[j] = data[m + 1 + j];
	}

    i = 0;
    j = 0;
    k = l;

	while (i < t1 && j < t2)
	{
		if (tempA[i] <= tempB[j])
		{
        	data[k] = tempA[i];
			i++;
		}
		else
		{
			data[k] = tempB[j];
			j++;
		}
		k++;
	}

	while (i < t1)
	{
		data[k] = tempA[i];
		i++;
		k++;
	}

	while (j < t2)
	{
		data[k] = tempB[j];
		j++;
		k++;
	}

	free(tempA);
	free(tempB);
}

void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		int middle = (l + r) / 2;
    	mergeSort(pData, l, middle);
    	mergeSort(pData, middle + 1, r);
    	merge(pData, l, middle, r);
	}
}

int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}
		fclose(inFile);
	}

	return dataSz;
}
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{

		if(i >= dataSz)
		{
			printf("\n\n");
			return;
		}
		printf("%d ",pData[i]);
	}
	printf("\n\t");

	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)malloc(sizeof(int)*dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}

}