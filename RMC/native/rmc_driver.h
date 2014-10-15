#ifndef RMC_DRIVER_H
#define RMC_DRIVER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hiredis.h"

typedef struct AIPoint {
	float offset;
	float presentvalue;	
} AIPoint;

typedef struct BIPoint {
	int presentvalue;
} BIPoint;

typedef struct AVPoint {
	float in1;
	float in2;
	float in3;
	float in4;
	float in5;
	float in6;
	float in7;
	float in8;
	float in9;
	float in10;
	float in11;
	float in12;
	float in13;
	float in14;
	float in15;
	float in16;
	float presentvalue;
} AVPoint;

typedef struct AOPoint {
	float in1;
	float in2;
	float in3;
	float in4;
	float in5;
	float in6;
	float in7;
	float in8;
	float in9;
	float in10;
	float in11;
	float in12;
	float in13;
	float in14;
	float in15;
	float in16;
	float presentvalue;
} AOPoint;

typedef struct BOPoint {
	int in1;
	int in2;
	int in3;
	int in4;
	int in5;
	int in6;
	int in7;
	int in8;
	int in9;
	int in10;
	int in11;
	int in12;
	int in13;
	int in14;
	int in15;
	int in16;
	int presentvalue;
} BOPoint;

typedef struct BVPoint {
	int in1;
	int in2;
	int in3;
	int in4;
	int in5;
	int in6;
	int in7;
	int in8;
	int in9;
	int in10;
	int in11;
	int in12;
	int in13;
	int in14;
	int in15;
	int in16;
	int presentvalue;
} BVPoint;

typedef struct Conn {
	char ip[16];
	int connected;
	redisContext *ctx;	
} Conn;

int rmc_connect(char *ip);
int rmc_read(char *ip, char *cmd, void *p, int type);
int rmc_write(char *ip, char *cmd);
#endif