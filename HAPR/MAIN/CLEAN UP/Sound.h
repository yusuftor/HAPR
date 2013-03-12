//Method: Play(char* s)
//Author: Tom Collier
//Use: This plays a sound based on the given input string.
//     There are some predefined sounds in Definitions.h
void Play(char* s) {	
	int i = 0, count = 0;	
	for(i=0;s[i]!='\0';i++) {
		count++;
	}	
	
	WriteByte((char) 0xB3);
	WriteByte((char) count);	
	Write(s, count);
}