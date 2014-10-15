#include <stdio.h>
#include <stdlib.h>
#include "sedona.h"
#include "rmc_driver.h"

#define REDIS_CMD_DEBUG 0

Cell RMC_Action_connect(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	Cell ret;
	int r;
	r = rmc_connect(ip);
	ret.ival = r;
	return ret;
}

Cell RMC_Action_aiRead(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	void *p = params[2].aval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}

#if REDIS_CMD_DEBUG
	fprintf(stderr, "RMC_Action_aiRead: start key: %s\n", keyStr);
#endif

	char str[128];
	sprintf(str, "HMGET %s modbusaddress offset presentvalue", keyStr);
	int rs = rmc_read(ip, str, p, 1);

#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_aiRead end\n");
#endif

	ret.ival = rs;
	return ret;
}

Cell RMC_Action_aoRead(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	void *p = params[2].aval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}

#if REDIS_CMD_DEBUG	
	fprintf(stderr, "key: %d, keyStr: %s\n", key, keyStr);	
	fprintf(stderr, "RMC_Action_aoRead: start key: %s\n", keyStr);
#endif	
	char str[128];
	sprintf(str, "HMGET %s modbusaddress priority presentvalue", keyStr);
	int rs = rmc_read(ip, str, p, 5);
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_aoRead end\n");
#endif
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_aoWrite(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	float in1 = params[2].fval;

	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d\0", key);
	} else {
		sprintf(keyStr, "%d\0", key);
	}

	char priorityStr[128];
	int i = 0;
	for (i = 2; i < 18; i++) {
		if (params[i].fval <= -9998.0) {
			if (i == 2) {
				sprintf(priorityStr, "NULL");
			} else {
				sprintf(priorityStr, "%s,NULL", priorityStr);				
			}
		} else {
			if (i == 2) {
				sprintf(priorityStr, "%f", params[i].fval);
			} else {
				sprintf(priorityStr, "%s,%f", priorityStr, params[i].fval);				
			}
		}
	}
	sprintf(priorityStr, "%s\0", priorityStr);
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_aoWrite: start key: %s\n", keyStr);
#endif	
	char str[150];
	sprintf(str, "HMSET %s priority %s", keyStr, priorityStr);

	int rs = rmc_write(ip, str);
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_aoWrite end\n");
#endif	
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_biRead(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	void *p = params[2].aval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_biRead: start key: %s\n", keyStr);
#endif	
	char str[128];
	sprintf(str, "HMGET %s modbusaddress presentvalue", keyStr);
	int rs = rmc_read(ip, str, p, 2);
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_biRead end\n");
#endif
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_boRead(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	void *p = params[2].aval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}
#if REDIS_CMD_DEBUG
	fprintf(stderr, "RMC_Action_boRead: start key: %s\n", keyStr);
#endif
	char str[128];
	sprintf(str, "HMGET %s modbusaddress priority presentvalue", keyStr);
	int rs = rmc_read(ip, str, p, 6);
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_boRead end\n");
#endif	
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_boWrite(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;

	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d\0", key);
	} else {
		sprintf(keyStr, "%d\0", key);
	}
	char priorityStr[128];
	int i = 0;
	for (i = 2; i < 18; i++) {
		if (params[i].ival > 1) {
			if (i == 2) {
				sprintf(priorityStr, "NULL");
			} else {
				sprintf(priorityStr, "%s,NULL", priorityStr);				
			}
		} else {
			if (i == 2) {
				sprintf(priorityStr, "%d", params[i].ival);
			} else {
				sprintf(priorityStr, "%s,%d", priorityStr, params[i].ival);				
			}
		}
	}
	sprintf(priorityStr, "%s\0", priorityStr);

#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_boWrite: start key: %s\n", keyStr);
#endif	
	char str[150];
	sprintf(str, "HMSET %s priority %s", keyStr, priorityStr);
	int rs = rmc_write(ip, str);
#if REDIS_CMD_DEBUG	
	fprintf(stderr, "RMC_Action_boWrite end\n");	
#endif	
	ret.ival = rs;
	return ret;	
}

Cell RMC_Action_avRead(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	void *p = params[2].aval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_avRead: start key: %s\n", keyStr);
#endif	
	char str[128];
	sprintf(str, "HMGET %s modbusaddress, priority", keyStr);
	int rs = rmc_read(ip, str, p, 3);
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_avRead end\n");
#endif	

	ret.ival = rs;
	return ret;
}

Cell RMC_Action_avWritePresentValue(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	float in1 = params[2].fval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d\0", key);
	} else {
		sprintf(keyStr, "%d\0", key);
	}
	char str[128];
	sprintf(str, "HMSET %s presentvalue %f", keyStr, in1);	
	int rs = rmc_write(ip, str);
	ret.ival = rs;
	return ret;
}


Cell RMC_Action_avWrite(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;

	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d\0", key);
	} else {
		sprintf(keyStr, "%d\0", key);
	}

	char priorityStr[128];
	int i = 0;
	for (i = 2; i < 18; i++) {
		if (params[i].fval <= -9998.0) {
			if (i == 2) {
				sprintf(priorityStr, "NULL");
			} else {
				sprintf(priorityStr, "%s,NULL", priorityStr);				
			}
		} else {
			if (i == 2) {
				sprintf(priorityStr, "%f", params[i].fval);
			} else {
				sprintf(priorityStr, "%s,%f", priorityStr, params[i].fval);				
			}
		}
	}
	sprintf(priorityStr, "%s\0", priorityStr);

#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_avWrite: start key: %s\n", keyStr);
#endif	
	char str[150];
	sprintf(str, "HMSET %s priority %s", keyStr, priorityStr);	
	int rs = rmc_write(ip, str);
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_avWrite end\n");
#endif	
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_bvRead(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	void *p = params[2].aval;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_bvRead: start key: %s\n", keyStr);
#endif	
	char str[128];
	sprintf(str, "HMGET %s modbusaddress priority", keyStr);
	int rs = rmc_read(ip, str, p, 4);
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_bvRead end\n");
#endif
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_bvWritePresentValue(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	int in1 = params[2].ival;
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d\0", key);
	} else {
		sprintf(keyStr, "%d\0", key);
	}

	char str[128];
	sprintf(str, "HMSET %s presentvalue %d", keyStr, in1);
	int rs = rmc_write(ip, str);
	ret.ival = rs;
	return ret;	
}

Cell RMC_Action_bvWrite(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;

	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d\0", key);
	} else {
		sprintf(keyStr, "%d\0", key);
	}

	char priorityStr[128];
	int i = 0;
	for (i = 2; i < 18; i++) {
		if (params[i].ival > 1) {
			if (i == 2) {
				sprintf(priorityStr, "NULL");
			} else {
				sprintf(priorityStr, "%s,NULL", priorityStr);				
			}
		} else {
			if (i == 2) {
				sprintf(priorityStr, "%d", params[i].ival);
			} else {
				sprintf(priorityStr, "%s,%d", priorityStr, params[i].ival);				
			}
		}
	}
	sprintf(priorityStr, "%s\0", priorityStr);

#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_bvWrite: start key: %s\n", keyStr);
#endif	
	char str[150];
	sprintf(str, "HMSET %s priority %s", keyStr, priorityStr);
	int rs = rmc_write(ip, str);
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_bvWrite end\n");
#endif	
	ret.ival = rs;
	return ret;
}

Cell RMC_Action_aiWrite(SedonaVM *vm, Cell *params) {
	char *ip = params[0].aval;
	int key = params[1].ival;
	float offset = params[2].fval;	
	char keyStr[8];
	Cell ret;
	if (key<1000000) {
		sprintf(keyStr, "0%d", key);
	} else {
		sprintf(keyStr, "%d", key);
	}
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_aiWrite: start key: %s\n", keyStr);
#endif	
	char str[128];
	sprintf(str, "HMSET %s offset %f", keyStr, offset);
	int rs = rmc_write(ip, str);
#if REDIS_CMD_DEBUG		
	fprintf(stderr, "RMC_Action_aiWrite end\n");
#endif	
	ret.ival = rs;
	return ret;
}