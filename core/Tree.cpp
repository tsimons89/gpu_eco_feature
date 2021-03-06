#include "Tree.hpp"
#include <ctime>

void Tree::set_params(int max_depth,int num_active_vars,int min_indices){
	Tree_node::init(max_depth,num_active_vars,min_indices);
}

Tree::Tree(cv::Mat data, cv::Mat labels,vector<int> indices){
    clock_t b = clock();
    int idx_size = indices.size();
	vector<int> copy = indices;
	Tree_node::data = data;
	if(Tree_node::num_active_vars == 0)
	    Tree_node::num_active_vars = sqrt(data.cols)+1;
	Tree_node::all_vars.resize(data.cols);
	iota(Tree_node::all_vars.begin(),Tree_node::all_vars.end(),0);
	Tree_node::labels.assign((int*)labels.data ,(int*)labels.data + labels.total());
	Tree_node::set_max_label();
	root.depth = 0;
	root.indices = indices;
	root.grow();
	clock_t e = clock();
	cout << "\tTree::Tree: " << e -b << endl;
}

cv::Mat Tree::predict(cv::Mat data){
	vector<int> res_array;
	for(int i = 0; i < data.rows; i++)
		res_array.push_back(root.predict(data.row(i)));
	return cv::Mat(res_array,true).t();
}

void Tree::write(string filename){
	ofstream out_stream(filename);
	root.write_to_file(out_stream);
	out_stream.close();
}

Tree::Tree(string filename){
	ifstream in_stream(filename);
	root = Tree_node(in_stream,in_stream.beg,0);
}
