#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#define N 10
void merge(int arr[],int low, int mid, int high){
	int n1 = mid-low+1,n2=high-mid,i,j,k;
	int left[n1],right[n2];
	for(i=0;i<n1;i++)
		left[i]=arr[low+i];
	for(i=0;i<n2;i++)
		right[i]=arr[mid+1+i];
	k=low;
	i=0,j=0;
	while(i<n1 && j<n2){
		if(left[i]<=right[j]){
			arr[k++]=left[i++];
		}
		else
			arr[k++]=right[j++];
	}
	while(i<n1)
		arr[k++]=left[i++];
	while(j<n2)
		arr[k++]=right[j++];
}

void mergeSort(int arr[],int low,int high){
	if(low<high){
		int mid = (low+high)/2;
		#pragma omp task shared(arr) firstprivate(low,mid) final(low<high)
			mergeSort(arr,low,mid);
		#pragma omp task shared(arr) firstprivate(low,mid,high) final(low<high)
			mergeSort(arr,mid+1,high);
		#pragma omp taskwait
		merge(arr,low,mid,high);
	}
}
int main(){
	int arr[N],i;
	for (i=0;i<N;i++)
		arr[i]=rand()%9999;//%(N-1);
	#pragma omp parallel
	{
		#pragma omp single
			mergeSort(arr,0,N-1);
	}
	for(i=0;i<N;i++)
		printf("%d -> %d\n",i,arr[i]);
	return 0;
}