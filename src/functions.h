#pragma once

// INCLUDES

#include "defines.h"

// class event_list;

using namespace std;

// FUNCTIONS
void read_inter_arrival(); 
void read_service_time(); 
double service_time(); 
double inter_arrival(); 
double exponential(double lambda);
double pareto(double lambda); 
Event_element f_timing(list<Event_element>&);
void f_user_arrival(double, double&, System&, list<Event_element>&,  Params&, vector<int>&, Statistics&);
void f_user_departure(Event_element&, double&, System&, list<Event_element>&,  Params&, vector<int>&, vector<int>&, Statistics&);
void f_VNF_activation(Event_element&, double&, System&, list<Event_element>&,  Params&, Statistics&);
void f_VNF_failure(Event_element&, double&, System&, list<Event_element>&,  Params&, vector<int>&, Statistics&);

