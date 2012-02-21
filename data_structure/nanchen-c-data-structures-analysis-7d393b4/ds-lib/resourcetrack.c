#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "resourcetrack.h"

// calculation result
struct CalcResult {
	float power;
	float cn2;
	float cnlog2n;
	float cnlogn;
	float cn;
	float clogn;
	//    float c1;

	float cn2Space;
	float cnlognSpace;
	float cnSpace;
	float clognSpace;
};
// raw data about resource consumption
struct Resource {
	unsigned int size;
	unsigned long time;
	long space;
	struct CalcResult* calcResult;
};

const char* target;

clock_t startTime;
clock_t endTime;

bool logAllowed = true;

void printList(const List l) {
	logAllowed = false;
	ListPosition p = List_header(l);

	if (List_isEmpty(l))
		printf("Empty list\n");
	else {
		do {
			p = List_advance(p);
			struct Resource* res = (struct Resource*) List_retrieve(p);
			printf("(%d, %ld, %ld)  ", res->size, res->time, res->space);
		} while (!List_isLast(p, l));
		printf("\n");
	}
	logAllowed = true;
}

struct Operation** pointer = NULL;
int operationCount = 0;

int timeMode = 0;

void Resource_setMode(int mode) {
	timeMode = mode;
}

void Resource_initilizeOperationArray() {
	int i;
	operationCount = 0;

	for (i = 0; i < 20; i++) {
		if (operations[i] != NULL)
			free(operations[i]);
		operations[i] = NULL;
	}
}
void Resource_startTrack(const char* path, const char* msg) {
	printf("Start tracking time/space usage for %s\n", msg);
	struct Operation* op = (struct Operation*) malloc(sizeof(struct Operation));
	op->path = path;
	op->name = msg;
	operations[operationCount] = op;

	Resource_clearData();
	target = msg;

	startTime = clock();

	logAllowed = false;
	if (resourceList != NULL) {
		List_deleteList(resourceList);
	}
	resourceList = List_makeEmpty(NULL);
	logAllowed = true;
}

void Resource_logTime(unsigned long time) {
	if (logAllowed)
		totalTime += time;
}

void Resource_logSpace(int space) {
	if (logAllowed) {
		// printf("space logged: %d\n", space);
		totalSpace += space;
		if (totalSpace > maxSpace)
			maxSpace = totalSpace;
	}
}

ListPosition position = NULL;

void Resource_clearData() {
	totalTime = 0;
	//    printf("clear totalSpace = %d\n", totalSpace);
	totalSpace = 0;
	maxSpace = 0;
	logAllowed = true;
	startTime = clock();
}

void Resource_storeData(unsigned int size) {
	logAllowed = false;
	if (position == NULL)
		position = List_header(resourceList);

	struct Resource* res = (struct Resource *) malloc(sizeof(struct Resource));
	res->size = size;
	// time
	if (timeMode == 0)
		res->time = totalTime;
	if (timeMode == 1) {
		long diff = (clock() - startTime) * 100;
		res->time = diff;
		//  printf("now = %ld, startTime = %ld, diff = %ld, CLOCKS_PER_SEC = %ld\n", clock(),startTime, diff, CLOCKS_PER_SEC);
	}
	// space
	//    struct mstats* mem = mstats();
	//    printf("bytes used: %d\n",mem);
	if (maxSpace > 0 && totalSpace >= 0)
		res->space = maxSpace;
	else
		res->space = totalSpace;

	List_insert(res, resourceList, position);
	position = List_advance(position);

	//clear
	Resource_clearData();
}

void printCalcRet(const char* name, struct CalcResult ret) {
	printf(
			"\t%s:\tc n2 = %.2f, c nlog^2n = %.2f, c nlogn = %.2f, c n = %.2f, clogn = %.2f\tc n2 space = %.2f, cnlogn space = %.2f, cn space = %.2f, clogn space = %.2f\n",
			name, ret.cn2, ret.cnlog2n, ret.cnlogn, ret.cn, ret.clogn,
			ret.cn2Space, ret.cnlognSpace, ret.cnSpace, ret.clognSpace);

}

//analytical results
void Resource_analyseSequence() {
	logAllowed = false;

	ListPosition p = List_header(resourceList);
	if (List_isEmpty(resourceList)) {
		printf("Empty list\n");
		logAllowed = true;
		return;
	}

	// ----------analyse raw data of one test, calc mean of constants------------
	int count = 0;
	const struct CalcResult INITILIZED = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0 };
	struct CalcResult sum = INITILIZED;
	struct CalcResult mean = INITILIZED;
	struct CalcResult sumDeviation = INITILIZED;
	struct CalcResult deviation = INITILIZED;

	unsigned long lastTime = 0l;
	long lastSpace = 0l;
	bool constant = true;
	bool constantSpace = true;
	do {
		p = List_advance(p);

		//calc data
		struct Resource* res = (struct Resource*) List_retrieve(p);
		if (count == 0) {
			lastTime = res->time;
			lastSpace = res->space;
		}
		struct CalcResult* calcRet = Resource_analyse(res->size, res->time,
				res->space);
		printf("(%d, %ld, %ld)", res->size, res->time, res->space);
		printCalcRet("r", *calcRet);

		res->calcResult = calcRet;

		//sum of constants
		sum.cn2 += calcRet->cn2;
		sum.cnlog2n += calcRet->cnlog2n;
		sum.cnlogn += calcRet->cnlogn;
		sum.cn += calcRet->cn;
		sum.clogn += calcRet->clogn;
		sum.cn2Space += calcRet->cn2Space;
		sum.cnlognSpace += calcRet->cnlognSpace;
		sum.cnSpace += calcRet->cnSpace;
		sum.clognSpace += calcRet->clognSpace;

		// constant time/space check
		constant &= (res->time == lastTime);
		constantSpace &= (res->space == lastSpace);

		// update vars
		lastTime = res->time;
		count++;
	} while (!List_isLast(p, resourceList));

	if (constant && constantSpace) {
		printf(
				"constant time & space! constant time = %ld, constant space = %ld\n",
				lastTime, lastSpace);
		//return;
	}

	// mean
	mean.cn2 = sum.cn2 / count;
	mean.cnlog2n = sum.cnlog2n / count;
	mean.cnlogn = sum.cnlogn / count;
	mean.cn = sum.cn / count;
	mean.clogn = sum.clogn / count;
	mean.cn2Space = sum.cn2Space / count;
	mean.cnlognSpace = sum.cnlognSpace / count;
	mean.cnSpace = sum.cnSpace / count;
	mean.clognSpace = sum.clognSpace / count;
	printCalcRet("constant mean", mean);

	// ---------Calc variance of constants-----------
	struct Operation* op = operations[operationCount];

	p = List_header(resourceList);
	// open file
	char data[50];
	char filePath[50] = "./plot/";
	strcat(filePath, op->path);
	strcat(filePath, "/");
	mkdir(filePath, 7777);
	strcat(filePath, target);
	strcat(filePath, ".dat");
	FILE *dataFile = fopen(filePath, "w+");
	fputs("#size  time  space\n", dataFile);
	do {
		// sum variance
		p = List_advance(p);
		struct Resource* res = (struct Resource*) List_retrieve(p);
		struct CalcResult* ret = res->calcResult;
		sumDeviation.cn2 += pow((ret->cn2 - mean.cn2), 2) / count;
		sumDeviation.cnlog2n += pow((ret->cnlog2n - mean.cnlog2n), 2) / count;
		sumDeviation.cnlogn += pow((ret->cnlogn - mean.cnlogn), 2) / count;
		sumDeviation.cn += pow((ret->cn - mean.cn), 2) / count;
		sumDeviation.clogn += pow((ret->clogn - mean.clogn), 2) / count;

		sumDeviation.cn2Space += pow((ret->cn2Space - mean.cn2Space), 2)
				/ count;
		sumDeviation.cnlognSpace += pow((ret->cnlognSpace - mean.cnlognSpace),
				2) / count;
		sumDeviation.cnSpace += pow((ret->cnSpace - mean.cnSpace), 2) / count;
		sumDeviation.clognSpace += pow((ret->clognSpace - mean.clognSpace), 2)
				/ count;

		//        printf(" cnlogn: %f = , sum = %f\n",pow((ret->cnlogn - mean.cnlogn),2) / count, sumDeviation.cnlogn);
		// write to data file
		sprintf(data, "%d  %lu  %ld\n", res->size, res->time, res->space);
		fputs(data, dataFile);
	} while (!List_isLast(p, resourceList));
	fclose(dataFile);

	// deviation
	deviation.cn2 = sqrt(sumDeviation.cn2) / mean.cn2 * 100;
	deviation.cnlog2n = sqrt(sumDeviation.cnlog2n) / mean.cnlog2n * 100;
	deviation.cnlogn = sqrt(sumDeviation.cnlogn) / mean.cnlogn * 100;
	deviation.cn = sqrt(sumDeviation.cn) / mean.cn * 100;
	deviation.clogn = sqrt(sumDeviation.clogn) / mean.clogn * 100;

	deviation.cn2Space
			= fabs(sqrt(sumDeviation.cn2Space) / mean.cn2Space * 100);
	deviation.cnlognSpace = fabs(
			sqrt(sumDeviation.cnlognSpace) / mean.cnlognSpace * 100);
	deviation.cnSpace = fabs(sqrt(sumDeviation.cnSpace) / mean.cnSpace * 100);
	deviation.clognSpace = fabs(
			sqrt(sumDeviation.clognSpace) / mean.clognSpace * 100);

	// determine which class of complexity
	printCalcRet("Relative standard deviation", deviation);

	//    struct Operation* op = *pointer;
	float dev = 0.0f;
	float con = 0.0f;
	if (constant) {
		op->timeComplexity = N0;
		op->timeConstant = lastTime;
	} else {
		if (deviation.cnlogn < deviation.cn2) {
			dev = deviation.cnlogn;
			con = mean.cnlogn;
			op->timeComplexity = NLOGN;
		} else {
			dev = deviation.cn2;
			con = mean.cn2;
			op->timeComplexity = N2;
		}
		if (deviation.cn < dev) {
			dev = deviation.cn;
			con = mean.cn;
			op->timeComplexity = N;
		}
		if (deviation.clogn < dev){
			dev = deviation.clogn;
			con = mean.clogn;
			op->timeComplexity = LOGN;
		}
		op->timeDeviation = dev;
		op->timeConstant = con;
	}

	if (constantSpace) {
		op->spaceComplexity = N0;
		op->spaceConstant = lastSpace;
	} else {
		if (deviation.cnlognSpace < deviation.cn2Space) {
			dev = deviation.cnlognSpace;
			con = mean.cnlognSpace;
			op->spaceComplexity = NLOGN;
		} else {
			dev = deviation.cn2Space;
			con = mean.cn2Space;
			op->spaceComplexity = N2;
		}
		if (deviation.cnSpace < dev) {
			dev = deviation.cnSpace;
			con = mean.cnSpace;
			op->spaceComplexity = N;
		}
		if (deviation.clognSpace < dev) {
			dev = deviation.clognSpace;
			con = mean.clognSpace;
			op->spaceComplexity = LOGN;
		}

		op->spaceDeviation = dev;
		op->spaceConstant = con;
	}
	printf(
			"Analytical result: class = %d, deviation = %.2f%, constant = %.2f space class =  %d, space dev = %.2f%, space constant = %.2f\n",
			op->timeComplexity, op->timeDeviation, op->timeConstant,
			op->spaceComplexity, op->spaceDeviation, op->spaceConstant);
	//pointer++;
	operationCount++;
	logAllowed = true;
}

struct CalcResult* Resource_analyse(unsigned int size, unsigned long time,
		long space) {
	//    printf("\ttotal time used: %ld against input size: %d\n", time, size);
	struct CalcResult* ret = malloc(sizeof(struct CalcResult));

	// ---------------time-----------------
	float power = 0.0F;
	power = log((float) time) / log((float) size);
	//    printf("\trunning time = size^%f\n",power);
	ret->power = power;

	long n2 = size * size;
	long nlog2n = size * log2((float) size) * log2((float) size);
	long nlogn = size * log2((float) size);
	long logn = log2((float) size);
	float c = (float) time / (float) n2;
	//    printf("\trunning time = %f * n^2\n",c);
	ret->cn2 = c;

	c = (float) time / (float) nlog2n;
	ret->cnlog2n = c;

	c = (float) time / (float) nlogn;
	//    printf("\trunning time = %f * n * log(n)\n",c);
	ret->cnlogn = c;

	c = (float) time / (float) size;
	ret->cn = c;

	c = (float) time / (float) logn;
	//    printf("\trunning time = %f * n * log(n)\n",c);
	ret->clogn = c;
	// -------------space -----------------
	//    power = log((float)space) / log((float)size);
	//    printf("\tspace = size^%f\n", power);

	c = (float) space / (float) n2;
	//    printf("\tspace = %f * n^2\n",c);
	ret->cn2Space = c;

	c = (float) space / (float) nlogn;
	//    printf("\tspace = %f * n * log(n)\n",c);
	ret->cnlognSpace = c;

	c = (float) space / (float) size;
	ret->cnSpace = c;

	c = (float) space / (float) logn;
	//    printf("\tspace = %f * n * log(n)\n",c);
	ret->clognSpace = c;

	return ret;
}

bool include(char* list[], int listLength, char* op) {
	bool ret = false;
	int i;
	for (i = 0; i < listLength; i++) {
		char* item = list[i];
		int cmp = strcmp(item, op);
		if (cmp == 0)
			return true;
	}
	return ret;
}

void removeUselessData(FILE* fp) {
	fseek(fp, -4, SEEK_END);
	int c = fgetc(fp);
	// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!c= %d\n",c);
	if (c == ',') {
		//            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!c= %d\n",c);
		fseek(fp, -4, SEEK_END);
		fputs("        \n", fp);
	}
}

char** getFunctionInfo(enum ComplexityClass cClass, float constant, float dev) {
	char* ret[4] = { "", "", "" };
	char func[100] = "";
	char funcTitle[100] = "";
	char bigO[100] = "";
	char deviation[100] = "";
	sprintf(deviation, "%.2f%", dev);
	if (cClass == N2) {
		sprintf(func, "%.2f * x * x", constant);
		sprintf(funcTitle, "%.2f * N^2");
		sprintf(bigO, "O(N^2)");
	}
	if (cClass == NLOGN) {
		sprintf(func, "%.2f * x * log(x)/log(2)", constant);
		sprintf(funcTitle, "%.2f * NlogN");
		sprintf(bigO, "O(N * logN)");
	}
	if (cClass == N) {
		sprintf(func, "%.2f * x", constant);
		sprintf(funcTitle, "%.2f * N");
		sprintf(bigO, "O(N)");
	}
	if (cClass == LOGN) {
		sprintf(func, "%.2f * log(x)/log(2)", constant);
		sprintf(funcTitle, "%.2f * logN");
		sprintf(bigO, "O(logN)");
	}
	if (cClass == N0) {
		sprintf(func, "%.2f", constant);
		sprintf(funcTitle, "%.2f");
		sprintf(bigO, "O(1)");
	}

	ret[0] = func;
	ret[1] = funcTitle;
	ret[2] = deviation;
	ret[3] = bigO;
	return ret;
}

void Resource_writePlotScript(const char* scriptName, char* opList[],
		int opListLength, int type, int logScale) {
	printf("writing gnu plot script %s \n", scriptName);
	//    struct Operation** p = &operations[0];
	bool includeTime = type == 0 || type == 2;
	bool includeSpace = type == 1 || type == 2;
	int i, j, k, timeCount, spaceCount;
	struct Operation* oi;
	struct Operation* oj;
	//initilize
	for (i = 0; i < operationCount; i++) {
		oi = operations[i];
		oi->timeEqual = false;
		oi->spaceEqual = false;
		for (k = 0; k < 20; k++) {
			oi->timeEqualList[k] = -1;
			oi->spaceEqualList[k] = -1;
		}
	}
	// check equality
	for (i = 0; i < operationCount; i++) {
		oi = operations[i];

		if (!include(opList, opListLength, oi->name) || (oi->timeEqual
				&& oi->spaceEqual))
			continue;
		timeCount = 0;
		spaceCount = 0;

		for (j = i + 1; j < operationCount; j++) {
			oj = operations[j];
			if (!include(opList, opListLength, oj->name))
				continue;
			if (oi->timeComplexity == oj->timeComplexity && oi->timeConstant
					== oj->timeConstant) {
				oi->timeEqualList[timeCount] = j;
				oj->timeEqual = true;
				timeCount++;
			}
			if (oi->spaceComplexity == oj->spaceComplexity && oi->spaceConstant
					== oj->spaceConstant) {
				oi->spaceEqualList[spaceCount] = j;
				oj->spaceEqual = true;
				spaceCount++;
			}
		}
	}

	// file paths
	char filePath[200] = "";
	sprintf(filePath, "./plot/%s/%s-time", oi->path, scriptName);
	char spaceFilePath[200] = "";
	sprintf(spaceFilePath, "./plot/%s/%s-space", oi->path, scriptName);
	if(logScale == 1){
        strcat(filePath,"-logscaley");
        strcat(spaceFilePath, "-logscaley");
	}
	strcat(filePath, ".plt");
	strcat(spaceFilePath, ".plt");

	// open files
	FILE *dataFile;
	FILE *spaceDataFile;
	if (includeTime) {
		dataFile = fopen(filePath, "w+");
		if(logScale == 1){
		    fputs("set logscale y\n",dataFile);
            fputs("set ylabel \"Time complexity (number of execution steps) logscale\"\n",
				dataFile);
		}
		else
            fputs("set ylabel \"Time complexity (number of execution steps)\"\n",
				dataFile);
		fputs("plot ", dataFile);
	}
	if (includeSpace) {
		spaceDataFile = fopen(spaceFilePath, "w+");
		if(logScale == 1){
		    fputs("set logscale y\n", spaceDataFile);
		    fputs("set ylabel \"Space complexity (bytes) logscale\"\n", spaceDataFile);
		}else
            fputs("set ylabel \"Space complexity (bytes)\"\n", spaceDataFile);
		fputs("plot ", spaceDataFile);
	}

	// write plot scripts
	char spaceData[200] = "";
	struct Operation* op;

	for (i = 0; i < operationCount; i++) {
		op = operations[i];
		if (!include(opList, opListLength, op->name)) //not included
			continue;

		// time
		if (includeTime && !op->timeEqual) {
			char data[200] = "";
			sprintf(data, "\"%s.dat\" using 1:2 title \"%s ", op->name,
					op->name);
			fputs(data, dataFile);

			// equal list
			int* p = &op->timeEqualList[0];
			int equalCount = 0;
			while (*p != -1) {
				if (equalCount > 1) {
					fputs("... ", dataFile);
					break;
				}
				struct Operation* another = operations[*p];
				fputs(another->name, dataFile);
				fputs(" ", dataFile);
				p++;
				equalCount++;
			}
			// function title
			char titleData[100] = "";
			char** funcArr = getFunctionInfo(op->timeComplexity,
					op->timeConstant, op->timeDeviation);
			char* title = *(funcArr + 1);
			strcat(titleData, "T(N) => ");
			strcat(titleData, title);
			fputs(titleData, dataFile);

			// function
			char funcData[100] = "";
			strcat(funcData, "\", \\\n");
			strcat(funcData, *funcArr);
			strcat(funcData, " notitle, \\\n");
			fputs(funcData, dataFile);
		}
		// space
		if (includeSpace && !op->spaceEqual) {
			char data[200] = "";
			sprintf(data, "\"%s.dat\" using 1:3 title \"%s ", op->name,
					op->name);
			fputs(data, spaceDataFile);

			// equal list
			int* p = &op->spaceEqualList[0];
			int equalCount = 0;
			while (*p != -1) {
				if (equalCount > 1) {
					fputs("... ", spaceDataFile);
					break;
				}

				struct Operation* another = operations[*p];
				fputs(another->name, spaceDataFile);
				fputs(" ", spaceDataFile);
				p++;
				equalCount++;
			}
			// function title
			char titleData[100] = "";
			char** funcArr = getFunctionInfo(op->spaceComplexity,
					op->spaceConstant, op->spaceDeviation);
			char* title = *(funcArr + 1);
			strcat(titleData, "S(N) => ");
			strcat(titleData, title);
			fputs(titleData, spaceDataFile);

			// function
			char funcData[100] = "";
			strcat(funcData, "\", \\\n");
			strcat(funcData, *funcArr);
			strcat(funcData, " notitle, \\\n");
			fputs(funcData, spaceDataFile);
		}
	}
	// remove ", \" on last line
	if (includeTime) {
		removeUselessData(dataFile);
		fclose(dataFile);
	}
	if (includeSpace) {
		removeUselessData(spaceDataFile);
		fclose(spaceDataFile);
	}
}

void Resource_writeTableData(const char* tableName) {
	int i;
	bool allFileExist = false;
	char data[20000] = "";
	char filePath[200] = "";
	const char* ALL_FILE_PATH = "./table/AllInOne.txt";
	sprintf(filePath, "./table/%s.txt", tableName);
	FILE *dataFile = fopen(filePath, "w+");
    FILE *allInOneFile;
	if ((allInOneFile = fopen (ALL_FILE_PATH, "r")) == NULL )
        printf ( "file non-existant!\n" );
    else{
        allFileExist = true;
        fclose(allInOneFile);
    }
	allInOneFile = fopen(ALL_FILE_PATH, "a+");
	const char* TABLE_HEADER = "Operation\tEmpirical time\tTime RSD\tAnalytical time\tEmpirical space\tSpace RSD\tAnalytical space\n";
	if(!allFileExist)
        fputs(TABLE_HEADER, allInOneFile);

    fputs(TABLE_HEADER,dataFile);
	for (i = 0; i < operationCount; i++) {
		struct Operation* op = operations[i];
		// time
		char** funcArr = getFunctionInfo(op->timeComplexity, op->timeConstant,
				op->timeDeviation);

		strcat(data, op->name);
		strcat(data, "\tTime(N) = ");
		strcat(data, *(funcArr + 1));
		//time rsd
		strcat(data, "\t");
		if (op->timeComplexity != N0) {
			//strcat(data, " ( ");
			strcat(data, *(funcArr + 2));
			strcat(data, "%");
		}
		strcat(data, "\t ");
		strcat(data, *(funcArr + 3));
	//	strcat(data, "\tEQUAL");
		// space
		strcat(data, " \tSpace(N) = ");
		char** funcSpaceArr = getFunctionInfo(op->spaceComplexity,
				op->spaceConstant, op->spaceDeviation);
		strcat(data, *(funcSpaceArr + 1));
        //space rsd
   		strcat(data, "\t");
		if (op->spaceComplexity != N0) {
//			strcat(data, " ( ");
			strcat(data, *(funcSpaceArr + 2));
			strcat(data, "%");
		}
		strcat(data, "\t ");
		strcat(data, *(funcSpaceArr + 3));
	//	strcat(data, "\tEQUAL");

		strcat(data, "\n");
	}
	printf("-----------------------------------------------\n%s\n", data);
	fputs(data, dataFile);
	fclose(dataFile);
	// all in one file
	fputs(tableName,allInOneFile);
    fputs("\n",allInOneFile);
	fputs(data, allInOneFile);
    fclose(allInOneFile);
}
