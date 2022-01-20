#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "Graph.h"
#include "Person.h"
using namespace std;

int main()
{
	SocialNet<int> net;

	net.add_person(5);
	net.add_person(3);
	net.add_person(7);
	net.add_person(4);
	net.add_person(2);
	net.add_person(6);
	net.add_person(1);
	net.add_person(8);

	net.show_all();
	net.add_contact(3, 5);
	net.add_contact(1, 2);
	net.add_contact(2, 3);
	net.add_contact(3, 4);
	net.add_contact(4, 5);
	net.add_contact(5, 6);
	net.add_contact(6, 7);
	net.add_contact(7, 8);
	net.remove_contact(3, 5);
	net.remove_person(3);
	cout << endl;
	net.show_all();

	vector<pair<Person<int>, Person<int>>> pairs{};
	through_third_parties(pairs, net, 3);
	for (const auto& elem : pairs) {
		cout << elem.first << " - " << elem.second << endl;
	}

	return 0;
}