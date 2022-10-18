#include "functions.h"
#include "defines.h"
#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>


using namespace std;

int x; 
int y; 
int i = 0; 
int j = 0; 
int p = 0; 
int k = 0; 
//vector <double> inter_arrival_values; 	
//vector <double> service_time_values; 

std::vector<double> inter_arrival_values;
std::vector<double> service_time_values;

//std::vector<long double> inter_arrival_values(88583);
//std::vector<long double> service_time_values(88583);

// READ VALUES FROM INTER-ARRIVAL FILE AND STORE IN A VECTOR
void read_inter_arrival()
{
	std::ifstream ifile("path/to/inter_arrival_time_trace.txt", std::ios::in);
	
	//check to see that the file was opened correctly:
	if (!ifile.is_open()) {
		std::cerr << "There was a problem opening the input file!\n";
		exit(1);//exit or do additional error checking
	}

	double num = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num) {
        inter_arrival_values.push_back(num); 
    }


}

//RETURN AN INTER-ARRIVAL VALUE
double inter_arrival()
{ 
	p++;
	if(p == inter_arrival_values.size())
	{
		p = 0; 
	}
	return inter_arrival_values[p]; 
}


// READ VALUES FROM SERVICE TIME FILE AND STORE IN A VECTOR
void read_service_time()
{
	std::ifstream ifile("path/to/service_time_trace.txt", std::ios::in);
	
	//check to see that the file was opened correctly:
	if (!ifile.is_open()) {
		std::cerr << "There was a problem opening the input file!\n";
		exit(1);//exit or do additional error checking
	}

	double num = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num) {
        service_time_values.push_back(num);  
    }

}

//RETURN A SERVICE TIME VALUE
double service_time()
{ 
	k++; 
	if(k == service_time_values.size())
	{
		k = 0; 
	}
	return service_time_values[k]; 
}

double exponential(double lambda)
{
	int r_int = rand();
	r_int =  (rand() % RAND_MAX); 
	r_int = (r_int == 0) ? 1 : r_int;
	r_int = (r_int == RAND_MAX	) ? (RAND_MAX - 1) : r_int;
	double r = ((double)r_int) / RAND_MAX;
	double expo = -(log(r) / lambda);
	//cout << expo << endl; 
	return expo;
}

double pareto(double lambda) 
{
	double unif;
	unif = (double) rand() /(double) RAND_MAX;
	if (unif > 0.99999)
	{
		unif = 0.99999;
	}		
	double shape = 1.5;
	double scale = (1/lambda)*(shape-1)/shape;
	//cout << "1/Lambda: " << 1/lambda << "\n Value: " << scale / pow((1-unif), (1/shape)) << endl; 
	//cout << scale / pow((1-unif), (1/shape)) << endl; 
	return scale / pow((1-unif), (1/shape));
}


Event_element f_timing(list<Event_element>& event_list)
{
	double next_sim_time = numeric_limits<double>::infinity();
	list<Event_element>::iterator next_event_it;

	for (auto it = event_list.begin(); it != event_list.end(); ++it)
		if (it->time < next_sim_time)
		{
			next_sim_time = it->time;
			next_event_it = it;
		}

	Event_element event_aux = *(next_event_it);
	event_list.erase(next_event_it); 

	return event_aux;
}


void f_user_arrival(double sim_time, double& time_last_event, System& system, list<Event_element>& event_list,  Params& params, vector<int>& on_thresholds, Statistics& statistics)
{
	static int user_id = params.VNFs;

	// we update statistics
	if (statistics.accum_time_in_states[system.VNFs_on].size() < system.users + 1)
		statistics.accum_time_in_states[system.VNFs_on].resize(system.users + 1, 0.0);

	statistics.accum_time_in_states[system.VNFs_on][system.users] += sim_time - time_last_event;
	
	bool empty_queue = (system.user_queue.size() == 0);
	
	if (system.user_queue.size() == 0)
		statistics.accum_available_time += sim_time - time_last_event;
	else
		statistics.accum_non_available_time += sim_time - time_last_event;
	
	// we recompute the pending_time of all users
	for (auto& VNF : system.VNFs)
		for (auto& user : VNF.users)
			user.pending_time -= sim_time - time_last_event;
	
	// the new user is generated
	User new_user;
	new_user.arrival_time = sim_time;
	new_user.pending_time = service_time(); 
	//new_user.pending_time = exponential(1 / params.service_time);
	//new_user.pending_time = pareto(1 / params.service_time);
	new_user.total_time = new_user.pending_time;
	new_user.user_id = user_id;
	new_user.moved_to_queue = false;
	new_user.delayed_at_arrival = false;
	user_id++;

	// generation of the next arrival
	Event_element next_arrival_evnt; 
	next_arrival_evnt.type = user_arrival;
	//next_arrival_evnt.time = sim_time + exponential(params.arrival_rate);
	next_arrival_evnt.time = sim_time + inter_arrival(); 
	//statistics.time = sim_time + exponential(params.arrival_rate);
	statistics.simulation_time = next_arrival_evnt.time; //sim_time + inter_arrival();	
	next_arrival_evnt.id = -1;
	event_list.push_back(next_arrival_evnt);

	// the new user is assigned to a VNF or the system queue
	bool user_in_VNF = false;

 	for (auto& VNF : system.VNFs)
		if ((VNF.state == on) && (VNF.users.size() < params.max_users_in_VNF))
		{
			VNF.users.push_back(new_user);
			user_in_VNF = true;

			Event_element departure_evnt;
			departure_evnt.type = user_departure;
			departure_evnt.time = sim_time + new_user.pending_time;
			departure_evnt.id = new_user.user_id;
			event_list.push_back(departure_evnt);

			break;
		}

	if (!user_in_VNF)
	{
		new_user.delayed_at_arrival = true;
		system.user_queue.push_back(new_user);
	}

	// we see if we have to switch on an VNF
	system.users++;

	int aux_VNFs = system.VNFs_going_on + system.VNFs_on;

	//if (system.users == on_thresholds[aux_VNFs - 1]) && on_thresholds[aux_VNFs - 1] != -1
	if (system.users >= on_thresholds[aux_VNFs - 1])  
		for (auto& VNF : system.VNFs) 
			if (VNF.state == off)
			{
				system.VNFs_going_on++;
				//statistics.VNF_activations++;
				VNF.state = going_on;

				Event_element VNF_activation_evnt;
				VNF_activation_evnt.type = VNF_activation;
				//VNF_activation_evnt.time = sim_time + exponential(1 / params.VNF_boot_time);
				VNF_activation_evnt.time = sim_time + params.VNF_boot_time;
				//VNF_activation_evnt.time = params.VNF_boot_time;
				statistics.boot_time = statistics.boot_time + params.VNF_boot_time;  
				//statistics.boot_time = VNF_activation_evnt.time - sim_time;  

				//cout << "Boot time consumption: " << statistics.boot_time_consumption << endl;   
				  
				//VNF_activation_evnt.time = sim_time + params.VNF_boot_time;
				VNF_activation_evnt.id = VNF.VNF_id;
				//cout << "VNF ID: " << VNF.VNF_id << endl;

				event_list.push_back(VNF_activation_evnt);
				break;
			}

	// Comprobamos si ha habido un cambio de periodo libre a periodo no libre
	if ((empty_queue) && (system.user_queue.size() > 0))
		statistics.num_non_available_periods++;

	time_last_event = sim_time;
}


void f_VNF_activation(Event_element& current_event, double& time_last_event, System& system, list<Event_element>& event_list,  Params& params, Statistics& statistics)
{
	// we compute statistics
	if (statistics.accum_time_in_states[system.VNFs_on].size() < system.users + 1)
		statistics.accum_time_in_states[system.VNFs_on].resize(system.users + 1, 0.0);

	statistics.accum_time_in_states[system.VNFs_on][system.users] += current_event.time - time_last_event;

	bool empty_queue = (system.user_queue.size() == 0);

	if (system.user_queue.size() == 0)
		statistics.accum_available_time += current_event.time - time_last_event;
	else
		statistics.accum_non_available_time += current_event.time - time_last_event;
	
	// we recompute the pending_time of all users
	for (auto& VNF : system.VNFs)
		for (auto& user : VNF.users)
			user.pending_time -= current_event.time - time_last_event;
	
	// we look for a VNF with state going_on to put its state into on
	VNF& VNF_to_activate = system.VNFs[current_event.id];

	VNF_to_activate.state = on;
	system.VNFs_going_on--;
	system.VNFs_on++;
	statistics.VNF_activations++;						//added 

	
	// we generate the failure of the VNF
	Event_element next_failure_evnt;
	next_failure_evnt.type = VNF_failure;
	next_failure_evnt.time = current_event.time + exponential(1.0 / params.VNF_failure_time);
	next_failure_evnt.id = VNF_to_activate.VNF_id; 
	event_list.push_back(next_failure_evnt);

	// if there are any user waiting in the queue, we move him to the new NFV
	while (system.user_queue.size() > 0)
	{
		User& user_in_queue = system.user_queue.front();
		
		Event_element departure_evnt;
		departure_evnt.type = user_departure;
		departure_evnt.time = current_event.time + user_in_queue.pending_time;
		departure_evnt.id = user_in_queue.user_id;
		event_list.push_back(departure_evnt);

		VNF_to_activate.users.push_back(user_in_queue);
		system.user_queue.pop_front();

		if (VNF_to_activate.users.size() == params.max_users_in_VNF)
			break;
	}

	time_last_event = current_event.time;

	// Comprobamos si ha habido un cambio de periodo libre a periodo no libre
	if ((!empty_queue) && (system.user_queue.size() == 0))
		statistics.num_available_periods++;
}


void f_user_departure(Event_element& current_event, double& time_last_event, System& system, list<Event_element>& event_list,  Params& params, vector<int>& off_thresholds, vector<int>& on_thresholds, Statistics& statistics)
{
	// we compute statistics
	if (statistics.accum_time_in_states[system.VNFs_on].size() < system.users + 1)
		statistics.accum_time_in_states[system.VNFs_on].resize(system.users + 1, 0.0);

	statistics.accum_time_in_states[system.VNFs_on][system.users] += current_event.time - time_last_event;

	bool empty_queue = (system.user_queue.size() == 0);

	if (system.user_queue.size() == 0)
		statistics.accum_available_time += current_event.time - time_last_event;
	else
		statistics.accum_non_available_time += current_event.time - time_last_event;

	// we recompute the pending_time of all users
	for (auto& VNF : system.VNFs)
		for (auto& user : VNF.users)
			user.pending_time -= current_event.time - time_last_event;
	
	// The leaving user is searched and deleted
	bool break_outer_for = false;
	for (auto& VNF : system.VNFs)
	{
		for (auto user_it = VNF.users.begin(); user_it != VNF.users.end(); ++user_it)
			if (user_it->user_id == current_event.id)
			{
				statistics.accum_time_in_queue += current_event.time - user_it->arrival_time - user_it->total_time;

				// we update the statistics
				if (user_it->moved_to_queue)
					statistics.moved_to_queue++;

				if (user_it->delayed_at_arrival)
					statistics.delayed_users++;

				if (user_it->moved_to_queue || user_it->delayed_at_arrival)
					statistics.delayed_and_moved++; 

				VNF.users.erase(user_it);

				// if there are waiting users in the queue, we move the first one to the VNF
				if (system.user_queue.size() > 0)
				{
					User& user_in_queue = system.user_queue.front();

					Event_element departure_evnt;
					departure_evnt.type = user_departure;
					departure_evnt.time = current_event.time + user_in_queue.pending_time;
					departure_evnt.id = user_in_queue.user_id;
					event_list.push_back(departure_evnt);

					VNF.users.push_back(user_in_queue);
					system.user_queue.pop_front();
				}
				break_outer_for = true;
				break;
			}
		if (break_outer_for)
			break;
	}

	// we see if we have to switch off an VNF
	system.users--;
	
	int aux_VNFs = system.VNFs_on;

	if (system.users == off_thresholds[aux_VNFs - 1])
	{
		vector<VNF>::iterator VNF_to_switch_off_it;
		unsigned int min_users = params.max_users_in_VNF + 1;

		for (auto VNF_it = system.VNFs.begin(); VNF_it != system.VNFs.end(); ++VNF_it)
			if ((VNF_it->users.size() < min_users) && (VNF_it->state == on))
			{
				min_users = VNF_it->users.size();
				VNF_to_switch_off_it = VNF_it;
			}

		// apagamos la VNF candidata
		system.VNFs_on--;
		statistics.VNF_deactivations++; 
		VNF_to_switch_off_it->state = off;

		// eliminamos el evento de su fallo o de su encendido
		for (auto event_it = event_list.begin(); event_it != event_list.end(); ++event_it)
			if (event_it->id == VNF_to_switch_off_it->VNF_id)
			{
				event_list.erase(event_it);
				break;
			}

		// if there are users in the VNF, we move them to another VNF or to the queue
		if (min_users > 0)
			while (VNF_to_switch_off_it->users.size() > 0)
			{
				bool moved_to_VNF = false;
				User& user_to_move = VNF_to_switch_off_it->users.front();

				// we try to put the user into another VNF
				for (auto& dest_VNF : system.VNFs)
					if ((dest_VNF.state == on) && (dest_VNF.users.size() < params.max_users_in_VNF))
					{
						statistics.moved_tasks = dest_VNF.users.size();   
						//cout << statistics.moved_tasks << endl;  
						//cout << dest_VNF.users.size() << endl; 
						dest_VNF.users.push_back(user_to_move); 
						VNF_to_switch_off_it->users.pop_front();
						moved_to_VNF = true;
						statistics.user_migrations++;   
						break;
					}

				if (!moved_to_VNF)
				{
					// we erase the departures in the event list associated to the user 
					for (auto event_it = event_list.begin(); event_it != event_list.end(); ++event_it)
						if ((event_it->id == user_to_move.user_id) && (event_it->type == user_departure))
						{
							event_list.erase(event_it);
							break;
						}

					// we move the user to the queue
					system.user_queue.push_front(user_to_move);
					VNF_to_switch_off_it->users.pop_front();
				}
			}
	}

	bool erase_VNF_going_on = false;

	for (auto& th : on_thresholds)
		if (system.users == th - 1)
		{
			erase_VNF_going_on = true;
			break;
		}

	if (erase_VNF_going_on)
	{
		vector<VNF>::iterator VNF_to_switch_off_it;

		for (auto VNF_it = system.VNFs.begin(); VNF_it != system.VNFs.end(); ++VNF_it)
			if (VNF_it->state == going_on)
			{
				VNF_to_switch_off_it = VNF_it;
				
				// apagamos la VNF candidata
				system.VNFs_going_on--;
				VNF_to_switch_off_it->state = off;

				// eliminamos el evento de su fallo o de su encendido
				for (auto event_it = event_list.begin(); event_it != event_list.end(); ++event_it)
					if (event_it->id == VNF_to_switch_off_it->VNF_id)
					{
						event_list.erase(event_it);
						break;
					}
				
				break;
			}
	}

	// Comprobamos si ha habido un cambio de periodo libre a periodo no libre
	if ((!empty_queue) && (system.user_queue.size() == 0))
		statistics.num_available_periods++;

	time_last_event = current_event.time;
}


void f_VNF_failure(Event_element& current_event, double& time_last_event, System& system, list<Event_element>& event_list,  Params& params, vector<int>& on_thresholds, Statistics& statistics)
{
	// we compute statistics
	if (statistics.accum_time_in_states[system.VNFs_on].size() < system.users + 1)
		statistics.accum_time_in_states[system.VNFs_on].resize(system.users + 1, 0.0);

	statistics.accum_time_in_states[system.VNFs_on][system.users] += current_event.time - time_last_event;

	bool empty_queue = (system.user_queue.size() == 0);

	if (system.user_queue.size() == 0)
		statistics.accum_available_time += current_event.time - time_last_event;
	else
		statistics.accum_non_available_time += current_event.time - time_last_event;

	// we recompute the pending_time of all users
	for (auto& VNF : system.VNFs)
		for (auto& user : VNF.users)
			user.pending_time -= current_event.time - time_last_event;

	// we look for a VNF to switch off
	VNF& failed_VNF = system.VNFs[current_event.id];

	failed_VNF.state = off;
	statistics.VNF_drop++; 
	system.VNFs_on--;

	while (failed_VNF.users.size() > 0)
	{
		bool moved_to_VNF = false;
		User& user_to_move = failed_VNF.users.front();

		// we try to put the user into another VNF
		for (auto& dest_VNF : system.VNFs)
			if ((dest_VNF.state == on) && (dest_VNF.users.size() < params.max_users_in_VNF))
			{
				statistics.user_migrations = dest_VNF.users.size(); 
				dest_VNF.users.push_back(user_to_move);
				failed_VNF.users.pop_front();
				moved_to_VNF = true;
				statistics.user_migrations++;   
				break;
			}

		if (!moved_to_VNF)
		{
			// we erase the departure in the event list associated to the user
			for (auto event_it = event_list.begin(); event_it != event_list.end(); ++event_it)
				if ((event_it->id == user_to_move.user_id) && (event_it->type == user_departure))
				{
					event_list.erase(event_it);
					break;
				}

			// we indicate that this user has been moved 
			user_to_move.moved_to_queue = true;

			//statistics.total_movements++;

			// we move the user to the queue
			system.user_queue.push_front(user_to_move);
			failed_VNF.users.pop_front();
		}
	}

	// we see if we have to switch on a VNF
	int aux_VNFs = system.VNFs_going_on + system.VNFs_on;

	if ((aux_VNFs == 0) || (system.users >= on_thresholds[aux_VNFs - 1]))
		for (auto& VNF : system.VNFs)
			if (VNF.state == off)
			{
				system.VNFs_going_on++;
				//statistics.VNF_activations++;
				VNF.state = going_on;

				Event_element VNF_activation_evnt;
				VNF_activation_evnt.type = VNF_activation;
				VNF_activation_evnt.time = current_event.time + params.VNF_boot_time;
				statistics.boot_time = statistics.boot_time + params.VNF_boot_time;      //added
				VNF_activation_evnt.id = VNF.VNF_id;
				event_list.push_back(VNF_activation_evnt);

				break;
			}

	// Comprobamos si ha habido un cambio de periodo libre a periodo no libre
	if ((empty_queue) && (system.user_queue.size() > 0))
		statistics.num_non_available_periods++;

	time_last_event = current_event.time;
}
