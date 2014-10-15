#include "rmc_driver.h"

#define REDIS_CMD_DEBUG 0

static Conn conns[10] = {0};

int rmc_connect(char *ip) {
	redisContext *ctx = NULL;
	int i = 0;
	int j = 0;
	for (j = 0; j < 10; j++) {
		if (strncmp(conns[j].ip, ip, 16) == 0 && conns[j].connected == 1) {
			ctx = conns[j].ctx;
			break;
		} else if (strncmp(conns[j].ip, ip, 16) != 0 && conns[j].connected == 0) {
			i = j;
			break;
		}
	}

	if (ctx == NULL) {
		struct timeval timeout = {1, 500000};
		ctx = redisConnectWithTimeout(ip, 6379, timeout);
		if (ctx->err) {
			fprintf(stderr, "connection error: %s\n", ctx->errstr);
			redisFree(ctx);
			ctx = NULL;
			return 0;
		}
		int len = strlen(ip);
		memcpy(&conns[i].ip[0], ip, len);	
		conns[i].ctx = ctx;
		conns[i].connected = 1;
	}
	return 1;
}

int rmc_read(char *ip, char *cmd, void *p, int type) {
	redisContext *currentCtx = NULL;
	int i = 0;
	int j = 0;
	for (j = 0; j < 10; j++) {
		if (strncmp(conns[j].ip, ip, 16) == 0 && conns[j].connected == 1) {
			currentCtx = conns[j].ctx;
			i = j;
			break;
		} 	
	}	
	if (currentCtx == NULL) {
		rmc_connect(ip);
		return 0;
	}
#if REDIS_CMD_DEBUG
	if (type == 1)
		fprintf(stderr, "rmc_ai_read cmd: %s\n", cmd);
	else if (type == 2)
		fprintf(stderr, "rmc_bi_read cmd: %s\n", cmd);
	else if (type == 3)		
		fprintf(stderr, "rmc_av_read cmd: %s\n", cmd);
	else if (type == 4)
		fprintf(stderr, "rmc_bv_read cmd: %s\n", cmd);
	else if (type == 5)
		fprintf(stderr, "rmc_ao_read cmd: %s\n", cmd);
	else if (type == 6)
		fprintf(stderr, "rmc_bo_read cmd: %s\n", cmd);
#endif

	redisReply *reply = redisCommand(currentCtx, cmd);

	if (currentCtx->err) {
		printf("REDIS ERROR: %s\n", currentCtx->errstr);
		redisFree(currentCtx);
		currentCtx= NULL;
		conns[i].connected = 0;
		rmc_connect(ip);
		return 0;
	}

	if (reply == NULL || 
		reply->type == REDIS_REPLY_ERROR ||
		reply->type == REDIS_REPLY_NIL) {
		freeReplyObject(reply);
		return 0;
	}

// Read AI point
	if (type == 1) {
		AIPoint *p1 = (AIPoint *)p;

		if (reply->type == REDIS_REPLY_ARRAY) {
			if ( ((redisReply *)(reply->element[1]))->type != REDIS_REPLY_NIL ) {
				p1->offset = (float)(atof(reply->element[1]->str));
			}
			if ( ((redisReply *)(reply->element[2]))->type != REDIS_REPLY_NIL ) {
				p1->presentvalue = (float)(atof(reply->element[2]->str));			
			}
		}
	}
// read BI point 
	else if (type == 2){
		BIPoint *p1 = (BIPoint *)p;

		if (reply->type == REDIS_REPLY_ARRAY) {
			if ( ((redisReply *)(reply->element[1]))->type != REDIS_REPLY_NIL ) {
				p1->presentvalue = atoi(reply->element[1]->str);			
			}
		}		
	}
// read av point 
	else if (type == 3) {
		if (reply->type == REDIS_REPLY_ARRAY) {
			float maxPriorty = -9999.0;
			if (((redisReply *)(reply->element[1]))->type != REDIS_REPLY_NIL ) {
				AVPoint *p1 = (AVPoint *)p;				
				char *tok = NULL;
				tok = strtok(reply->element[1]->str, ",");
				int index = 0;
				float in = -9999.0;
				while (tok) {
					in = -9999.0;
					if (strncmp(tok, "n", 1) && strncmp(tok, "N", 1)) {
						in = atof(tok);
						if (maxPriorty == -9999.0) {
							maxPriorty = in;
						}
					} 
					tok = strtok(NULL, ",");
					switch(index) {
						case 0:
							p1->in1 = in;
							break;
						case 1:
							p1->in2 = in;
							break;
						case 2:
							p1->in3 = in;
							break;
						case 3:
							p1->in4 = in;
							break;
						case 4:
							p1->in5 = in;
							break;
						case 5:
							p1->in6 = in;
							break;
						case 6:
							p1->in7 = in;
							break;
						case 7:
							p1->in8 = in;
							break;
						case 8:
							p1->in9 = in;
							break;
						case 9:
							p1->in10 = in;
							break;
						case 10:
							p1->in11 = in;
							break;
						case 11:
							p1->in12 = in;
							break;
						case 12:
							p1->in13 = in;
							break;
						case 13:
							p1->in14 = in;
							break;
						case 14:
							p1->in15 = in;
							break;
						case 15:
							p1->in16 = in;
							break;
					}
					index++;
				}
				p1->presentvalue = maxPriorty;
			}		
		}		
	}
// read bv point 
	else if (type == 4) {
		if (reply->type == REDIS_REPLY_ARRAY) {
			int maxPriorty = -1;
			if ( ((redisReply *)(reply->element[1]))->type != REDIS_REPLY_NIL ) {
				BVPoint *p1 = (BVPoint *)p;				
				char *tok = NULL;
				tok = strtok(reply->element[1]->str, ",");
				int index = 0;
				int in = -1;
				while (tok) {
					in = -1;
					if (strncmp(tok, "n", 1) && strncmp(tok, "N", 1)) {
						in = atoi(tok);
						if (maxPriorty == -1) {
							maxPriorty = in;
						}						
					} 
					tok = strtok(NULL, ",");
					switch(index) {
						case 0:
							p1->in1 = in;
							break;
						case 1:
							p1->in2 = in;
							break;
						case 2:
							p1->in3 = in;
							break;
						case 3:
							p1->in4 = in;
							break;
						case 4:
							p1->in5 = in;
							break;
						case 5:
							p1->in6 = in;
							break;
						case 6:
							p1->in7 = in;
							break;
						case 7:
							p1->in8 = in;
							break;
						case 8:
							p1->in9 = in;
							break;
						case 9:
							p1->in10 = in;
							break;
						case 10:
							p1->in11 = in;
							break;
						case 11:
							p1->in12 = in;
							break;
						case 12:
							p1->in13 = in;
							break;
						case 13:
							p1->in14 = in;
							break;
						case 14:
							p1->in15 = in;
							break;
						case 15:
							p1->in16 = in;
							break;
					}
					index++;
				}	
				p1->presentvalue = maxPriorty;										
			}						
		}		
	}
// read ao point
	else if (type == 5) {
		if (reply->type == REDIS_REPLY_ARRAY) {
			AOPoint *p1 = (AOPoint *)p;				
			if ( ((redisReply *)(reply->element[1]))->type != REDIS_REPLY_NIL ) {
				char *tok = NULL;
				tok = strtok(reply->element[1]->str, ",");
				int index = 0;
				float in = -9999.0;
				while (tok) {
					in = -9999.0;
					if (strncmp(tok, "n", 1) && strncmp(tok, "N", 1)) {
						in = atof(tok);
					}
					tok = strtok(NULL, ",");					
					switch(index) {
						case 0:
							p1->in1 = in;
							break;
						case 1:
							p1->in2 = in;
							break;
						case 2:
							p1->in3 = in;
							break;
						case 3:
							p1->in4 = in;
							break;
						case 4:
							p1->in5 = in;
							break;
						case 5:
							p1->in6 = in;
							break;
						case 6:
							p1->in7 = in;
							break;
						case 7:
							p1->in8 = in;
							break;
						case 8:
							p1->in9 = in;
							break;
						case 9:
							p1->in10 = in;
							break;
						case 10:
							p1->in11 = in;
							break;
						case 11:
							p1->in12 = in;
							break;
						case 12:
							p1->in13 = in;
							break;
						case 13:
							p1->in14 = in;
							break;
						case 14:
							p1->in15 = in;
							break;
						case 15:
							p1->in16 = in;
							break;
					}
					index++;
				}
			}			
			if ( ((redisReply *)(reply->element[2]))->type != REDIS_REPLY_NIL ) {
				p1->presentvalue = atof(reply->element[2]->str);			
			}
		}				
	}
// read bo point
	else if (type == 6) {
		if (reply->type == REDIS_REPLY_ARRAY) {
			BOPoint *p1 = (BOPoint *)p;					
			if ( ((redisReply *)(reply->element[1]))->type != REDIS_REPLY_NIL ) {
				char *tok = NULL;
				tok = strtok(reply->element[1]->str, ",");
				int index = 0;
				int in = -1;
				while (tok) {
					in = -1;
					if (strncmp(tok, "n", 1) && strncmp(tok, "N", 1)) {
						in = atoi(tok);
					}
					tok = strtok(NULL, ",");
					switch(index) {
						case 0:
							p1->in1 = in;
							break;
						case 1:
							p1->in2 = in;
							break;
						case 2:
							p1->in3 = in;
							break;
						case 3:
							p1->in4 = in;
							break;
						case 4:
							p1->in5 = in;
							break;
						case 5:
							p1->in6 = in;
							break;
						case 6:
							p1->in7 = in;
							break;
						case 7:
							p1->in8 = in;
							break;
						case 8:
							p1->in9 = in;
							break;
						case 9:
							p1->in10 = in;
							break;
						case 10:
							p1->in11 = in;
							break;
						case 11:
							p1->in12 = in;
							break;
						case 12:
							p1->in13 = in;
							break;
						case 13:
							p1->in14 = in;
							break;
						case 14:
							p1->in15 = in;
							break;
						case 15:
							p1->in16 = in;
							break;
					}
					index++;
				}				
			}						
			if ( ((redisReply *)(reply->element[2]))->type != REDIS_REPLY_NIL ) {
				p1->presentvalue = atoi(reply->element[2]->str);			
			}
		}				
	}
	freeReplyObject(reply);
	return 1;
}


int rmc_write(char *ip, char *cmd) {
	int ipLen = 0;

	redisContext *ctx;

	ipLen = strlen(ip);
	if (ipLen < 9 || ipLen > 15) {
		return 0;
	}

	ctx = redisConnect(ip, 6379);
	if (ctx->err) {
		redisFree(ctx);		
		ctx = NULL;
		return 0;
	}
	#if REDIS_CMD_DEBUG
	fprintf(stderr, "rmc_write cmd: %s\n", cmd);
	#endif

	redisReply *reply = redisCommand(ctx, cmd);
	if (reply == NULL || 
		reply->type == REDIS_REPLY_ERROR ||
		reply->type == REDIS_REPLY_NIL) {
		freeReplyObject(reply);		
		redisFree(ctx);
		ctx = NULL;
		return 0;
	} 

	int r = strncmp(reply->str, "OK", 2);
	if (r == 0) {
		freeReplyObject(reply);
		redisFree(ctx);		
		return 1;
	}

	freeReplyObject(reply);
	redisFree(ctx);
	return 0;
}