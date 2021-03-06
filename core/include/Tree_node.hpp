#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define NO_LABEL -1

class Tree_node{

	typedef struct Split{
		int var;
		float value;
		double quality;
	} Split;

	Split split;
	Tree_node* left;
	Tree_node* right;
	int label = NO_LABEL;
	int id;

  int cur_gini_iter;
  vector<int> l_hist,r_hist;
  double rsum,lsum,r2sum,l2sum;
  double gini_quality;
  float gini_value;
  int gini_var;
  void init_gini(int var);
  void run_gini();
  void init_gini_hists();
  void init_gini_sums();
  bool try_split();
	void split_node();
	void set_best_split();
	Split get_split(int var);
	Split get_split(int var,float value);
	bool try_set_label();
	void set_label();
	bool can_split();
	void set_branch(Tree_node** branch,const vector<int>& samp_indices);
	bool has_single_class();
	int get_common_label();
	vector<int> get_labels_hist(vector<int>::iterator indices_begin,vector<int>::iterator indices_end);
	void set_branches();
	vector<int> get_branch_indices(bool left);
	vector<int>::iterator sort_and_thresh_indicies(int var,float value);
	double get_gini(vector<int> hist);
	void search_for_node(ifstream& in_stream,streampos begin);


public:
	Tree_node(){id = next_id++;}
	Tree_node(ifstream& in_stream,streampos pos,int id);
	void grow();
	vector<int> indices;		
	unsigned int depth;

	static void init(int max_depth,int num_active_vars,int min_indices);
	static void set_max_label();

	int predict(cv::Mat data);
	void write_to_file(ofstream& out_stream);

	static cv::Mat data;
	static vector<int> labels,all_vars;
	static unsigned int max_depth,min_indices,max_label,num_active_vars,next_id;



};
