#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

char* readJSONFILE(){
 FILE *pFile = NULL;
 	char STRING[300] ;
 	char * str = (char*)malloc(sizeof(char)*10);
 	str[0]='\0';
 	int i=0;
  printf("원 하 는  파 일 명 을  입 력: ");
  scanf("%s",STRING);
  getchar();
 		pFile = fopen( STRING, "r" );
 		if( pFile == NULL )
 		{
 			printf("no file exist\n");
 			return;
 		}
 		else
 		{

 			while( fgets( STRING,300,pFile )!=NULL )
 			{
 				str = (char *)realloc(str,sizeof(char)*strlen(STRING)+sizeof(char)*strlen(str));
 				strcat(str,STRING);
 			}

   		fclose( pFile );

		}
		return str;
	}

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex)
{
  int i;
  int j=1;
  int max=0;
  int count=1;
  for ( i = 0; i < tokcount; i++)
  {
    if(t[i+1].size>0&&t[i+1].type==JSMN_STRING){
//      printf("[Name %d]%.*s\n", j,t[i+1].end-t[i+1].start,
  //      jsonstr + t[i+1].start);
    if(t[i+2].end>max)
    {
      printf("[Name %d]%.*s\n", count++,t[i+1].end-t[i+1].start,jsonstr + t[i+1].start);
    }
        nameTokIndex[j++]=i+1;
        max=t[i+2].end;
      }
  }
  nameTokIndex[0] = j-1; //put size of array in 0
}

void printNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex)
{
  int sizeOfArray = nameTokIndex[0];
  int i;
  printf("* * * * * Name List * * * * *\n");
  for ( i = 0; i < sizeOfArray; i++)
  {
        printf("[Name %d]%.*s\n", i+1,t[nameTokIndex[i+1]].end-t[nameTokIndex[i+1]].start,
        jsonstr + t[nameTokIndex[i+1]].start);
  }
}

void selectNameList(char * jsonstr, jsmntok_t * t, int *nameTokIndex)
{
  int i;
  while(1)
  {
      printf("Select name's no (exit:0) >> ");
      scanf("%d",&i);
      getchar();
      if(i==0) break;

      printf("[Name %d]%.*s\n%.*s\n", i,t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,
      jsonstr + t[nameTokIndex[i]].start,t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,
      jsonstr + t[nameTokIndex[i]+1].start);
  }
}

void showObjectList(char * jsonstr, jsmntok_t *t,int tokcount, int * toklengthObject)
{
  int max=0;
  int i=0;
  int j=1;
  int size=0;
  toklengthObject[0]=0;
    printf("* * * * * Object List * * * * *\n");

  for (i = 0; i < tokcount-1; i++) {
      // We may use strndup() to fetch string value
      if(i==0 && t[1].type==JSMN_STRING && t[2].type==JSMN_ARRAY && t[3].type==JSMN_OBJECT)
      {
          i=3;
      }
      if(t[i].end>max&& t[i].type==JSMN_OBJECT) {
        printf("[NAME: %d] %.*s\n ",j ,t[i+2].end-t[i+2].start,
              jsonstr + t[i+2].start);
              toklengthObject[j++] =t[i].end;
              if(j==2&&i!=0)
                toklengthObject[0]= t[i].start;
              max = t[i].end;

        }

 }

/*
 printf("[NAME: %d] %.*s\n ",0 ,t[0].end-t[0].start,
       jsonstr + t[0].start);
*/
  }

char * printSpeicifiedObject(char * jsonstr, jsmntok_t *t, int * nameTokIndex, int * toklengthObject)
{
  int jsonObject;
  int i;
  char STRING[300] ;
  char * str = (char*)malloc(sizeof(char)*10);
  str[0]='\0';

  printf("원하는 번호 입력 (Exit:0): ");
  scanf("%d",&jsonObject);

  if(jsonObject==0)
  {
    return;
  }
  else{

    for ( i = 1; i < nameTokIndex[0]; i++)
    {

      if(t[nameTokIndex[i]].end> toklengthObject[jsonObject-1] && t[nameTokIndex[i]].end<toklengthObject[jsonObject] ){
        sprintf(STRING,"[%.*s] %.*s\n", t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start,
        t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start, jsonstr + t[nameTokIndex[i]+1].start);
        /*
        printf("[%.*s] %.*s\n", t[nameIndex[i]].end-t[nameIndex[i]].start,
        JSON_STRING + t[nameIndex[i]].start,t[nameIndex[i]+1].end-t[nameIndex[i]+1].start,
        JSON_STRING + t[nameIndex[i]+1].start);
        */
        str = (char *)realloc(str,sizeof(char)*strlen(STRING)+sizeof(char)*strlen(str));
        strcat(str,STRING);
      }
    }
    return str;
  }


}



char * JSON_STRING;
/*
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";
*/


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
  int nameIndex[100];
  int nameLengthEachObject[30];
   /*maximum length of json object inside json object.
  number of object is in array 0*/
  char * jsonObject;

	JSON_STRING = readJSONFILE();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || (t[0].type != JSMN_OBJECT&&t[0].type != JSMN_ARRAY)) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
  /*
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
			// We may use strndup() to fetch string value
			printf("- Name: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {
		    //We may additionally check if the value is either "true" or "false"
			printf("- Keywords %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {
			// We may want to do strtol() here to get numeric value
			printf("- Uid: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "examples") == 0) {
			int j;
			printf("- Examples:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; // We expect groups to be an array of strings
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		}
    //else {
		//	printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
		//			JSON_STRING + t[i].start);
		//}


	}
  */



 jsonNameList(JSON_STRING, t, r,nameIndex);
 showObjectList(JSON_STRING,t,r,nameLengthEachObject);

 //printNameList(JSON_STRING,t,nameIndex);
   printf("\n====================\n");
   //selectNameList(JSON_STRING,t,nameIndex);

 jsonObject=printSpeicifiedObject(JSON_STRING, t, nameIndex,nameLengthEachObject);
 printf("%s",jsonObject);




	return EXIT_SUCCESS;
}
