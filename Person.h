#pragma once
#include <list>
#include <memory>
#include <iostream>

template <typename T>
struct Person
{
	explicit Person(T id) :id{ id } {}

	bool operator==(const Person& other) const noexcept { return id == other.id; }
	bool operator>(const Person& other) const noexcept { return id > other.id; }
	bool operator<(const Person& other) const noexcept { return id < other.id; }
	bool operator>=(const Person& other) const noexcept { return id >= other.id; }
	bool operator<=(const Person& other) const noexcept { return id <= other.id; }
	bool operator!=(const Person& other) const noexcept { return id != other.id; }

	T id;
	std::list<std::shared_ptr<Person>> contacts;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Person<T>& person)
{
	return os << person.id;
}