#ifndef FUZZY_LOGIC_RECOGNIZER_INCLUDED
#define FUZZY_LOGIC_RECOGNIZER_INCLUDED

#include "recognizer.h"
#include "weight_list.h"
#include "weight_pool.h"
#include "weight_list_pool.h"
#include "feature_deviation_list.h"
#include "feature_deviation_pool.h"
#include "feature_deviation_list_pool.h"

#define MAX_DEVIATION 1000000000000.0
#define DEVIATION_TRESHOLD 100.0

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
class fuzzy_logic_recognizer:public recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>
{
 public:
	fuzzy_logic_recognizer();
	~fuzzy_logic_recognizer();
	virtual unsigned int recognize(FEATURE_LIST_TYPE input_features);
	virtual void learn(unsigned int id, FEATURE_LIST_TYPE input_features);

 protected:
	 DEFINED_WEIGHT_LIST_TYPE wl;
     DEFINED_FEATURE_DEVIATION_LIST_TYPE fdl;
};

#include "fuzzy_logic_recognizer.cpp"

#endif

