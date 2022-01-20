#pragma once
#include <vector>
#include <memory>

template <typename T>
class BinaryTree
{
public:

	template <typename T>
	struct Node
	{
		Node(const T& data) : key{ std::make_shared<T>(data) } {}
		~Node() noexcept { delete left; delete right; }
		std::shared_ptr<T> key;
		Node* parent{};
		Node* left{};
		Node* right{};
	};

	BinaryTree() = default;
	BinaryTree(const BinaryTree&) = delete;
	BinaryTree& operator=(const BinaryTree&) = delete;
	~BinaryTree() noexcept { delete root_; }

	void push(const T&);
	Node<T>* node(const T&) noexcept;
	const Node<T>* node(const T&) const noexcept;
	Node<T>* root() const noexcept { return root_; };
	void remove(const T&);
	void cast(std::vector<T>&, Node<T>*) const;

private:
	Node<T>* root_{};
};

template <typename T>
void BinaryTree<T>::push(const T& data)
{
	auto current{ root_ };

	if (!current) {
		root_ = new Node<T>{ data };
	}
	else {
		while (current) {
			if (data == *current->key) {//������ �� �������� ����������
				break;
			}
			auto& temp{ (data < *current->key) ? current->left : current->right };
			if (temp) {
				current = temp;
			}
			else {
				temp = new Node<T>{ data };
				temp->parent = current;
				current = nullptr;
			}
		}
	}
}

template <typename T>
BinaryTree<T>::Node<T>* BinaryTree<T>::node(const T& data) noexcept
{
	auto current{ root_ };

	while (current) {
		if (data < *current->key) {
			current = current->left;
		}
		else if (data > *current->key) {
			current = current->right;
		}
		else {
			break;
		}
	}
	return current;
}

template <typename T>
const BinaryTree<T>::Node<T>* BinaryTree<T>::node(const T& data) const noexcept
{
	auto current{ root_ };

	while (current) {
		if (data < *current->key) {
			current = current->left;
		}
		else if (data > *current->key) {
			current = current->right;
		}
		else {
			break;
		}
	}
	return current;
}

template <typename T>
void BinaryTree<T>::remove(const T& data)
{
	auto rem{ node(data) };

	if (rem) {

		Node<T>** link{};//��������� �� ��������� � ������������ ���� �� rem
		if (rem->parent) {//���� �������� ����, �� link ��������� �� rem
			link = (*rem->parent->key < data) ? &(rem->parent->right) : &(rem->parent->left);
		}

		if (!rem->left and !rem->right) {//���� ��� ����, �� ��������� �� rem ��������
			(link ? *link : root_) = nullptr;
		}
		else if (!rem->left) {//���� ����� ����� ����, �� ��������� �� rem ����������� ��� ������ ���������
			(link ? *link : root_) = rem->right;
			rem->right = nullptr;
		}
		else if (!rem->right) {//���� ������ ����� ����, �� ��������� �� rem ����������� ��� ����� ���������
			(link ? *link : root_) = rem->left;
			rem->left = nullptr;
		}
		else {
			auto max{ rem->left };//��������� � ����� ����
			while (max->right) {//����, �������� ������, ����, � ������ ������ ��������� ����� ����
				max = max->right;//��� ������������ ������� ������ ���������
			}
			//������� ������ �� ��������� �� ������������ ���� �� ��������� ������������ �������
			auto& link{ (*max->key < *max->parent->key) ? max->parent->left : max->parent->right };
			//����������� ��������� �� ������������� ���� ����� ��������� ������������� ��������
			link = max->left;
			//�������� ��� ������������ ��������� �� �������� ����
			max->left = nullptr;
			//���������� � rem->key �������� �� ���������� ����
			rem->key = max->key;
			rem = max;
		}
		delete rem;//������ ����� �������
	}
}

template <typename T>
void BinaryTree<T>::cast(std::vector<T>& content, Node<T>* current) const
{
	if (current) {
		cast(content, current->left);
		content.push_back(*current->key);
		cast(content, current->right);
	}
}
