#pragma once
#include <stdexcept>
#include <vector>
#include <utility>
#include <algorithm>
#include <memory>
#include "Person.h"
#include "BinaryTree.h"

template <typename ID>
class SocialNet
{
	template <typename ID>
	friend void through_third_parties(
		std::vector<std::pair<Person<ID>, Person<ID>>>&, const SocialNet<ID>&, size_t);
	template <typename ID>
	friend void through_third_parties(const std::vector<Person<ID>>&,
		typename std::vector<Person<ID>>::const_iterator,
		typename std::vector<Person<ID>>::const_iterator,
		BinaryTree<std::pair<Person<ID>, Person<ID>>>&,
		size_t);
public:
	void add_person(const ID&);
	void add_contact(const ID&, const ID&);
	void remove_person(const ID&);
	void remove_contact(const ID&, const ID&);
	bool is_person_exists(const ID&) const;
	bool is_contact_exists(const ID&, const ID&) const;
	void show_contacts(const ID&) const;
	void show_all() const;
private:
	BinaryTree<Person<ID>> net_;
};

template <typename ID>
void SocialNet<ID>::add_person(const ID& person)
{
	net_.push(Person<ID>{ person });
}

template <typename ID>
bool SocialNet<ID>::is_person_exists(const ID& person) const
{
	return net_.node(Person<ID>{ person });
}

template <typename ID>
bool SocialNet<ID>::is_contact_exists(const ID& first, const ID& second) const
{
	const auto f{ net_.node(Person<ID>{ first }) };
	if (f) {
		auto& contacts{ f->key->contacts };
		auto iter{ std::find_if(contacts.cbegin(), contacts.cend(),
			[&second](std::shared_ptr<Person<ID>> elem) { return elem->id == second; }) };
		return iter == contacts.cend() ? false : true;
	}
	return false;
}

template <typename ID>
void SocialNet<ID>::add_contact(const ID& first, const ID& second)
{
	auto f{ net_.node(Person<ID>{ first }) };
	if (!f) {
		throw std::runtime_error("SocialNet::add_link(): first person is unknown!\n");
	}

	auto s{ net_.node(Person<ID>{ second }) };
	if (!s) {
		throw std::runtime_error("SocialNet::add_link(): second person is unknown!\n");
	}

	auto& contacts{ f->key->contacts };
	auto iter{ std::find_if(contacts.cbegin(), contacts.cend(),
		[&second](std::shared_ptr<Person<ID>> elem) { return elem->id == second; }) };

	if (iter == contacts.cend()) {
		f->key->contacts.push_back(s->key);
		s->key->contacts.push_back(f->key);
	}
}

template <typename ID>
void SocialNet<ID>::show_contacts(const ID& person) const
{
	auto psn { net_.node(Person<ID>{ person }) };

	if (psn) {
		for (const auto& elem : psn->key->contacts) {
			std::cout << *elem << '\n';
		}
	}
}

template <typename ID>
void SocialNet<ID>::remove_person(const ID& id)
{
	auto node{ net_.node(Person<ID>{ id }) };

	if (node) {
		for (auto& contact : node->key->contacts) {
			contact->contacts.remove_if([id](std::shared_ptr<Person<ID>> person) { return person->id == id; });
		}
		net_.remove(Person<ID>{ id });
	}
}

template <typename ID>
void SocialNet<ID>::remove_contact(const ID& first, const ID& second)
{
	auto f{ net_.node(Person<ID>{ first }) };
	if (!f) {
		return;
	}

	auto s{ net_.node(Person<ID>{ second }) };
	if (!s) {
		return;
	}

	f->key->contacts.remove_if(
		[second](std::shared_ptr<Person<ID>> person) { return person->id == second; });
	s->key->contacts.remove_if(
		[first](std::shared_ptr<Person<ID>> person) { return person->id == first; });
}

template <typename ID>
void SocialNet<ID>::show_all() const
{
	std::vector<Person<ID>> persons{};
	net_.cast(persons, net_.root());
	for (const auto& person : persons) {
		std::cout << person << '\n';
	}
}

template <typename ID>
void through_third_parties(
	std::vector<std::pair<Person<ID>, Person<ID>>>& pairs,
	const SocialNet<ID>& net,
	size_t depth)
{
	BinaryTree<std::pair<Person<ID>, Person<ID>>> tree{};
	std::vector<Person<ID>> persons{};
	net.net_.cast(persons, net.net_.root());

	for (typename std::vector<Person<ID>>::const_iterator first{ persons.cbegin() };
		first != persons.cend(); ++first) {
		through_third_parties(persons, first, first, tree, depth);
	}

	tree.cast(pairs, tree.root());
}

template <typename ID>
void through_third_parties(const std::vector<Person<ID>>& all,
	typename std::vector<Person<ID>>::const_iterator first,
	typename std::vector<Person<ID>>::const_iterator second,
	BinaryTree<std::pair<Person<ID>, Person<ID>>>& tree,
	size_t depth)
{
	if (depth) {
		for (const auto& contact : (*second).contacts) {
			auto third{ std::find(all.cbegin(), all.cend(), *contact) };
			if (*first != *third) {
				tree.push(std::pair<Person<ID>, Person<ID>>{ std::min(*first, *third), std::max(*first, *third) });
			}
			through_third_parties(all, first, third, tree, depth - 1);
		}
	}
}