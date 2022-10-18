#pragma once
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <limits>
#include <deque>
#include <algorithm>    

using namespace std;

// DATA TYPES

enum event_type { user_arrival, user_departure, VNF_activation, VNF_failure };
enum VNF_status { on, off, going_on };

struct Params
{
	unsigned int users_to_simulate;			    // number of simulated users 
	double arrival_rate;						// arrival rate of users per minute (Poisson)
	double service_time;						// average service time in minutes (exponential)
	int VNFs;									// number of VNFs
	//unsigned int max_users_in_VNF;		    // max number of users served by each VNF
	float max_users_in_VNF;				        // max number of users served by each VNF
	double VNF_boot_time;						// average delay in minutes to switch on a VNF (exponential)
	double VNF_failure_time;					// average delay in minutes for a VNF to be broken (exponential)
	double threshold_level; 
};

struct User
{
	double arrival_time;						// in seconds
	double pending_time;						// remaining data to transmit at a time instant
	double total_time;							// required service time
	int user_id;			
	bool moved_to_queue;
	bool delayed_at_arrival;
};

struct Event_element
{
	event_type type;
	double time;
	int id;										// id of the user (for a departure) or the VNF (for a VNF failure)
};

struct VNF
{
 	list <User> users;							// list of the users in the VNF
	VNF_status state;							// state of the AP
	int VNF_id;
};

struct System
{
	vector <VNF> VNFs;
	deque <User> user_queue;
	int users;
	int VNFs_going_on;
	int VNFs_on;
};

 struct Statistics
{
	 vector <vector <double>> accum_time_in_states;
	 int num_available_periods;
	 int num_non_available_periods;
	 double accum_available_time;
	 double accum_non_available_time;
	 double accum_time_in_queue;
	 int moved_to_queue; 
	 int VNF_activations;
	 int VNF_deactivations; 
	 int VNF_drop; 
	 int user_migrations; 
	 int delayed_users;										// Number of users that have had to wait
	 int delayed_and_moved;
	 double simulation_time;  
	 double boot_time; 
	 int moved_tasks; 
}; 