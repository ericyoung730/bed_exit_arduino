#include "databaseStruct.h"
//===========================================
//		EulerDistance
//
//===========================================

//calculate EulerDistance between two points 每次計算一組
float EulerDistance(float v1[],  float v2[]){
	
	int i = 0;
	float powerSum = 0;
	for(i = 0;i < FEATURE_QUANTITY;i++){
		
		powerSum = pow(v1[i] - v2[i], 2) + powerSum;
		
	}
	return powerSum;	 
}
float cityblock(float v1[],  float v2[]){
  
  int i = 0;
  float citySum = 0;
  for(i = 0;i < FEATURE_QUANTITY;i++){
    
    citySum =(v1[i] - v2[i]) + citySum;
    
  }
  return citySum;   
}



//===========================================
//		Bubble_Sorting 
//
//===========================================

void bubbleSort(resultVector resultDistance[]){
	int i,j;
	resultVector temp;
	for(i = 0;i < K_OF_KNN ;i++){
		for(j = 0;j < K_OF_KNN - i;j++){
			
			if(resultDistance[j].result > resultDistance[j+1].result){
				temp = resultDistance[j];
				resultDistance[j] = resultDistance[j+1];
				resultDistance[j+1] = temp;
			}
		}
	}
	
}
//===========================================
//		Statistic Function 
//
//===========================================



double RMS_F(double SVM){
	
	double ans = 0;
	
	ans = 1/sqrt(224);	//test the effect
	ans = ans*SVM;
	return ans;
}
