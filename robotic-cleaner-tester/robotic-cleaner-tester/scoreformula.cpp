#include <map>
#include <string>
#include <iostream>

using namespace std;

typedef map<string,int>::const_iterator const_it_type;
#define SCORE_CANT_BE_CALCULATED -1

int calc_score(const map<string, int>& score_params){

	int position_in_copmetition, winner_num_steps, this_num_steps, sum_dirt_in_house, dirt_collected, is_back_in_docking;
	map<string, int &> params= {
		{string("position_in_copmetition"), position_in_copmetition}, 
		{string("winner_num_steps"), winner_num_steps}, 
		{string("this_num_steps"), this_num_steps}, 
		{string("sum_dirt_in_house"), sum_dirt_in_house}, 
		{string("dirt_collected"), dirt_collected}, 
		{string("is_back_in_docking"), is_back_in_docking}
	};
	for(auto & kv: params){
		const_it_type it = score_params.find(kv.first);
	  	if (it != score_params.end())
	    	return SCORE_CANT_BE_CALCULATED;
	    else
	    	kv.second = it->second;
	}
	for(auto & kv: params){
		cout << kv.first << " value: " << kv.second << endl;
	}
	// const_it_type it = score_params.find('position_in_copmetition');
 //  	if (it != score_params.end())
 //    	return SCORE_CANT_BE_CALCULATED;
 //    else
 //    	position_in_copmetition = it->second;
 //    it = score_params.find('winner_num_steps');
 //  	if (it != score_params.end())
 //    	return SCORE_CANT_BE_CALCULATED;
 //    else
 //    	winner_num_steps = it->second;
 //    it = score_params.find('this_num_steps');
 //  	if (it != score_params.end())
 //    	return SCORE_CANT_BE_CALCULATED;
 //    else
 //    	this_num_steps = it->second;
 //    it = score_params.find('sum_dirt_in_house');
 //  	if (it != score_params.end())
 //    	return SCORE_CANT_BE_CALCULATED;
 //    else
 //    	sum_dirt_in_house = it->second;
 //    it = score_params.find('dirt_collected');
 //  	if (it != score_params.end())
 //    	return SCORE_CANT_BE_CALCULATED;
 //    else
 //    	dirt_collected = it->second;
 //    it = score_params.find('is_back_in_docking');
 //  	if (it != score_params.end())
 //    	return SCORE_CANT_BE_CALCULATED;
 //    else
 //    	is_back_in_docking = it->second;

	int score = 2000 
		- (position_in_copmetition - 1) * 50 
		+ (winner_num_steps - this_num_steps) * 10
		- (sum_dirt_in_house - dirt_collected) * 3
		+ (is_back_in_docking? 50 : -200);
	return score > 0 ? score : 0;

}

int main(int argc, const char * argv[])
{
	map<string, int> params= {{"position_in_copmetition", 1}, {"winner_num_steps", 2}, {"this_num_steps", 3}, {"sum_dirtin_house", 4}, {"dirt_collected", 5}, {"is_back_in_docking", false}};
	// calc_score(params);
	return 1;
}