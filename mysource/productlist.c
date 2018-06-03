#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../jsmn.h"
#include "./productlist.h"
int countRamenObject;
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

 		pFile = fopen("./data4.json", "r" );
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

int jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo** nameTokenInfo)
{
    int i;
    int j=1;
    int countObject=0;
    int count=0;

    for ( i = 1; i <= tokcount; i++)
    {
        if(t[i].size>0&&t[i].type==JSMN_OBJECT)
        {
        countObject++;
        j=  i;
          while(t[j].end>t[++i].start)
          {
            if(t[i].size>0&&t[i].type==JSMN_STRING)
            {
                  (*nameTokenInfo)[count].tokindex = i;
                  (*nameTokenInfo)[count++].objectindex= countObject;
                  (*nameTokenInfo) = (NameTokenInfo*)realloc(*nameTokenInfo,sizeof(NameTokenInfo)*(count+2));

            }
          }
          i--;
        }
    }
    return count;
}

void printSavedInfo(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo * nameTokenInfo, int count)
{
  int i=0;
  int j=0;
  char nameOfJson[50];
  int objectNumber;
  for(i=0;i<count;i++)
  {
      sprintf(nameOfJson,"%.*s", t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start);
      printf("Object number: %d , name: %s\n",nameTokenInfo[i].objectindex, nameOfJson);
  }

}

void getTokenIndex(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo * nameTokenInfo, int count)
{
  int objectNumber;
  char name[50];
  char nameOfJson[50];
  int i=0;
  int j=0;

  printf("object number를 입력하라: ");
  scanf("%d",&objectNumber);
  getchar();
  printf("name을 입력하라: ");
  scanf("%s",name);
  getchar();

  for(i=0;i<count;i++)
  {
    sprintf(nameOfJson,"%.*s", t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start);
    if(nameTokenInfo[i].objectindex==objectNumber&&strcmp(nameOfJson,name)==0)
    {
      printf("token의 index는 %d\n",nameTokenInfo[i].tokindex);
    }
  }

}


void printTable(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo * nameTokenInfo, int count)
{

  int price;
  int countProduct;
  char STRING[30];
  char nameOfJson[50];
  int current=1;
  int i=0;
  int j=0;
  int priceIndex;
  int nameIndex;
  int companyIndex;
  int countIndex;
  int charLen;
  for(i=0;i<count;i++)
  {
    if(nameTokenInfo[i].objectindex!=current || i==count-1)
    {
      printf("%-8d",current);
      printf("%-15.*s ", t[ nameIndex].end-t[nameIndex].start,jsonstr + t[nameIndex].start);
      charLen=t[ nameIndex].end-t[nameIndex].start;
      charLen/=3;
      for(j=0;j<charLen;j++) printf(" ");

      printf("%-17.*s", t[companyIndex].end-t[companyIndex].start,jsonstr + t[companyIndex].start);
      charLen= t[companyIndex].end-t[companyIndex].start;
      charLen/=3;
      for(j=0;j<charLen;j++) printf(" ");

      printf("%-8.*s", t[priceIndex].end-t[priceIndex].start,jsonstr + t[priceIndex].start);
      sprintf(STRING,"%.*s",t[priceIndex].end-t[priceIndex].start,jsonstr + t[priceIndex].start);
      price = atoi(STRING);

      printf("%-6.*s", t[countIndex].end-t[countIndex].start,jsonstr + t[countIndex].start);
      sprintf(STRING,"%.*s",t[countIndex].end-t[countIndex].start,jsonstr + t[countIndex].start);
      countProduct = atoi(STRING);


      printf("%5d",price*countProduct);
      printf("\n");
      current+=1;
    }

    sprintf(nameOfJson,"%.*s", t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start);
    if(strcmp(nameOfJson,"count")==0)
    {
      countIndex=nameTokenInfo[i].tokindex+1;
    }
    if(strcmp(nameOfJson,"price")==0)
    {
      priceIndex=nameTokenInfo[i].tokindex+1;
    }
    if(strcmp(nameOfJson,"name")==0)
    {
      nameIndex=nameTokenInfo[i].tokindex+1;
    }
    if(strcmp(nameOfJson,"company")==0)
    {
      companyIndex=nameTokenInfo[i].tokindex+1;
    }


  }

}





char * JSON_STRING;


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
  int tokenInfoLen;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
  NameTokenInfo * tokenInfo;
  tokenInfo = (NameTokenInfo*)malloc(sizeof(NameTokenInfo)*2);
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



  tokenInfoLen=jsonNameList(JSON_STRING, t, r, &tokenInfo);

  //printSavedInfo(JSON_STRING, t, r, tokenInfo,tokenInfoLen);
  //getTokenIndex(JSON_STRING, t, r, tokenInfo,tokenInfoLen);


  printf("*******************************************************************\n");
  printf("번호\t제 품 명\t제 조 사\t가 격\t개 수\t총 가 격\n");
  printf("*******************************************************************\n");
  printTable(JSON_STRING, t, r, tokenInfo,tokenInfoLen);

	return EXIT_SUCCESS;
}
