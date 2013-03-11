#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct loglist {
	char* logtext;
	struct loglist* next;
};
typedef struct loglist log;

char timestampstr[8] = "@ TIME: ";
log* head;
log* last;

void addlog(char* logstring);
void printlogs();	

int main()
{
	log * temp = (log *)malloc(sizeof(log));
	temp->logtext = "@ 00:00 PROGRAM STARTED\n\r";
	temp->next = NULL;
	last = temp;
	head = temp;

	addlog("NEXT LOG");
	addlog("the third log");

	printlogs();

}

void addlog(char* logstring)
{
	
	log *  newlog = (log *)malloc(sizeof(log));
	
	char *tempstring = malloc(strlen(timestampstr) + strlen(logstring) + 3); // additional bytes for \n\r\0
	strcpy(tempstring, timestampstr);
	strcat(tempstring, logstring);
	strcat(tempstring, "\n\r");
	
	newlog->logtext = tempstring;
	newlog->next = NULL;
					
	last->next = newlog;

	last = newlog;

}

void printlogs()
{
	last = head;
	while(last->next)
		{
			printf("%s",last->logtext);
			last = last->next;
		}
	printf("%s", last->logtext);
}

void turnangle(float angle)
{

	if(angle > 3.14f)
		{
			angle = 3.1f;
		}else if(angle < -3.28f)
		{
			angle = 3.28f;
		}

	float finalTHETA = currentTHETA + angle;
	float angle10 = angle * 0.1f;
	float prelimTHETA = finalTHETA - angle10;

	if(angle > 0)
	{
		spin();
		while(currentTHETA < prelimTHETA);
		spin();
		while(currentTHETA < finalTHETA);

	}else if(angle < 0){

		spin();
		while(currentTHETA > prelimTHETA);
		spin();
		while(currentTHETA > finalTHETA);
	
	}
}
