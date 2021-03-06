#include "Random_forest.hpp"
#include "probability.hpp"

void Random_forest::set_params(int num_trees,int max_depth,int num_active_vars,int min_indices){
	this->num_trees = num_trees;
	Tree::set_params(max_depth,num_active_vars,min_indices);
}


void Random_forest::train(cv::Mat data, cv::Mat labels){
    cout << "Random\n";
    assert(num_trees != NOT_INIT);
	trees.clear();
	for(int i = 0; i < num_trees; i++){
		vector<int> new_indices = get_random_indices(data.rows);
		trees.push_back(Tree(data,labels,new_indices));
	}
}

void Random_forest::predict(cv::Mat data, cv::Mat& results){
	Mat all_results;
	vector<int> res_array;
	for(Tree tree:trees){
		all_results.push_back(tree.predict(data));
	}
	for(int i = 0; i < all_results.cols;i++){
		Mat mat_col = all_results.col(i);
		vector<int> col;
		mat_col.copyTo(col);
		res_array.push_back(*max_element(col.begin(),col.end()));
	}
	results = cv::Mat(res_array,true);
}

void Random_forest::write(string filename){

}

vector<int> Random_forest::get_random_indices(int num_samples){
	vector<int> new_indices;
	while(new_indices.size()<num_samples)
		new_indices.push_back(get_random_uint(num_samples-1));
	return new_indices;
}
