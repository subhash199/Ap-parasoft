#include "Navigation.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Nodes.h"
#include "Arc.h"
#include<vector>
#include <string>


using namespace std;

// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);
vector<Nodes*> nodeVec;
vector<Arc*> arcVec;
vector<string>transportMethod;

bool  Navigation::Maxdistact()
{
	double lattitude;
	double longitude;

	double northing;
	double easting;

	double x1;
	double y1;

	double x2;
	double y2;

	string maxName1;
	string maxName2;
	double maxDistance = 0;

	double xCordinates;
	double yCordinates;
	double tempDistance;



	for (int i = 0; i < nodeVec.size(); i++)
	{
		lattitude = nodeVec[i]->get_lattitude();
		longitude = nodeVec[i]->get_longitude();
		LLtoUTM(lattitude, longitude, northing, easting);
		x1 = easting;
		y1 = northing;

		for (int j = i; j < nodeVec.size(); j++)
		{

			lattitude = nodeVec[j]->get_lattitude();
			longitude = nodeVec[j]->get_longitude();
			LLtoUTM(lattitude, longitude, northing, easting);
			x2 = easting;
			y2 = northing;


			xCordinates = (x2 - x1);
			yCordinates = (y2 - y1);
			tempDistance = (xCordinates* xCordinates) + (yCordinates*yCordinates);
			tempDistance = sqrt(tempDistance);
			if ((tempDistance / 1000) > maxDistance)
			{
				maxDistance = tempDistance/1000;
				maxName1 = nodeVec[i]->get_name();
				maxName2 = nodeVec[j]->get_name();
			}
		}
	}
	_outFile << "MaxDist" << "\r\n";
	_outFile << maxName1 << "," << maxName2 << "," << maxDistance << endl << "\r\n";
	return true;

}
bool Navigation::MaxLinks()
{

	bool ref1Sucess = false;
	bool ref2Sucess = false;

	int ref1;
	int ref2;

	double lattitude;
	double longitude;

	double northing;
	double easting;

	double x1;
	double y1;

	double x2;
	double y2;

	int link1;
	int link2;
	double maxDistance = 0;

	double xCordinates;
	double yCordinates;
	double tempDistance;

	for (int i = 0; i < arcVec.size(); i++)
	{

		ref1 = arcVec[i]->get_arc1();
		ref2 = arcVec[i]->get_arc2();
		for (int j = 0; j < nodeVec.size(); j++)
		{

			const int tempRef = nodeVec[j]->get_ref();

			if (tempRef == ref1)
			{
				ref1Sucess = true;
				lattitude = nodeVec[j]->get_lattitude();
				longitude = nodeVec[j]->get_longitude();
				LLtoUTM(lattitude, longitude, northing, easting);
				x1 = easting;
				y1 = northing;
			}
			if (tempRef == ref2)
			{
				ref2Sucess = true;
				lattitude = nodeVec[j]->get_lattitude();
				longitude = nodeVec[j]->get_longitude();
				LLtoUTM(lattitude, longitude, northing, easting);
				x2 = easting;
				y2 = northing;
			}
			if (ref1Sucess == true && ref2Sucess == true)
			{
				ref1Sucess = false;
				ref2Sucess = false;
				xCordinates = (x2 - x1);
				yCordinates = (y2 - y1);
				tempDistance = (xCordinates* xCordinates) + (yCordinates*yCordinates);
				tempDistance = sqrt(tempDistance);
				if ((tempDistance / 1000) > maxDistance)
				{
					maxDistance = tempDistance / 1000;

					link1 = arcVec[i]->get_arc1();
					link2 = arcVec[i]->get_arc2();

				}
			}
		}
	}
	_outFile << "MaxLink" << "\r\n";
	_outFile << link2 << "," << link1 << "," << maxDistance << endl << "\r\n";
	return true;
}


Navigation::Navigation() : _outFile("Output.txt")
{

}

Navigation::~Navigation()
{
}

bool Navigation::ProcessCommand(const string& commandString)
{

	istringstream inString(commandString);
	string command;
	inString >> command;
	if (command == "MaxDist")
	{
		Maxdistact();
	}
	else if (command == "MaxLink")
	{
		MaxLinks();
	}
	else if (command == "FindDist")
	{
		int refs;
		vector <int>location;
		while (inString >> refs)
		{
			location.push_back(refs);
		}
		bool loop1 = false;
		bool loop2 = false;

		double latittude;
		double longititude;

		double easting;
		double northing;

		double x1;
		double x2;

		double y1;
		double y2;

		double xCordinates;
		double yCordinates;

		double tempDistance;

		double distance = 0;

		string locationName1;
		string locationName2;
		for (int j = 0; j < nodeVec.size(); j++)
		{
			if (nodeVec[j]->get_ref() == location[0])
			{
				loop1 = true;
				locationName1 = nodeVec[j]->get_name();
				latittude = nodeVec[j]->get_lattitude();
				longititude = nodeVec[j]->get_longitude();
				LLtoUTM(latittude, longititude, easting, northing);
				x1 = easting;
				y1 = northing;
			}
			if (nodeVec[j]->get_ref() == location[1])
			{
				loop2 = true;
				locationName2 = nodeVec[j]->get_name();
				latittude = nodeVec[j]->get_lattitude();
				longititude = nodeVec[j]->get_longitude();
				LLtoUTM(latittude, longititude, easting, northing);
				x2 = easting;
				y2 = northing;
			}
			if (loop1 == true && loop2 == true)
			{
				xCordinates = (x2 - x1);
				yCordinates = (y2 - y1);
				tempDistance = (xCordinates* xCordinates) + (yCordinates*yCordinates);
				tempDistance = sqrt(tempDistance);
				if ((tempDistance / 1000) > distance)
				{
					distance = tempDistance / 1000;
				}

			}

		}
		_outFile << "FindDist " << location[0] << " " << location[1] << "\r\n";
		_outFile << locationName1 << "," << locationName2 << "," << distance << endl << "\r\n";
		return true;
	}
	else if (command == "FindNeighbour")
	{
		vector<int>neighbourNodes;
		int findNodes;
		inString >> findNodes;
		for (int i = 0; i < arcVec.size(); i++)
		{
			if (arcVec[i]->get_arc1() == findNodes)
			{
				neighbourNodes.push_back(arcVec[i]->get_arc2());
			}
			else if (arcVec[i]->get_arc2() == findNodes)
			{
				neighbourNodes.push_back(arcVec[i]->get_arc1());
			}
		}
		_outFile << "FindNeighbour" << " " << findNodes << "\r\n";
		for (int i = 0; i < neighbourNodes.size(); i++)
		{
			_outFile << neighbourNodes[i] << "\r\n";
		}
		_outFile << "\r\n";
		return true;
	}
	else if (command == "Check")
	{
		string mode;
		string readIn;
		string tempString;
		int tempNum = 0;
		int i = 0;
		int x = 0;

		bool breakout = false;
		vector<int>nodesRef;
		while (inString.peek() > -1)
		{
			inString >> tempString;
			readIn = readIn + tempString + " ";
		}
		stringstream ssin(readIn);
		while (ssin.good() && i < readIn.size())
		{
			i++;
			if (i == 2)
			{
				ssin >> mode;
			}
			else if (i > 2)
			{
				ssin >> tempNum;
				nodesRef.push_back(tempNum);

			}
		}

		nodesRef.erase(nodesRef.end() - 1);
		for (int f = 0; f < nodesRef.size(); f++)
		{
			if (f == 0)
			{
				_outFile << "Check " << mode << " ";
			}

			_outFile << nodesRef[f] << " ";
		}
		_outFile << endl;
		for (int i = 0; i < arcVec.size(); i++)
		{
			if (x == nodesRef.size() - 1)
			{
				breakout = true;
			}
			if (breakout == false)
			{
				if (mode == "Rail" || mode == "Ship")
				{
					if (arcVec[i]->get_arc1() == nodesRef[x] && arcVec[i]->get_arc2() == nodesRef[x + 1])
					{
						x++;
						if (arcVec[i]->get_arcName() == mode)
						{
							_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "PASS" << endl;
						}
						else
						{
							_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "FAIL" << endl << "\r\n";
							break;
						}
						i = 0;
					}
					else if (arcVec[i]->get_arc2() == nodesRef[x] && arcVec[i]->get_arc1() == nodesRef[x + 1])
					{
						x++;
						if (arcVec[i]->get_arcName() == mode)
						{
							_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "PASS" << endl;
						}
						else
						{
							_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "FAIL" << endl << "\r\n";

							break;
						}
						i = 0;
					}
				}
				else if (mode == "Foot")
				{
					if (arcVec[i]->get_arc1() == nodesRef[x] && arcVec[i]->get_arc2() == nodesRef[x + 1])
					{
						x++;

						_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "PASS" << endl;


					}
					else if (arcVec[i]->get_arc2() == nodesRef[x] && arcVec[i]->get_arc1() == nodesRef[x + 1])
					{
						x++;
						_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "PASS" << endl;
						i = 0;
					}
				}
				else if (mode == "Bike")
				{
					if (arcVec[i]->get_arc1() == nodesRef[x] && arcVec[i]->get_arc2() == nodesRef[x + 1])
					{
						x++;
						if (arcVec[i]->get_arcName() != "Foot")
						{
							_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "PASS" << endl;
						}
						else
						{
							_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "FAIL" << endl << "\r\n";
							break;
						}
						i = 0;
					}
					else if (arcVec[i]->get_arc2() == nodesRef[x] && arcVec[i]->get_arc1() == nodesRef[x + 1])
					{
						x++;
						if (arcVec[i]->get_arcName() != "Foot")
						{
							_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "PASS" << endl;
						}
						else
						{
							_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "FAIL" << endl << "\r\n";

							break;
						}
						i = 0;
					}
				}
				else if (mode == "Bus")
				{
					if (arcVec[i]->get_arc1() == nodesRef[x] && arcVec[i]->get_arc2() == nodesRef[x + 1])
					{
						x++;
						if (arcVec[i]->get_arcName() != "Bike" || arcVec[i]->get_arcName() != "Foot")
						{
							_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "PASS" << endl;
						}
						else
						{
							_outFile << arcVec[i]->get_arc1() << "," << arcVec[i]->get_arc2() << "," << "FAIL" << endl << "\r\n";
							break;
						}
						i = 0;
					}
					else if (arcVec[i]->get_arc2() == nodesRef[x] && arcVec[i]->get_arc1() == nodesRef[x + 1])
					{
						x++;
						if (arcVec[i]->get_arcName() != "Bike" || arcVec[i]->get_arcName() != "Foot")
						{
							_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "PASS" << endl;
						}
						else
						{
							_outFile << arcVec[i]->get_arc2() << "," << arcVec[i]->get_arc1() << "," << "FAIL" << endl << "\r\n";

							break;
						}
						i = 0;
					}
				}
			}

		}
		
		return true;
	}
	else if (command == "FindRoute"||command=="FindShortestRoute")
	{

		vector<string> TransportMethod;
		string transport; inString >> transport;

		vector<int>startEndRefs;
		int refs = 0;
		while (inString >> refs)
		{
			startEndRefs.push_back(refs);
		}
		if (command == "FindShortestRoute")
		{
			_outFile << "FindShortestRoute " << transport << " " << startEndRefs[0] << " " << startEndRefs[1] << "\r\n";
		}
		else if (command == "FindRoute")
		{
			_outFile << "FindRoute " << transport << " " << startEndRefs[0] << " " << startEndRefs[1] << "\r\n";
		}
		




		vector<int>connectedNodes;
		vector<int>findRoute;
		vector<int>visitedNodes;

		bool breakout = false;
		bool breakout1 = false;
		bool foundEndRef = false;

		if (transport == "Rail")
		{
			TransportMethod.push_back("Rail");
		}
		else if (transport == "Ship")
		{
			TransportMethod.push_back("Ship");
		}
		else if (transport == "Bus")
		{
			TransportMethod.push_back("Bus");
			TransportMethod.push_back("Ship");
		}
		else if (transport == "Bike")
		{
			TransportMethod.push_back("Bike");
			TransportMethod.push_back("Bus");
			TransportMethod.push_back("Rail");
			TransportMethod.push_back("Car");
			TransportMethod.push_back("Ship");
		}
		else if (transport == "Foot")
		{
			TransportMethod.push_back("Bike");
			TransportMethod.push_back("Bus");
			TransportMethod.push_back("Rail");
			TransportMethod.push_back("Foot");
			TransportMethod.push_back("Car");
			TransportMethod.push_back("Ship");


		}
		else if (transport == "Car")
		{
			TransportMethod.push_back("Car");
			TransportMethod.push_back("Bus");
			TransportMethod.push_back("Ship");
		}

		int temp = startEndRefs[0];
		connectedNodes.push_back(startEndRefs[0]);
		while (foundEndRef == false)
		{
			for (int i = 0; i < nodeVec.size(); i++)
			{
				if (nodeVec[i]->get_ref() == temp)
				{

					for (const Arc* const a : nodeVec[i]->get_linked())
					{
						if (a->get_arc1() == temp && count(TransportMethod.begin(), TransportMethod.end(), a->get_arcName()))
						{
							if (!count(connectedNodes.begin(), connectedNodes.end(), a->get_arc2()))
							{
								if (a->get_arc2() == startEndRefs[1])
								{
									connectedNodes.push_back(a->get_arc2());
									foundEndRef = true;
									break;
								}
								else
								{
									connectedNodes.push_back(a->get_arc2());
								}

							}

						}
						else if (a->get_arc2() == temp && count(TransportMethod.begin(), TransportMethod.end(), a->get_arcName()))
						{
							if (!count(connectedNodes.begin(), connectedNodes.end(), a->get_arc1()))
							{
								if (a->get_arc1() == startEndRefs[1])
								{
									connectedNodes.push_back(a->get_arc1());
									foundEndRef = true;
									break;
								}
								else
								{
									connectedNodes.push_back(a->get_arc1());
								}
							}
						}
					}
					if (foundEndRef == true)
					{
						break;
					}
					else
					{
						visitedNodes.push_back(temp);
						for (int x = 0; x < connectedNodes.size(); x++)
						{
							if (!count(visitedNodes.begin(), visitedNodes.end(), connectedNodes[x]))
							{
								temp = connectedNodes[x];
								i = 0;
								break;
							}
						}
					}

				}
			}
		}

		temp = startEndRefs[0];
		breakout = false;
		bool added = false;

		while (breakout == false)
		{

			for (int x = 0; x < arcVec.size(); x++)
			{
				added = false;
				if (temp == arcVec[x]->get_arc1() && count(connectedNodes.begin(), connectedNodes.end(), arcVec[x]->get_arc2()))
				{
					if (!count(findRoute.begin(), findRoute.end(), temp))
					{
						findRoute.push_back(temp);
						findRoute.push_back(arcVec[x]->get_arc2());
						temp = arcVec[x]->get_arc2();
						added = true;
						x = 0;
						break;
					}
					else if (!count(findRoute.begin(), findRoute.end(), arcVec[x]->get_arc2()))
					{
						findRoute.push_back(arcVec[x]->get_arc2());
						temp = arcVec[x]->get_arc2();
						x = 0;
						added = true;
						break;

					}

				}
				else if (temp == arcVec[x]->get_arc2() && count(connectedNodes.begin(), connectedNodes.end(), arcVec[x]->get_arc1()))
				{
					if (!count(findRoute.begin(), findRoute.end(), temp))
					{
						findRoute.push_back(temp);
						findRoute.push_back(arcVec[x]->get_arc1());
						temp = arcVec[x]->get_arc1();
						added = true;
						x = 0;
						break;
					}
					else if (!count(findRoute.begin(), findRoute.end(), arcVec[x]->get_arc1()))
					{
						findRoute.push_back(arcVec[x]->get_arc1());
						temp = arcVec[x]->get_arc1();
						added = true;
						x = 0;
						break;

					}

				}
			}

			if (count(findRoute.begin(), findRoute.end(), startEndRefs[1]))
			{
				
				breakout = true;
				break;
			}
			else if (added == false)
			{
				for (int i = 0; i < connectedNodes.size(); i++)
				{
					if (connectedNodes[i] == findRoute.back())
					{
						connectedNodes.erase(connectedNodes.begin() + i);
						temp = startEndRefs[0];
						findRoute.clear();
						break;
					}

				}
			}

		}
		vector<int>reversed;
		reversed = findRoute;

		for (int i = 0; i < findRoute.size(); i++)
		{
			for (int z = 0; z < reversed.size(); z++)
			{
				for (int j = 0; j < arcVec.size(); j++)
				{
					if (findRoute[i] == arcVec[j]->get_arc1() && reversed[z] == arcVec[j]->get_arc2())
					{
						if (i + 1 == z || z + 1 == i)
						{							
							break;
						}
						else
						{
							findRoute.erase(findRoute.begin() + i + 1, findRoute.begin() + z);
							reversed.erase(reversed.begin() + i + 1, reversed.begin() + z);
							i = 0;
							z = 0;
							j = 0;
							break;
						}
						break;
					}
					else if (findRoute[i] == arcVec[j]->get_arc2() && reversed[z] == arcVec[j]->get_arc1())
					{
						if (i + 1 == z || z + 1 == i)
						{							
							break;
						}
						else
						{
							findRoute.erase(findRoute.begin() + i + 1, findRoute.begin() + z);
							reversed.erase(reversed.begin() + i + 1, reversed.begin() + z);
							i = 0;
							z = 0;
							j = 0;
							break;
						}
						break;
					}
				}
			}
		}


		for (int i = 0; i < findRoute.size(); i++)
		{
			if (findRoute[i] == startEndRefs[0])
			{
				breakout = true;
			}
			else if (findRoute[i] == startEndRefs[1])
			{
				breakout1 = true;
			}
		}

		if (breakout == true && breakout1 == true)
		{
			for (int i = 0; i < findRoute.size(); i++)
			{
				_outFile << findRoute[i] << "\r\n";
			}
			_outFile << "\r\n";
		}

		else
		{
			_outFile << "FAIL" << "\r\n";
		}
		return true;
	}
	
}


 bool Navigation:: BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)const
{

	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);
	if (finPlaces.fail() || finLinks.fail())
		return false;
	int count = 0;
	while (finPlaces.good())
	{
		count++;
		char line[255];
		string name;
		int ref;
		float lattitude;
		float longitude;

		finPlaces.getline(line, 255, ',');
		name = string(line);

		finPlaces.getline(line, 255, ',');
		istringstream sin(line);
		sin >> ref;

		finPlaces.getline(line, 255, ',');
		istringstream sin1(line);
		sin1 >> lattitude;

		finPlaces.getline(line, 255);
		istringstream sin2(line);
		sin2 >> longitude;
	    Nodes *const node = new Nodes(name, ref, lattitude, longitude);
		nodeVec.push_back(node);
	}
	count = 0;
	while (finLinks.good())
	{
		count++;
		char line[255];
		int pointA;
		int pointB;
		string arc;

		finLinks.getline(line, 255, ',');
		istringstream sin(line);
		sin >> pointA;

		finLinks.getline(line, 255, ',');
		istringstream sin1(line);
		sin1 >> pointB;

		finLinks.getline(line, 255);
		arc = string(line);

	    Arc *const arcs = new Arc(pointA, pointB, arc);
		arcVec.push_back(arcs);

	}
	count = 0;
	for (int i = 0; i < arcVec.size(); i++)
	{
		for (int j = 0; j < nodeVec.size(); j++)
		{
			count++;
			if (arcVec[i]->get_arc1() == nodeVec[j]->get_ref())
			{
				nodeVec[j]->set_linked(arcVec[i]);
			}
			else if (arcVec[i]->get_arc2() == nodeVec[j]->get_ref())
			{
				nodeVec[j]->set_linked(arcVec[i]);
			}
		}
	}
	return true;
}

