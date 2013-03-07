#define _BEEP "T240O8MSC"
#define _ALARM "T240O8MSCCC"
#define _AMERICANIDIOT "T176L8MSG#G#G#>C#4>C#>C#F#4F#>C#.G#.F#."
#define _CRAZYFROG "T240MSFG#FFA#FD#F>CFF>C#>CG#F>C>FFD#D#GF"

int i;

void play(char* s) {	
	int count = 0;	
	for(i=0;s[i]!='\0';i++){
	count++;
	}	
	
	WriteByte((char) 0xB3);
	WriteByte((char) count);	
	Write(s, count);
}

