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
 		pFile = fopen( "./data.json", "r" );
 		if( pFile == NULL )
 		{
 			printf("no file exist\n");
 			return;
 		}
 		else
 		{
 			char strTemp;

 			while( fgets( STRING,300,pFile )!=NULL )
 			{
 				str = (char *)realloc(str,sizeof(char)*strlen(STRING)+sizeof(char)*strlen(str));
 				strcat(str,STRING);
 			}

   		fclose( pFile );

		}
		return str;
	}

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount)
{
  int i;
  int j=1;
  for ( i = 0; i < tokcount; i++)
  {
    if(t[i+1].size>0&&t[i+1].type==3){
      printf("[Name %d]%.*s\n", j++,t[i+1].end-t[i+1].start,
        jsonstr + t[i+1].start);
      }
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

	JSON_STRING = readJSONFILE();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
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
 jsonNameList(JSON_STRING, t, r);


	return EXIT_SUCCESS;
}
