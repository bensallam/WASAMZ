#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define uint16 unsigned short

#define MAX_KEYWORD_NUM 2

#define SERVER_PATH 0
#define PORT_NUMBER 1

char * keywords[2]={"server_path","port_number"};

struct confData_struct {

	uint16 id;
	char * value;
	struct confData_struct * next;
};

#define confData struct confData_struct

confData * main_data_struct = NULL;

#endif
