#include "Tree_node.hpp"

cv::Mat Tree_node::data;
vector<int> Tree_node::labels,Tree_node::all_vars;
unsigned int Tree_node::max_depth,Tree_node::min_indices,Tree_node::max_label,Tree_node::num_active_vars,Tree_node:: next_id = 0;

struct indexed_compare
{
    indexed_compare(const float* arr) : arr(arr) {}
    bool operator ()(int a, int b) const { return arr[a] < arr[b]; }
    const float* arr;
};

void Tree_node::init(int _max_depth,int _num_active_vars,int _min_indices){
	max_depth = _max_depth;
	num_active_vars = _num_active_vars;
	min_indices = _min_indices;
}

void Tree_node::set_max_label(){
	max_label = 0;
	for(unsigned int label: labels)
		max_label = max(max_label,label);
}

void Tree_node::grow(){
	if(!try_split())
		return;
	left->grow();
	right->grow();
}

bool Tree_node::try_split(){
	if(try_set_label())
		return false;
	split_node();
	return true;
}

bool Tree_node::try_set_label(){
	if(can_split())
		return false;
	set_label();
	return true;
}

bool Tree_node::can_split(){
	return depth < max_depth && indices.size() >= min_indices && !has_single_class();
}

bool Tree_node::has_single_class(){
	for(unsigned int i = 1; i < indices.size(); i++){
		if(labels.at(indices.at(i)) != labels.at(indices.at(0)))
			return false;
	}
	return true;
}

void Tree_node::set_label(){
	int max_count = 0,max_label;
	vector<int> hist = get_labels_hist(indices.begin(),indices.end());
	for(unsigned int label = 0; label < hist.size();label++){
		if(hist.at(label) > max_count){
			max_count = hist.at(label);
			max_label = label;
		}
	}
	label = max_label;
}

vector<int> Tree_node::get_labels_hist(vector<int>::iterator indices_begin,vector<int>::iterator indices_end){
	vector<int> hist(max_label+1,0);
	vector<int>::iterator iter;
	for(iter = indices_begin; iter != indices_end;iter++)
		hist[labels.at(*iter)]++;
	return hist;
}

void Tree_node::split_node(){
	set_best_split();
	set_branches();
}

void Tree_node::set_best_split(){
	random_shuffle(all_vars.begin(),all_vars.end());
	Split best_split = get_split(all_vars[0]);
	for(int var_idx = 0; var_idx < num_active_vars;var_idx++){
		Split split = get_split(all_vars[var_idx]);
		if(split.quality > best_split.quality)
			best_split = split;
	}
	split = best_split;
}

Tree_node::Split Tree_node::get_split(int var){
	init_gini(var);
	run_gini();
	return 	Split{var,gini_value,gini_quality};
}

void Tree_node::init_gini(int var){
  gini_var = var;
  sort(indices.begin(),indices.end(),indexed_compare((float*)data.col(var).data));
  cur_gini_iter = 0;
  init_gini_hists();
  init_gini_sums();
}

void Tree_node::init_gini_hists(){
  l_hist = vector<int>(max_label+1,0);
  r_hist = get_labels_hist(indices.begin(),indices.end());
}

void Tree_node::init_gini_sums(){
  rsum = lsum = r2sum = l2sum = 0;
  for(int hist_el:r_hist){
    rsum+=hist_el;
    r2sum+=hist_el*hist_el;
  }
}

void Tree_node::run_gini(){
  gini_quality = -1;
  for(int idx:indices){
    if(idx == indices.back())
      break;
    rsum--;
    lsum++;
    r2sum-=r_hist[labels[idx]]*2-1;
    l2sum+=l_hist[labels[idx]]*2+1;
    r_hist[labels[idx]]--;
    l_hist[labels[idx]]++;
    double quality = (l2sum+r2sum)/(lsum*rsum);
    if(quality > gini_quality){
      gini_quality = quality;
      gini_value = data.at<float>(idx,gini_var);
    }
  }
}

Tree_node::Split Tree_node::get_split(int var,float value){
	Split new_split = {var,value};
	vector<int>::iterator thresh_bound = sort_and_thresh_indicies(var,value);
	new_split.quality = get_gini(get_labels_hist(indices.begin(),thresh_bound)) +  get_gini(vector<int>(thresh_bound,indices.end()));
	return new_split;
}
double Tree_node::get_gini(vector<int> hist){
	double sum2 = 0,sum = 0;
	for(int count:hist){
		sum2 += count*count;
		sum += count;
	}
	if(sum == 0)
		return 0;
	return sum2/sum;
}

void Tree_node::set_branches(){
	vector<int>::iterator thresh_bound = sort_and_thresh_indicies(split.var,split.value);
	set_branch(&left,vector<int>(indices.begin(),thresh_bound));
	set_branch(&right,vector<int>(thresh_bound,indices.end()));
}

void Tree_node::set_branch(Tree_node** branch,const vector<int>& samp_indices){
	*branch = new Tree_node();
	(*branch)->depth = depth + 1;
	(*branch)->indices = samp_indices;
}

vector<int>::iterator Tree_node::sort_and_thresh_indicies(int var,float value){
	vector<int>::iterator thresh_boundry;
	sort(indices.begin(),indices.end(),indexed_compare((float*)data.col(var).data));
	for(thresh_boundry = indices.begin();thresh_boundry != indices.end();thresh_boundry++){
		if(data.at<float>(*thresh_boundry,var) >= value)
			break;
	}
	return thresh_boundry;
}

int Tree_node::predict(cv::Mat data){
	if(label != NO_LABEL)
		return label;
	if(data.at<float>(split.var) < split.value)
		return left->predict(data);
	return right->predict(data);
}

void Tree_node::write_to_file(ofstream& out_stream){
	out_stream << id <<" " << label;
	if(label != NO_LABEL){
		out_stream << endl;
		return;
	}
	out_stream << " " << split.var << " " << split.value << " " << left->id << " " << right->id << endl;
	left->write_to_file(out_stream);
	right->write_to_file(out_stream);
}

Tree_node::Tree_node(ifstream& in_stream,streampos pos,int id){
	this->id = id;
	search_for_node(in_stream,pos);
	in_stream >> label;
	int left_id,right_id;
	if(label == NO_LABEL){
		in_stream >> split.var >> split.value >> left_id >> right_id;
		streampos cur_pos = in_stream.tellg();
		left = new Tree_node(in_stream,cur_pos,left_id);
		right = new Tree_node(in_stream,cur_pos,right_id);
	}

}
void Tree_node::search_for_node(ifstream& in_stream,streampos begin){
	int found_id = -1;
	in_stream.seekg(begin);
	string dummy;
	while(1){
		in_stream >> found_id;
		if(found_id == id)
			return;
		getline(in_stream,dummy);
	}
}
