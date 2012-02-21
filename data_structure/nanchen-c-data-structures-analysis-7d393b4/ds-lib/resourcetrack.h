/*
 * resource-track.h
 *
 *  Created on: Mar 22, 2011
 *      Author: nan
 */

#ifndef RESOURCE_TRACK_H_
#define RESOURCE_TRACK_H_

#include <time.h>
#include <stdbool.h>

#include "list.h"

unsigned long totalTime;
long totalSpace;
long maxSpace;
List resourceList;
enum ComplexityClass{N2, NLOGN, N, LOGN, N0};
struct Operation{
    char* name;
    char* path;
    List resourceList;
    enum ComplexityClass timeComplexity;
    enum ComplexityClass spaceComplexity;
    float timeConstant;
    float spaceConstant;
    float timeDeviation;
    float spaceDeviation;
    bool timeEqual;
    bool spaceEqual;
    int timeEqualList[20];
    int spaceEqualList[20];
};
struct Operation* operations[20];

void Resource_setMode(int mode);
void Resource_initilizeOperationArray();
void Resource_startTrack(const char* path, const char* msg);
void Resource_logTime(unsigned long time);
void Resource_logSpace(int space);
void Resource_clearData();
void Resource_storeData(unsigned int size);
struct CalcResult* Resource_analyse(unsigned int size, unsigned long time, long space);
void Resource_analyseSequence();
void Resource_writePlotScript(const char* scriptName, char* opList[], int opListLength, int type, int logScale);
void Resource_writeTableData(const char* tableName);
#endif /* RESOURCE_TRACK_H_ */
