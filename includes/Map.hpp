#ifndef MAP_HPP
# define MAP_HPP

# include "Iterator.hpp"

namespace ft
{
	template < typename Key, typename T >
	class Mtree
	{
		public :
			std::pair< const Key, T >	*value;
			Mtree						*parent;
			Mtree						*left;
			Mtree						*right;

			Mtree() : value(NULL), parent(NULL), left(NULL), right(NULL) {};
	};

	template < class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key,T> > >
	class Map
	{
		public:

			// -------------------------------- TYPEDEFS --------------------------------
			typedef Key key_type;
			typedef T mapped_type;
			typedef typename std::pair<const key_type, mapped_type> value_type;
			typedef Compare key_compare;
			typedef Allocator allocator_type;
			typedef typename Allocator::reference reference;
			typedef typename Allocator::const_reference const_reference;
			typedef typename Allocator::pointer pointer;
			typedef typename Allocator::const_pointer const_pointer;
			typedef MapIterator<value_type, Mtree<Key, T> > iterator;
			typedef ConstMapIterator<value_type, Mtree<Key, T> > const_iterator;
			typedef ReverseIterator<iterator> reverse_iterator;
			typedef ReverseIterator<const_iterator> const_reverse_iterator;
			typedef typename std::ptrdiff_t difference_type;
			typedef size_t size_type;

			// --------------------------------  CLASSE  --------------------------------
			class value_compare : public std::binary_function<value_type, value_type, bool>
			{
				friend class Map;

				public:
					typedef bool result_type;
					typedef value_type first_argument_type;
					typedef value_type second_argument_type;

				protected:
					key_compare _comp;
					value_compare(key_compare comp) : _comp(comp) {}

				public:
					bool operator()(const value_type &__x, const value_type &__y) const { return _comp(__x.first, __y.first); }	
			};

		private:
			typedef Mtree<Key, T> node_type;

			node_type *_data;
			size_type _size;
			allocator_type _alloc;
			key_compare _comp;
			node_type *_end;
			node_type *_rend;

			// Recursive inserter
			void p_erase_node(node_type *ptr, const key_type &key);
			iterator p_insert_node(const value_type &value, node_type *ptr);
			// Recursive delete
			void p_deallocate_tree(node_type *node);

		public:

			// TODO Remove
			void debug_leaf(node_type *ptr);
			void debug_tree();
			static void grapher(void *item, int current_level, bool side);
			void put_tree();
			void call(Mtree<Key, T> *root, int current_level, bool side, void (*applyf)(void *item, int current_level, bool side));
			int btree_level_count(Mtree<Key, T> *root);
			void btree_apply_by_level(Mtree<Key, T> *root, void (*applyf)(void *item, int current_level, bool side));

			// -------------------------------- Member functions --------------------------------
			explicit Map(const Compare &comp = key_compare(), const Allocator &alloc = allocator_type());

			template <class InputIterator>
			Map(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type());
			Map(const Map &x);
			
			~Map();
			
			Map &operator=( const Map &other );

			// -------------------------------- Iterators --------------------------------
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;

			reverse_iterator rbegin();
			const_reverse_iterator rbegin() const;
			reverse_iterator rend();
			const_reverse_iterator rend() const;

			// -------------------------------- Capacity --------------------------------
			bool empty() const;
			size_type size() const;
			size_type max_size() const;

			// -------------------------------- Element acces --------------------------------
			T &operator[]( const Key &key );

			// -------------------------------- Modifiers --------------------------------
			std::pair<iterator,bool> insert( const value_type &value );

			template< class InputIt >
			void insert(InputIt first, InputIt last);
			void erase(iterator pos);
			void erase(iterator first, iterator last);
			size_type erase(const key_type &key);
			void clear();
			void swap(Map &other);

			// -------------------------------- Observers --------------------------------
			key_compare key_comp() const;
			value_compare value_comp() const;

			// -------------------------------- Operations --------------------------------
			iterator find(const Key &key);
			const_iterator find(const Key &key) const;
			size_type count(const Key &key) const;
			iterator lower_bound(const Key &key);
			const_iterator lower_bound(const Key &key) const;
			iterator upper_bound(const Key &key);
			const_iterator upper_bound(const Key &key) const;
			std::pair<iterator,iterator> equal_range(const Key &key);
			std::pair<const_iterator,const_iterator> equal_range(const Key &key) const;	

			// -------------------------------- Non-member functions --------------------------------
			bool operator==(const Map &rhs);
			bool operator!=(const Map &rhs);
			bool operator<(const Map &rhs);
			bool operator<=(const Map &rhs);
			bool operator>(const Map &rhs);
			bool operator>=(const Map &rhs);
	};

	// -------------------------------- Member functions --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	Map<Key, T, Compare, Allocator>::Map(const Compare &comp, const Allocator &alloc)
	: _data(NULL), _size(0), _alloc(alloc), _comp(comp), _end(new node_type()), _rend(new node_type())
	{
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	template <class InputIterator>
	Map<Key, T, Compare, Allocator>::Map(InputIterator first, InputIterator last, const key_compare &comp, const allocator_type &alloc)
	: _data(NULL), _size(0), _alloc(alloc), _comp(comp), _end(new node_type()), _rend(new node_type())
	{
		for (; first != last; first++)
			insert(*first);
	}

	template< class Key, class T, class Compare, class Allocator >
	Map<Key, T, Compare, Allocator>::Map(const Map &x)
	{
		(void)x;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::p_deallocate_tree(node_type *node)
	{
		if (node == NULL)
			return;
		p_deallocate_tree(node->left);
		p_deallocate_tree(node->right);
		_alloc.destroy(node->value);
		_alloc.deallocate(node->value, 1);
		delete node;
	}

	template< class Key, class T, class Compare, class Allocator >
	Map<Key, T, Compare, Allocator>::~Map()
	{
		p_deallocate_tree(_data);
	}
			
	template< class Key, class T, class Compare, class Allocator >
	Map<Key, T, Compare, Allocator> &Map<Key, T, Compare, Allocator>::operator=(const Map &other)
	{
		if (this == &other) return(*this);
		this->~Map();
		return *new(this) Map(other);
	}

	/*template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::realloc(size_type len)
	{
		pointer tmp = _alloc.allocate(len);

		for (size_type i = 0; i < (len < _size) ? len : _size; i++)
			tmp[i] = _data[i];
		_alloc.deallocate(_data);
		_data = tmp;
		_capacity = len;
	}*/

	/*template< class Key, class T, class Compare, class Allocator >
	Map<Key, T, Compare, Allocator>::Map() : _data(NULL), _size(0), _capacity(0), _alloc(Allocator()), _tree(NULL) {}*/

	// template< class Key, class T, class Compare, class Allocator >
	// template< class InputIterator >
	// Map<Key, T, Compare, Allocator>::Map(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
	// {
	// 	;
	// }

	// need shallow
	// template< class Key, class T, class Compare, class Allocator >
	// Map<Key, T, Compare, Allocator>::Map(const Map &x) : _data(x->_data), _size(x->_size), _capacity(x->_capacity), _alloc(x->_alloc), _tree(x->_tree)
	// {
	// 	(Value_compare(x->Value_compare->_comp));
	// }	

	// -------------------------------- Iterators --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::begin()
	{
		return (iterator(_rend->parent));
	}
	
	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::begin() const
	{
		return (iterator(_rend->parent));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::end()
	{
		return (iterator(_end));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::end() const
	{
		return (iterator(_end));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::reverse_iterator Map<Key, T, Compare, Allocator>::rbegin()
	{
		return (iterator(_end->parent));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_reverse_iterator Map<Key, T, Compare, Allocator>::rbegin() const
	{
		return (iterator(_end->parent));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::reverse_iterator Map<Key, T, Compare, Allocator>::rend()
	{
		return (iterator(_rend));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_reverse_iterator Map<Key, T, Compare, Allocator>::rend() const
	{
		return (iterator(_rend));
	}

	// -------------------------------- Capacity --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::empty() const { return (_size == 0); }

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::size() const { return (_size); }

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::max_size() const
	{
		return (std::numeric_limits<size_type>::max() / sizeof(1 /*TODO Replace here*/ ));
	}

	// -------------------------------- Element acces --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	T &Map<Key, T, Compare, Allocator>::operator[]( const Key &key )
	{
		// Mtree<Key, T> *temp;

		// temp = _data;
		// while (temp)
		// {
		// 	if (temp->value->first == key)
		// 		return (temp->value->second);
		// 	else if (key_comp()(temp->value->first, key))
		// 		temp = temp->left;
		// 	else
		// 		temp = temp->right;
		// }
		// insert(std::pair<Key, T>(key, T()));
		// operator[](key);
		// return (temp->value->second);
		(void)key;
		// TODO
	}

	// -------------------------------- Modifiers --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::debug_leaf(node_type *ptr)
	{
		if (ptr->left != NULL)
			debug_leaf(ptr->left);
		std::cout << "{ " << ptr->value->first << " : " << ptr->value->second << " }\n";
		if (ptr->right != NULL)
			debug_leaf(ptr->right);
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::debug_tree()
	{
		debug_leaf(_data);
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::p_insert_node(const value_type &value, node_type *ptr)
	{
		value_compare tmp_cmp = value_comp();
		if (_data == NULL)
		{
			_data = new node_type();
			_data->value = _alloc.allocate(1);
			_data->parent = NULL;
			_alloc.construct(_data->value, std::pair< Key, T >(value.first, value.second));
			_size++;
		}
		else if (value.first == ptr->value->first)
			; // Skip if existing key
		else if (tmp_cmp(value, *ptr->value))
		{
			if (ptr->left != NULL)
				p_insert_node(value, ptr->left);
			else
			{
				ptr->left = new node_type();
				ptr->left->value = _alloc.allocate(1);
				ptr->left->parent = ptr;
				_alloc.construct(ptr->left->value, std::pair< Key, T >(value.first, value.second));
				_size++;
				return (iterator(ptr->left));
			}
		}
		else
		{
			if (ptr->right != NULL)
				p_insert_node(value, ptr->right);
			else
			{
				ptr->right = new node_type();
				ptr->right->value = _alloc.allocate(1);
				ptr->right->parent = ptr;
				_alloc.construct(ptr->right->value, std::pair< Key, T >(value.first, value.second));
				_size++;
				return (iterator(ptr->right));
			}
		}
		return (iterator(_data));
	}
	
	template< class Key, class T, class Compare, class Allocator >
	std::pair<typename Map<Key, T, Compare, Allocator>::iterator, bool> Map<Key, T, Compare, Allocator>::insert(const value_type &value)
	{
		return (std::pair<iterator, bool>(p_insert_node(value, _data), true));
	}

	template< class Key, class T, class Compare, class Allocator >
	template< class InputIt >
	void Map<Key, T, Compare, Allocator>::insert(InputIt first, InputIt last)
	{
		for (; first != last; first++)
			insert(*first);
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::p_erase_node(node_type *ptr, const key_type &key)
	{
		if (ptr->value->first == key)
		{
			if (ptr->parent != NULL)
			{
				if (ptr->parent->left == ptr)
				{
					if (ptr->right)
						ptr->parent->left = ptr->right;
					else
						ptr->parent->left = ptr->left;
				}
				else if (ptr->parent->right == ptr)
				{
					if (ptr->right)
						ptr->parent->right = ptr->right;
					else
						ptr->parent->right = ptr->left;
				}
			}
			else
			{
				if (ptr->right != NULL)
					_data = ptr->right;
				else if (ptr->left != NULL)
					_data = ptr->left;
				else
					_data = NULL;
			}
			_alloc.destroy(ptr->value);
			_alloc.deallocate(ptr->value, 1);
			delete ptr;
			_size--;
		}
		else if (_comp(key, ptr->value->first))
			p_erase_node(ptr->left, key);
		else
			p_erase_node(ptr->right, key);
	}
	
	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::erase(iterator pos)
	{
		erase(pos->first);
	}
	
	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::erase(iterator first, iterator last)
	{
		for (; first != last; first++)
			erase(first);
	}
	
	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::erase(const key_type &key)
	{
		size_type tmp_size = _size;
		p_erase_node(_data, key);
		return (tmp_size - _size);
	}
	
	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::clear()
	{
		erase(begin(), end());
	}
	
	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::swap(Map &other)
	{
		(void)other;
		// TODO
	}

	// -------------------------------- Observers --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::key_compare Map<Key, T, Compare, Allocator>::key_comp() const
	{
		return (key_compare(_comp));
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::value_compare Map<Key, T, Compare, Allocator>::value_comp() const
	{
		return (value_compare(key_comp()));
	}

	// -------------------------------- Operations --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::find(const Key &key)
	{
		std::cout << begin()->first << "\n";
		std::cout << rbegin()->first << "\n";
		for (iterator itr = begin(); itr != end(); itr++)
			if (itr->first == key)
				return (itr);
		return (end());
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::find(const Key &key) const
	{
		for (iterator itr = begin(); itr != end(); itr++)
			if (itr->first == key)
				return (itr);
		return (end());
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::count(const Key &key) const
	{
		return (find(key) != end());
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::lower_bound(const Key &key)
	{
		// verif
		iterator temp = find(key);
		temp--;
		return (temp);
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::lower_bound(const Key &key) const
	{
		// verif
		iterator temp = find(key);
		temp--;
		return (temp);
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::upper_bound(const Key &key)
	{
		// verif
		iterator temp = find(key);
		temp++;
		return (temp);
	}

	template< class Key, class T, class Compare, class Allocator >
	typename Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::upper_bound(const Key &key) const
	{
		// verif
		iterator temp = find(key);
		temp++;
		return (temp);
	}

	template< class Key, class T, class Compare, class Allocator >
	std::pair<	typename Map<Key, T, Compare, Allocator>::iterator,
				typename Map<Key, T, Compare, Allocator>::iterator> Map<Key, T, Compare, Allocator>::equal_range(const Key &key)
	{
		return (std::pair<iterator, iterator>(find(key), ++find(key)));
	}

	template< class Key, class T, class Compare, class Allocator >
	std::pair<	typename Map<Key, T, Compare, Allocator>::const_iterator,
				typename Map<Key, T, Compare, Allocator>::const_iterator> Map<Key, T, Compare, Allocator>::equal_range(const Key &key) const
	{
		return (std::pair<iterator, iterator>(find(key), ++find(key)));
	}

	// -------------------------------- Non-member functions --------------------------------
	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::operator==(const Map &rhs)
	{
		(void)rhs;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::operator!=(const Map &rhs)
	{
		(void)rhs;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::operator<(const Map &rhs)
	{
		(void)rhs;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::operator<=(const Map &rhs)
	{
		(void)rhs;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::operator>(const Map &rhs)
	{
		(void)rhs;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	bool Map<Key, T, Compare, Allocator>::operator>=(const Map &rhs)
	{
		(void)rhs;
		// TODO
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::grapher(void *item, int current_level, bool side)
	{
		std::pair<Key, T> *pair_kv = static_cast<std::pair<Key, T> *>(item);
		if (current_level == 0)
		{
			std::cout << " " << pair_kv->first << "\n";
			return;
		}

		for (int i = 0; i < current_level - 1; i++)
			std::cout << " │  ";
		if (!side)
			std::cout << " └──";
		else
			std::cout << " ┌──";
		// for (int i = 0; i < current_level - 1; i++)
		// 	std::cout << "───";
		std::cout << " " << pair_kv->first << "\n";
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::put_tree()
	{
		btree_apply_by_level(_data, Map::grapher);
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::call(Mtree<Key, T> *root, int current_level, bool side, void (*applyf)(void *item, int current_level, bool side))
	{
		if (root->right)
			call(root->right, current_level + 1, 1, applyf);
		applyf(root->value, current_level, side);
		if (root->left)
			call(root->left, current_level + 1, 0, applyf);
	}

	template< class Key, class T, class Compare, class Allocator >
	int	Map<Key, T, Compare, Allocator>::btree_level_count(Mtree<Key, T> *root)
	{
		int	count;

		count = 0;
		if (!root)
			return (0);
		if (root->left)
			count = std::max(count, btree_level_count(root->left));
		if (root->right)
			count = std::max(count, btree_level_count(root->right));
		return (count + 1);
	}

	template< class Key, class T, class Compare, class Allocator >
	void Map<Key, T, Compare, Allocator>::btree_apply_by_level(Mtree<Key, T> *root, void (*applyf)(void *item, int current_level, bool side))
	{
		if (!root)
			return ;
		call(root, 0, 0, applyf);
	}
};

#endif