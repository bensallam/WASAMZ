#include "conf.h"


void checkConf(){
	FILE * confFile;
	char buff[256];
	char tmp[32];
	uint16 line_counter = 0;

	confFile = fopen("serv.conf","r");
	
	if(confFile == 0)
	{
		printf("Configuration file not found.\n\
				Maybe you are using WASAMZ for the first time or you delete this file.\n\
				We will generate the defualt file, and you can edit it later.\n");
		confFile = fopen("serv.conf","w");
		fprintf(confFile,"#This is the main configuration file of the WASAMZ server.\n\
				#Set the server path to your site directory:\n\
				server_path : ./http_doc\n\
				\n\
				#Set your port number for connection:\n\
				port_number : 80\n");
		fclose(confFile);
		confFile = fopen("serv.conf","r");
	}

	while(!feof(confFile))
	{	
		fgets(buff,sizeof(buff),confFile);
		line_counter++;
		if(test_line(buff)==0)
		{
			printf("An Error found if line %d , unable to start system.\n",line_counter);
			exit(1);
		}
	}
	fclose(confFile);

}

int test_line(char * line){
	
	int i = 0;
	int fc,lc;//first shar and the last char of the word
	char tmp[32];

	int idVal;
	confData * head = main_data_struct;
	confData * curr ;

	//skip all spaces
	while(i<strlen(line)){
		if(line[i] == ' ')
			i++;
		else break;
	}
	//deduce the keyword
	fc=i;//set the first char of the keyword
	while(i<strlen(line)){
		if(line[i] == '#')
			return 1;//this symbol of comment no thing to do
		else if(line[i] == ' ')
			break;
		else
			lc=i++;
	}

	if(i == strlen(line))
		return 1;//we found an empty line after skip all spaces
	if(line[i] != ' ')
		return 0;
	//check if true keyword
	memset((char *) tmp, 0, sizeof(tmp));
	strncpy(tmp,line+fc,lc-fc+1);
	idVal=test_keyword(tmp);
	if(idVal <0)
		return 0;
	//check separator
	while(i<strlen(line)){
		
		if(line[i] == ' ')
			i++;
		else if (line[i] == ':' && line[i+1] == ' ')
		{
			i++;
			break;
		}
		else 
			return 0;
	}	
	while(i<strlen(line)){
		if(line[i]==' ')
			i++;
		else 
			break;
	}
	//check value
	fc=i;
	while(i<strlen(line)){

		if(line[i]== ' ' || line[i]=='\r' || line[i]=='\n')
			break;
		else
			i++;
	}
	lc=i-1;
	memset((char *) tmp, 0, sizeof(tmp));
	strncpy(tmp,line+fc,lc-fc+1);
	//get new list
	//check if found any error with data
	//analyze_data(tmp ,idVal)

	if(head == NULL)
	{
		main_data_struct=(confData *) malloc(sizeof(confData));
		head = main_data_struct;
		curr=head;
	}
	else{
		curr=head;
		while(curr->next!=NULL)
		{
			curr=curr->next;
		}

		curr->next=(confData *) malloc(sizeof(confData));
		curr=curr->next;
	}
	curr->next=NULL;
	curr->id=idVal;
	curr->value=(char *)malloc(strlen(tmp)+1);
	strcpy(curr->value,tmp);
}

int test_keyword(char * data){
	
	int i;

	for(i=0; i<MAX_KEYWORD_NUM; i++)
		if(strcmp(data,keywords[i])==0)
			return i;
	return -1;
}
