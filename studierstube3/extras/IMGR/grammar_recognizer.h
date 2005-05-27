#ifndef GRAMMAR_RECOGNIZER_INCLUDED
#define GRAMMAR_RECOGNIZER_INCLUDED

#include "sample.h"
#include "sample_pool.h"
#include "property.h"
#include "property_pool.h"
#include "property_list.h"
#include "property_list_pool.h"
#include "phrase_property_list_pool.h"

FILE *gesture_phrase_file=NULL;

//input properties
DEFINED_PROPERTY_LIST_TYPE yystroke_properties;

//output properties
DEFINED_PHRASE_PROPERTY_LIST_TYPE yyphrase_properties;

//parser-stack
typedef struct {
  DEFINED_PROPERTY_LIST_TYPE            yystroke_properties;
  DEFINED_PHRASE_PROPERTY_LIST_TYPE     yyphrase_properties;
}PARSER_STACK_TYPE;

//parser stack type
#define YYSTYPE PARSER_STACK_TYPE 

int yyrecognize(void)
{
	double value;
        int    id;
	property<DEFINED_PROPERTY_TYPE,DEFINED_SAMPLE_TYPE> p;
     
	int i,token;
	fscanf(gesture_phrase_file,"%i\n",&token);
    for (i=0;i<yystroke_properties.get_dim();i++)
	{
	 fscanf(gesture_phrase_file,"%i:%lf ",&id,&value);
	 p.set(id,value);
	 yystroke_properties.set(i,p);
	}
	return token;
}

#include "y.tab.c"

void init_recognition(void)
{
	gesture_phrase_file=fopen("gesture_phrase.tmp","w");
    fclose(gesture_phrase_file);
    gesture_phrase_file=NULL;
}

void new_gesture_stroke(unsigned int s_id,DEFINED_PROPERTY_LIST_TYPE pl)
{
	int i;

	if(gesture_phrase_file==NULL)
     gesture_phrase_file=fopen("gesture_phrase.tmp","w");
    fprintf(gesture_phrase_file,"%i\n",s_id);
 
	for (i=0;i<pl.get_dim();i++)
	 fprintf(gesture_phrase_file,"%i:%lf ",pl.get(i).identification(),(double)pl.get(i).get());

	fprintf(gesture_phrase_file,"\n");
}

int recognize_gesture_phrase(DEFINED_PHRASE_PROPERTY_LIST_TYPE *ppl)
{
	int result;

	fclose(gesture_phrase_file);
	gesture_phrase_file=fopen("gesture_phrase.tmp","r");
	result=yyparse();
        *ppl=yyphrase_properties;
	fclose(gesture_phrase_file);
    gesture_phrase_file=NULL;
    return result;
}

#endif

