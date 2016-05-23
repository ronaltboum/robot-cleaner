#include <map>
#include <string>
#include <iostream>
#include <math.h> 

using namespace std;

typedef map<string,int>::const_iterator const_it_type;
#define SCORE_CANT_BE_CALCULATED -1

extern "C" int calc_score(const map<string, int>& score_params){
	int position_in_copmetition, simulation_steps, winner_num_steps, this_num_steps, sum_dirt_in_house, dirt_collected,
		is_back_in_docking, actual_position_in_competition;
	map<string, int &> params= {{"actual_position_in_competition",actual_position_in_competition},
		{"simulation_steps", simulation_steps},	{"winner_num_steps", winner_num_steps}, {"this_num_steps", this_num_steps}, {"sum_dirt_in_house", sum_dirt_in_house},
		{"dirt_collected", dirt_collected}, {"is_back_in_docking", is_back_in_docking}};
	for(auto & kv: params){
		const_it_type it = score_params.find(kv.first);
	  	if (it == score_params.end()){
	    	return SCORE_CANT_BE_CALCULATED;
	  	}
	    else
	    	kv.second = it->second;
	}
	if(sum_dirt_in_house == dirt_collected && is_back_in_docking)
		position_in_copmetition = actual_position_in_competition < 4 ? actual_position_in_competition : 4;
	else
		position_in_copmetition = 10; 

	int score = 2000 
		- (position_in_copmetition - 1) * 50 
		+ (winner_num_steps - this_num_steps) * 10
		- (sum_dirt_in_house - dirt_collected) * 3
		+ (is_back_in_docking? 50 : -200);
	
	return max(0,score);
}

// int main(int argc, const char * argv[])
// {
// 	map<string, int> params= {{"position_in_copmetition", 1}, {"winner_num_steps", 2}, {"this_num_steps", 3}, {"sum_dirt_in_house", 4}, {"dirt_collected", 5}, {"is_back_in_docking", false}};
// 	cout << "score: " << calc_score(params) << endl;
// 	return 1;
// }
