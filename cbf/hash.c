#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//******************************* ELF*******************************//
unsigned int ELFHash(char *str)
{                                               
	int hash = 0;   
	long x =0;     
	while (*str){
		hash = (hash << 4) + *str++;   
		if((x = (hash & 0xF0000000L)) != 0){   
			hash ^= (x >> 24);   
			hash &= ~x;   
		}   
	}   
	int result = (hash & 0x7FFFFFFF) ;   
	return result;   
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
	time_t start = 0, end = 0;	
	unsigned int hcode = 0;
	char wfile[32] = {0};

	if(argc == 2)
		hashi = atoi(argv[1]);

	fd = fopen("/usr/share/dict/words","r");
	if(fd == NULL)
		printf("open file for read error\n");


	snprintf(wfile, sizeof(wfile), "%s_random.txt",h_name[hashi]);
	wfd = fopen(wfile,"w+");
	if(wfd == NULL)
		printf("open file for write error\n");

	start = time(NULL);
	while(fgets(buf,sizeof(buf),fd))
	{
		hcode = (hash[hashi])(buf);
		snprintf(buf, sizeof(buf), "%d\r\n", hcode);
		fwrite(buf, strlen(buf),1,wfd);
	}
	end = time(NULL);

	rewind(fd);
	while(fgets(buf,sizeof(buf),fd))
	{
		
	}

	fclose(fd);
	fclose(wfd);
	fd = wfd = NULL;


	printf("%s cose %ld secondes\n", h_name[hashi], end - start);

	return 0;
}
#endif
