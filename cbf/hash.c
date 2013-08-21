#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//******************************* ELF*******************************//
unsigned int ELFHash(char *str)
{                                               
	unsigned int hash = 0;   
	unsigned long x =0;     
	while (*str){
		hash = (hash << 4) + *str++;   
		if((x = (hash & 0xF0000000L)) != 0){   
			hash ^= (x >> 24);   
			hash &= ~x;   
		}   
	}   
	return (hash & 0x7FFFFFFF) ;   
} 
//*************************** JS Hash Function **********************//
unsigned int JsHash( char * str)                           
{ 
	unsigned int hash = 1315423911 ; 

	while ( * str) 
	{ 
		hash ^= ((hash << 5 ) + ( * str ++ ) + (hash >> 2 )); 
	} 

	return (hash & 0x7FFFFFFF ); 
} 

//*************************RS Hash Function*************************// 
unsigned int RSHash( char * str)                     
{ 
	unsigned int b = 378551 ; 
	unsigned int a = 63689 ; 
	unsigned int hash = 0 ; 

	while ( * str) 
	{ 
		hash = hash * a + ( * str ++ ); 
		a *= b; 
	} 

	return (hash & 0x7FFFFFFF ); 
} 
 //****************************PJWHash**********************************//
unsigned int PJWHash( char * str) 
{ 
	unsigned int BitsInUnignedInt = (unsigned int )( sizeof (unsigned int ) *8 ); 
	unsigned int ThreeQuarters = (unsigned int )((BitsInUnignedInt * 3 )  / 4 ); 
	unsigned int OneEighth = (unsigned int )(BitsInUnignedInt / 8 ); 

	unsigned int HighBits = (unsigned int )( 0xFFFFFFFF ) << (BitsInUnignedInt - OneEighth); 
	unsigned int hash = 0 ; 
	unsigned int test = 0 ; 

	while ( * str) 
	{ 
		hash = (hash << OneEighth) + ( * str ++ ); 
		if ((test = hash & HighBits) != 0 ) 
		{ 
			hash = ((hash ^ (test >> ThreeQuarters)) & ( ~ HighBits) 
		); 
		} 
	} 

	return (hash & 0x7FFFFFFF ); 
}
//********************** BKDR Hash Function ***************************//
unsigned int BKDRHash( char * str) 
{ 
	unsigned int seed = 131 ; // 31 131 1313 13131 131313 etc.. 
	unsigned int hash = 0 ; 

	while ( * str) 
	{ 
		hash = hash * seed + ( * str ++ ); 
	} 

	return (hash & 0x7FFFFFFF ); 
}

//************************ SDBM Hash Function ************************//
unsigned int SDBMHash( char * str) 
{ 
	unsigned int hash = 0 ; 

	while ( * str) 
	{ 
		hash = ( * str ++ ) + (hash << 6 ) + (hash << 16 ) - hash; 
	} 

	return (hash & 0x7FFFFFFF ); 
} 

//************************DJB Hash Function **************************//
 
unsigned int DJBHash( char * str) 
{ 
	unsigned int hash = 5381 ; 

	while ( * str) 
	{ 
		hash += (hash << 5 ) + ( * str ++ ); 
	} 

	return (hash & 0x7FFFFFFF ); 
} 

//************************* AP Hash Function *************************//

unsigned int APHash( char * str) 
{ 
	unsigned int hash = 0 ; 
	int i; 

	for (i = 0 ; * str; i ++ ) 
	{ 
		if ((i & 1 ) == 0 ) 
		{ 
			hash ^= ((hash << 7 ) ^ ( * str ++ ) ^ (hash >> 3 )); 
		} 
		else 
		{ 
			hash ^= ( ~ ((hash << 11 ) ^ ( * str ++ ) ^ (hash >> 5 ))); 
		} 
	} 

	return (hash & 0x7FFFFFFF ); 
} 


#if 0
int main(int argc, char **argv)
{
	unsigned int (*hash[])(char *input) = {
		ELFHash, JsHash, RSHash, PJWHash, BKDRHash, SDBMHash, DJBHash, APHash
	};
	char *h_name[] = {
		"ELFHash", "JsHash", "RSHash", "PJWHash", "BKDRHash", "SDBMHash", "DJBHash", "APHash"
	};

	int  hashi = 0;
	FILE *fd, *wfd;
	char buf[32];
	char cbuf[32];
	struct timeval start, end;
	unsigned int hcode = 0;
	char wfile[32] = {0};
	char *p = NULL;

	if(argc >= 2)
		hashi = atoi(argv[1]);

	printf("hashi=%d\n", hashi);
	if(argc >= 3)
	{
		hcode = (hash[hashi])(argv[2]);
		printf("%s hash %u\n", argv[2], hcode);
		return 0;
	}


	fd = fopen("/usr/share/dict/words","r");
	//fd = fopen("random.txt","r");
	if(fd == NULL)
		printf("open file for read error\n");

	snprintf(wfile, sizeof(wfile), "%s_random%d.txt",h_name[hashi], hashi);
	wfd = fopen(wfile,"w+");
	if(wfd == NULL)
		printf("open file for write error\n");

	gettimeofday(&start, NULL);
	while(fgets(buf,sizeof(buf),fd))
	{
		p = buf;
		p += strlen(buf);
		p--;
		while ((*p=='\n' || *p=='\r') && p >= buf) 
			p--;
		*(p+1)='\0';

		hcode = (hash[hashi])(buf);
		snprintf(cbuf, sizeof(cbuf), "%u \t%s\n", hcode, buf);
		fwrite(cbuf, strlen(cbuf),1,wfd);
	}
	gettimeofday(&end, NULL);

	fclose(fd);
	fclose(wfd);
	fd = wfd = NULL;


	printf("%s cose %ld secondes %ld usecondes \n", h_name[hashi], end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

	return 0;
}
#endif
