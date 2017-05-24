#pragma once
#include "Eco_genome.hpp"
#include "Data_set.hpp"

using namespace ml;

#define MIN_SAMPLE_COUNT 5
#define NOT_UPDATED -1

class Eco_creature{
protected:
	Eco_genome genome;	
	double fitness = NOT_UPDATED;
	double weighted_fitness = NOT_UPDATED;\
	double alpha;
	Ptr<RTrees> forest;
	bool trained = false;
	Mat labels;
	Mat images;
	
	Mat process(vector<Mat> data_set);
	void init_forest();
	void set_labels_and_images(Data_set data_set);
	void set_fitness(Data_set data_set);
	void set_weighted_fitness(Data_set data_set);
	void set_alpha(Data_set data_set);
	Mat predict();
	double calc_fitness(Mat results);
	double calc_weighted_fitness(Mat results,vector<double> weights);
	static int num_trees,max_tree_depth;
	void load_forest(string path);

public:
	Eco_creature(){genome.init();}
	Eco_creature(string genome_str,double alpha,string forest_path);
	Eco_creature(Eco_creature mother, Eco_creature father)
		{genome = *(new Eco_genome(mother.genome,father.genome));}
	friend ostream& operator<<(ostream& stream, const Eco_creature& eco_creature);
	bool operator<(const Eco_creature& rhs);
	int predict(Mat image);
	bool mutate(double rate){trained = false;return genome.mutate(rate);}
	void train(Data_set data_set);
	void update_fitness(Data_set data_set);
	void update_weighted_fitness(Data_set data_set);
	double get_fitness()const{return fitness;}
	double get_weighted_fitness()const{return weighted_fitness;}
	double get_alpha()const{return alpha;}
	Eco_genome get_genome()const{return genome;}
	void write_forest(string path);

	static void set_num_trees(int num){num_trees = num;}
	static void set_max_tree_depth(int depth){max_tree_depth = depth;}
};

