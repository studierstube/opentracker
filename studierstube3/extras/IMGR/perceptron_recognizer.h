#ifndef PERCEPTRON_RECOGNIZER_INCLUDED
#define PERCEPTRON_RECOGNIZER_INCLUDED

#include "recognizer.h"
#include "weight_list.h"
#include "weight_pool.h"
#include "weight_list_pool.h"
#include "feature_deviation_list.h"
#include "feature_deviation_pool.h"
#include "feature_deviation_list_pool.h"
#include "matrix.h"

#define MAX_CLUSTERS 100
#define MAX_FEATURES 100

template <class PROPERTY_LIST_TYPE,class FEATURE_TYPE,class FEATURE_LIST_TYPE>
class perceptron_recognizer:public recognizer<PROPERTY_LIST_TYPE, FEATURE_TYPE, FEATURE_LIST_TYPE>
{
	
 public:
	
	perceptron_recognizer(unsigned int noc);
	~perceptron_recognizer();
	virtual unsigned int recognize(FEATURE_LIST_TYPE input_features);
	virtual void learn(unsigned int id, FEATURE_LIST_TYPE input_features);
	void train(void);
	void find_clusters(void);
	void set_id(unsigned int cluster, unsigned int id);
	int load(const char *fn);
	int save(const char *fn);
	
 protected:
	unsigned int			number_of_clusters;
	unsigned int			number_of_features;
	unsigned int			*cluster_ids;//look up
	matrix_t<double>		clusters;//clusters : 0...n
};

#include "perceptron_recognizer.cpp"

#endif

