#include "defines.h"
#include "functions.h"
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <string>
using namespace std;

int main(int argc, char *argv[], char *envp[])
{
	//#########################################################################################
	// SIMULATOR PARAMETERS
	Params params;
	params.users_to_simulate = 88583; 					//10000000  
	//params.arrival_rate = 0.2; 						// by default 
	//params.service_time = 30;
	params.VNFs = 13;
	params.max_users_in_VNF = 32;
	params.VNF_boot_time = 3; 	 						//minutes 
	params.VNF_failure_time = 1440*32; 				    //1440min -> 1day  
	params.threshold_level = 1;    


	read_inter_arrival(); 								// read  inter-arrival times from file
	read_service_time(); 								// read service times from file 

	int temp = 0; 
	int seed = 1; 
	int simulation_step = 0;      

	//int moved_tasks = 0; 
	int moved_acc = 0; 
	int delayed_acc = 0; 
	int delayed_moved_acc = 0; 
	int migrations_acc = 0; 

	int VNF_activation_acc = 0;  
	int VNF_deactivation_acc = 0; 
	int VNF_drops_acc = 0; 
	int total_failures = 0; 

	bool var = true; 
	int failures; 

	//#########################################################################################

	// arguments (defined in script to run sim)
 	if (argc > 1)	
	 	params.VNFs = atof(argv[1]);          
	 	params.max_users_in_VNF = atof(argv[2]);          
	 	params.VNF_boot_time = atof(argv[3]);          
	 	params.VNF_failure_time = atof(argv[4]);          
	 	seed = atof(argv[5]);
		failures = atof(argv[6]);
		std::string file_name_sim = argv[7];               

	srand(seed);

	//cout << file_name_sim << endl;        

	vector<int> on_thresholds;
	for (int i = 1; i < params.VNFs; i++) 
	{
		int threshold = int(ceil(params.threshold_level * params.max_users_in_VNF * i));   
		on_thresholds.push_back(threshold);    
	}
	on_thresholds.push_back(-1); 

	vector<int> off_thresholds;
	off_thresholds.push_back(-1);
	for (int i = 1; i < params.VNFs; i++)  
	{
		int threshold = on_thresholds[i - 1] - 1;	
		off_thresholds.push_back(threshold);  
	}

	//#########################################################################################

	// FILE TO DUMP RESULTS
	char file_name[256];
	sprintf(file_name, file_name_sim.c_str());                               
	ofstream output_file; 
	output_file.open(file_name, fstream::app);     
	

	// INITIALIZATION
	unsigned int simulated_users = 0;
	double time_last_event = 0.0;

	list<Event_element> event_list; 

	System system;
	system.VNFs.resize(params.VNFs);
	int idx = 0;

	for (auto &VNF : system.VNFs)
	{ 
		VNF.state = off;				//empezamos con todo encendido        
		VNF.VNF_id = idx;        
		idx++; 
	}

	system.VNFs[0].state = on;  
	system.users = 0;
	system.VNFs_on = 1;
	system.VNFs_going_on = 0;

	// variables for statistical purposes
	Statistics statistics;
	statistics.moved_tasks = 0; 
	statistics.accum_time_in_states.resize(params.VNFs + 1);
	statistics.num_available_periods = 1;
	statistics.num_non_available_periods = 0;
	statistics.accum_available_time = 0.0;
	statistics.accum_non_available_time = 0.0;
	statistics.accum_time_in_queue = 0.0;

	statistics.moved_to_queue = 0;
	statistics.delayed_users = 0;
	statistics.delayed_and_moved = 0;
	statistics.user_migrations = 0;

	statistics.VNF_activations = 0;
	statistics.VNF_deactivations = 0; 
	statistics.VNF_drop = 0; 

	statistics.boot_time = 0; 
	 

	// GENERATION OF THE FIRST ARRIVAL
	Event_element event_aux;
	event_aux.type = user_arrival;
	//event_aux.time = exponential(params.arrival_rate);
	event_aux.time = inter_arrival();
	event_aux.id = -1;
	event_list.push_back(event_aux);

	// GENERATION OF THE FIRST FAILURE
	event_aux.type = VNF_failure;
	event_aux.time = exponential(1.0 / params.VNF_failure_time);
	event_aux.id = 0;
	event_list.push_back(event_aux);

	//#########################################################################################
	// SIMULATION LOOP 
	while(var)
	{

	while (simulated_users < params.users_to_simulate) 
	{
		// LOOK FOR NEXT EVENT
		event_aux = f_timing(event_list);
		switch (event_aux.type)
		{
		case user_arrival:
			f_user_arrival(event_aux.time, time_last_event, system, event_list, params, on_thresholds, statistics);
			break;
		case user_departure:
			f_user_departure(event_aux, time_last_event, system, event_list, params, off_thresholds, on_thresholds, statistics);
			simulated_users++;
			break;
		case VNF_activation:
			f_VNF_activation(event_aux, time_last_event, system, event_list, params, statistics);
			break;
		case VNF_failure:
			f_VNF_failure(event_aux, time_last_event, system, event_list, params, on_thresholds, statistics);
			break;
		}
	}

		 

		//#########################################################################################
		int VNF_counter = 0;
		int user_counter = 0;
		double avg_active_VNFs = 0.0;
		double avg_number_users = 0.0;

		for (auto &row : statistics.accum_time_in_states)
		{
			user_counter = 0;	
			for (auto element : row)
			{
				element /= event_aux.time;
				avg_active_VNFs += element * VNF_counter;
				avg_number_users += element * user_counter;
				//cout << "VNF: " << VNF_counter << " USER: " << user_counter << " PROB: " << element << endl;
				user_counter++;
			}
			VNF_counter++;
		}

		temp = simulated_users; 

		double MTTF = statistics.accum_available_time / statistics.num_available_periods;
		double MTTR = statistics.accum_non_available_time / statistics.num_non_available_periods;
		double avg_queue_time = statistics.accum_time_in_queue / simulated_users;
		double prob_moved_to_queue = double(statistics.moved_to_queue) / simulated_users;
		double VNF_activation_rate = double(statistics.VNF_activations) / event_aux.time;
		double prob_delayed_user = double(statistics.delayed_users) / simulated_users;
		double delayed_and_moved = double(statistics.delayed_and_moved) / simulated_users;
		
		double reliability_moved = 1 - prob_moved_to_queue;
		double reliability_delayed = 1 - prob_delayed_user; 
		
		cout << "MTTF: " << MTTF << endl;
		cout << "MTTR: " << MTTR << endl;
		cout << MTTF / (MTTF + MTTR) << " " << statistics.accum_available_time / event_aux.time << endl;
		cout << "Queue time: " << avg_queue_time << endl;
		cout << "Day: " << temp << endl; 
		cout << "Users moved to queue: " << statistics.moved_to_queue << endl;
		cout << "Average number of users: " << avg_number_users << endl;
		cout << "Average number of active VNFs: " << avg_active_VNFs << endl;
		//cout << "Probability of moving a user to queue: " << prob_moved_to_queue << endl;
		

		//METRICS REGARDING FAILURES 
		moved_acc = moved_acc + statistics.moved_to_queue; 
		delayed_acc = delayed_acc + statistics.delayed_users; 
		delayed_moved_acc = delayed_moved_acc + statistics.delayed_and_moved;
		migrations_acc = migrations_acc + statistics.user_migrations; 


		//METRICS REGARDING VNFs
		VNF_activation_acc = VNF_activation_acc + statistics.VNF_activations; 
		VNF_deactivation_acc = VNF_deactivation_acc + statistics.VNF_deactivations; 
		VNF_drops_acc = VNF_drops_acc + statistics.VNF_drop; 

		/*LIMPIAMOS LOS VECTORES*/
		on_thresholds.clear();  
		off_thresholds.clear();  

		if(delayed_moved_acc <= failures) 
		{
			output_file << params.VNFs << "," << params.max_users_in_VNF << "," << params.VNF_boot_time << "," << params.VNF_failure_time << "," << params.threshold_level << "," << seed << "," << moved_acc << "," << delayed_acc << "," << delayed_moved_acc << "," 
			<< migrations_acc << "," << VNF_activation_acc << "," << VNF_deactivation_acc << "," 
			<< VNF_drops_acc << "," << avg_active_VNFs << "," << avg_number_users << ","
			<< statistics.boot_time/statistics.simulation_time << "," << statistics.simulation_time << endl;    

			//cout << "threshold level: " << params.threshold_level << endl; 
			var = false;  
		}
		else
		{
			params.threshold_level = params.threshold_level - 0.05; 

			for (int i = 1; i < params.VNFs; i++) 
			{
				int threshold = int(ceil(params.threshold_level * params.max_users_in_VNF * i));    
				on_thresholds.push_back(threshold);       
			}
			on_thresholds.push_back(-1);   

			// for (int i = 0; i < params.VNFs; i++) 
			// {
			// 	cout << on_thresholds[i] << endl;  
			// }

			off_thresholds.push_back(-1); 
			for (int i = 1; i < params.VNFs; i++)  
			{
				int threshold = on_thresholds[i - 1] - 1;	
				off_thresholds.push_back(threshold);  
			}

			simulated_users = 0; 
		}

		
		//we reset statistics 
		moved_acc = 0; 
		delayed_acc = 0; 
		delayed_moved_acc = 0; 

		migrations_acc = 0; 
		VNF_activation_acc = 0;
		VNF_deactivation_acc = 0;  
		VNF_drops_acc = 0; 


		statistics.moved_tasks = 0; 
		statistics.moved_to_queue = 0; 
		statistics.delayed_users = 0; 
		statistics.delayed_and_moved = 0; 
		statistics.user_migrations = 0;  

		statistics.VNF_activations = 0; 
		statistics.VNF_deactivations = 0; 
		statistics.VNF_drop = 0; 

		statistics.boot_time = 0; 
		statistics.simulation_time = 0; 
		avg_active_VNFs = 0; 

	}

	//}
	//}

	//} 
	output_file.close();   
	
}
