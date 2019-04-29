#pragma once
#include <string>
#include <vector>
#include "Arc.h"
using namespace std;

class Nodes
{
private:
    vector< Arc*>  linked;
    string name;
	const int ref;
	const double lattitude;
	const double longitude;
public:

	Nodes(string _name, int _ref, float _lattitude, float _longitude);
	~Nodes();
	Nodes(const Nodes&);
	Nodes& operator = (const Nodes&);
	void set_linked(Arc* arc);
	string get_name(void)const;
	int get_ref(void)const;
	double get_lattitude(void)const;
	double get_longitude(void)const;
	const vector<Arc*>& get_linked()const;

};

