#include "Nodes.h"
#include "Arc.h"
using namespace std;
Nodes::Nodes(string _name, const int _ref, const float _lattitude, const float _longitude) :name(_name), ref(_ref), lattitude(_lattitude), longitude(_longitude)
{	
}

 string Nodes::get_name(void)const
{
	
	return name;
}

 int Nodes::get_ref(void)const
{
	
	return ref;
}

 double Nodes::get_lattitude(void)const
{
	return lattitude;
}

 double Nodes::get_longitude(void)const
{
	return longitude;
}
 void Nodes::set_linked( Arc* const arcs)
{
	linked.push_back(arcs);
}
 const vector<Arc*>& Nodes::get_linked(void)const
{
  return linked;
}
Nodes::~Nodes()
{
}
