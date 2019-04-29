#pragma once
using namespace std;
#include <string>
#include<vector>
class Arc
{
private:
	const int arc1;
	const int arc2;
	string arcName;
public:
	Arc (const Arc&);
	Arc& operator = (const Arc&);
	Arc(int _arc1, int _arc2, string _arcName);
	~Arc();
	int get_arc1(void)const;
	int get_arc2(void)const;
	string get_arcName(void)const;

};

