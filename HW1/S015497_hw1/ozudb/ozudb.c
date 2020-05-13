#include "people.h"
#include <stdio.h>
#include <stdlib.h>


struct person People[5];

int my_getnumber(char *str){
  int result;
  int puiss;
  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+')){
      if (*str == '-')
        puiss = puiss * -1;
      str++;
  }
  while ((*str >= '0') && (*str <= '9')){
      result = (result * 10) + ((*str) - '0');
      str++;
  }
  return (result * puiss);
}


const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}


int main() {
	printf("\n Welcome to ozudb! \n");
	while(1){
		printf("\n Main menu: \n");
		char command;
		scanf(" %c", &command);

		if(command == 'C'){
			printf("\nCreating Table: \n");
			for(int i=0 ; i<5; i = i + 1 ){
				People[i].id = 0;
				strcpy( People[i].name , "" );
				strcpy( People[i].surname , "" );
			}
			printf("\nDone! \n");
		}
		
		else if (command == 'A'){
			int tmp1_id;
			printf("\nEnter Id: \n");
			scanf("%d",&tmp1_id);
			if (People[tmp1_id].id != 0)
				printf("\nCare OVERWRITING!! \n");				
			People[tmp1_id].id = tmp1_id;
			printf("\nEnter Name: \n");
			char temp[20] ;
		    	scanf("%s",temp);
			strcpy(People[tmp1_id].name,temp);
			printf("\nEnter Surname: \n");
			scanf("%s",temp);
			strcpy(People[tmp1_id].surname,temp);
			
		}
		
		else if (command == 'D'){
			printf("\nId to be deleted: \n");
			int temp_id;
			scanf("%d", &temp_id);
			
			if(temp_id < 0 || temp_id > 4){
				printf("\n Not valid id. Enter between 0 and 4. Getting back to menu.\n");
				break;
			}
			
			strcpy(People[temp_id].name , "");
			strcpy(People[temp_id].surname , "");
			People[temp_id].id = 0;
			printf("\n Done! \n");
		}
		
		else if (command == 'P'){
			
			if (strcmp(People[0].name , "empty") == 0)
				printf("\n Student table is empty. \n");
			    
			for(int i=0 ; i<5; i= i+1){
				int people_id = People[i].id;
				char name[20],surname[20];
				strcpy(name,People[i].name);
			    strcpy(surname,People[i].surname);
			    

				printf("\n %d  %s  %s \n", people_id , name , surname);
			}
		}	
		
		else if (command == 'R'){
		
			printf("\n Reading data from People.csv file \n");

			FILE* stream = fopen("People.csv", "r");

			    char line[1024];
				
			if (!stream){
			printf("Can't open file\n");
			}
			    
			    while (fgets(line, 1024, stream))
			    {
				

				char str[100];
				strcpy(str, line);
				int init_size = strlen(str);
				const char delim[] = ",";

				char *ptr;
				ptr = strtok(str, delim);
				int temp_number = 1;

				int j = 0;
				// j attribute counter...
				int id = 0;
				// id student id...

				
				while(ptr != NULL)
				{
					
					//printf("'%s'\n", ptr);

					//People[1].id = 1;
					//strcpy(People[1].name,ptr);
					//ptr = strtok(NULL, delim);
					//strcpy(People[1].surname,ptr);

					if (j == 0) {
					  id = atoi(ptr);
					  People[id].id = id;
					} else if (j == 1) {
					  strcpy(People[id].name, ptr);
					} else if (j == 2) {
					  strcpy(People[id].surname, ptr);
					}

					ptr = strtok(0, ",");
					j++;
				}
				


				//free(ptr);
				//free(str);
				//free(delim);

			    }
			
		
		}else if (command == 'E'){
			printf("\n Exiting.. \n");
			return 0;
		}
	}

}

