#ifndef DATABASE_STRUCTURE_

#define DATABASE_STRUCTURE_
#include <math.h>


#define FEATURE_QUANTITY 7
#define AC_NUMBER 9//for eample
#define AC_TYPE 3

#define K_OF_KNN 5  //odd number is much better


typedef struct{
  
  double DATABASE[FEATURE_QUANTITY];
  int ClusterType;
  
}vector;

//=======================================
//  DATABASE & ClusterType
//=======================================

//vector AC[AC_NUMBER];


//=======================================
//  Other Prototype & Function
//=======================================
 
typedef struct{
  
  double result;
  int Type;
  
}resultVector;


double EulerDistance(double v1[], double v2[]);
void bubbleSort(resultVector resultDistance[]);
int MainExecuteKNN(double RealTimeAC[]);

double RMS_F(double SVM);

#endif
