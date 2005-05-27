#ifndef PHRASE_PROPERTIY_LIST_POOL_INCLUDED
#define PHRASE_PROPERTIY_LIST_POOL_INCLUDED

typedef struct{
          double pos_x,        /*position*/      
                 pos_y,
                 pos_z;
          double ori_i,        /*orientation*/
                 ori_j,
                 ori_k,
                 ori_a;
          double size,         /*size*/
                 diam;         /*diameter*/
}PIPE_PROPERTY_LIST;

#define DEFINED_PHRASE_PROPERTY_LIST_TYPE PIPE_PROPERTY_LIST

#endif
