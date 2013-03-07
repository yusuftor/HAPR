//Definition of method inputs and returns
void Play(char* s) {	
	int i = 0, count = 0;	
	for(i=0;s[i]!='\0';i++) {
		count++;
	}	
	
	WriteByte((char) 0xB3);
	WriteByte((char) count);	
	Write(s, count);
}