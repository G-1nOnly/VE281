#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>

/**
 * An abstract template base of the KDTree class
 */
template <typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template <typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType>
{
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");

protected:
    struct Node
    {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator
    {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment()
        {
            // TODO: implement this function
            if (!node)
                return;
            if (node->right)
            {
                node = node->right;
                while (node->left)
                    node = node->left;
            }
            else
            {
                while (node->parent)
                {
                    if (node->parent->left == node)
                    {
                        node = node->parent;
                        return;
                    }
                    node = node->parent;
                }
                if (!node->parent)
                    node = nullptr;
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement()
        {
            // TODO: implement this function
            if (!node)
            {
                node = tree->root;
                while (node->right)
                    node = node->right;
            }
            else if (node->left)
            {
                node = node->left;
                while (node->right)
                    node = node->right;
            }
            else
            {
                while (node->parent)
                {
                    if (node->parent->right == node)
                    {
                        node = node->parent;
                        return;
                    }
                    node = node->parent;
                }
                if (!node->parent)
                    throw std::range_error("Cannot be decremented!");
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++()
        {
            increment();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--()
        {
            decrement();
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const
        {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const
        {
            return node != that.node;
        }

        Data *operator->()
        {
            return &(node->data);
        }

        Data &operator*()
        {
            return node->data;
        }
    };

protected:                // DO NOT USE private HERE!
    Node *root = nullptr; // root of the tree
    size_t treeSize = 0;  // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template <size_t DIM>
    Node *find(const Key &key, Node *node)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node)
            return nullptr;
        if (key == node->key())
            return node;
        if (std::less<>()(std::get<DIM>(key), std::get<DIM>(node->key())))
            return find<DIM_NEXT>(key, node->left);
        else
            return find<DIM_NEXT>(key, node->right);
        // if (compareKey<DIM, std::less<>>(key, node->key()))
        //     return find<DIM_NEXT>(key, node->left);
        // else
        //     return find<DIM_NEXT>(key, node->right);
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template <size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node)
        {
            node = new Node(key, value, parent);
            treeSize++;
            return true;
        }
        if (node->key() == key)
        {
            node->value() = value;
            return false;
        }
        if (std::less<>()(std::get<DIM>(key), std::get<DIM>(node->key())))
            return insert<DIM_NEXT>(key, value, node->left, node);
        else
            return insert<DIM_NEXT>(key, value, node->right, node);
        // if (compareKey<DIM, std::less<>>(key, node->key()))
        //     return insert<DIM_NEXT>(key, value, node->left, node);
        // else
        //     return insert<DIM_NEXT>(key, value, node->right, node);
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template <size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare())
    {
        if (std::get<DIM>(a) != std::get<DIM>(b))
        {
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template <size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare())
    {
        if (!a)
            return b;
        if (!b)
            return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template <size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node)
            return nullptr;
        Node *min = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if (DIM != DIM_CMP)
        {
            Node *min_right = findMin<DIM_CMP, DIM_NEXT>(node->right);
            min = compareNode<DIM_CMP, std::less<>>(min, min_right);
        }
        return compareNode<DIM_CMP, std::less<>>(min, node);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template <size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node)
            return nullptr;
        Node *max = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if (DIM != DIM_CMP)
        {
            Node *max_left = findMax<DIM_CMP, DIM_NEXT>(node->left);
            max = compareNode<DIM_CMP, std::greater<>>(max, max_left);
        }
        return compareNode<DIM_CMP, std::greater<>>(max, node);
    }

    template <size_t DIM>
    Node *findMinDynamic(size_t dim)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize)
        {
            dim %= KeySize;
        }
        if (dim == DIM)
            return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template <size_t DIM>
    Node *findMaxDynamic(size_t dim)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize)
        {
            dim %= KeySize;
        }
        if (dim == DIM)
            return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template <size_t DIM>
    Node *erase(Node *node, const Key &key)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (!node)
            return nullptr;
        if (key == node->key())
        {
            if (node->right)
            {
                Node *Min = findMin<DIM, DIM_NEXT>(node->right);
                Node *temp = node;
                node = new Node(Min->key(), Min->value(), temp->parent);
                node->right = temp->right;
                node->right->parent = node;
                if (temp->left)
                {
                    node->left = temp->left;
                    node->left->parent = node;
                }
                if (temp == root)
                    root = node;
                delete temp;
                node->right = erase<DIM_NEXT>(node->right, Min->key());
            }
            else if (node->left)
            {
                Node *Max = findMax<DIM, DIM_NEXT>(node->left);
                Node *temp = node;
                node = new Node(Max->key(), Max->value(), temp->parent);
                node->left = temp->left;
                node->left->parent = node;
                if (temp->right)
                {
                    node->right = temp->right;
                    node->right->parent = node;
                }
                if (temp == root)
                    root = node;
                delete temp;
                node->left = erase<DIM_NEXT>(node->left, Max->key());
            }
            else
            {
                if (node == root)
                    root = nullptr;
                if (node->parent)
                {
                    if (node->parent->left && node->parent->left->key() == node->key())
                        node->parent->left = nullptr;
                    else
                        node->parent->right = nullptr;
                }
                delete node;
                treeSize--;
                return nullptr;
            }
        }
        else
        {
            if (std::less<>()(std::get<DIM>(key), std::get<DIM>(node->key())))
                node->left = erase<DIM_NEXT>(node->left, key);
            else
                node->right = erase<DIM_NEXT>(node->right, key);
            // if (compareKey<DIM, std::less<>>(key, node->key()))
            //     node->left = erase<DIM_NEXT>(node->left, key);
            // else
            //     node->right = erase<DIM_NEXT>(node->right, key);
        }
        return node;
    }

    template <size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize)
        {
            dim %= KeySize;
        }
        if (dim == DIM)
            return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // TODO: define your helper functions here if necessary
    void delete_kdtree(Node *node)
    {
        if (!node) return;
        delete_kdtree(node->left);
        delete_kdtree(node->right);
        delete node;
    }

    template <size_t DIM>
    void kdTree_helper(typename std::vector<std::pair<Key, Value>>::iterator begin, typename std::vector<std::pair<Key, Value>>::iterator end)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (begin == end) return;
        auto mid = begin + (end - begin - 1) / 2;
        std::nth_element(begin, mid, end, [](const Data &x, const Data &y){ return std::less<>()(std::get<DIM>(x.first), std::get<DIM>(y.first));});
        insert(mid->first, mid->second);
        kdTree_helper<DIM_NEXT>(begin, mid);
        kdTree_helper<DIM_NEXT>(mid + 1, end);
    }

    template <size_t DIM>
    void kdtree_that_helper(Node *&node, Node *copy, Node *parent)
    {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        node = new Node(copy->data.first, copy->data.second, parent);
        if (copy->left)
            kdtree_that_helper<DIM_NEXT>(node->left, copy->left, node);
        if (copy->right)
            kdtree_that_helper<DIM_NEXT>(node->right, copy->right, node);
    }

    static bool sort_condition(const Data &x, const Data &y)
    {
        return std::less<>()(std::get<0>(x.first), std::get<0>(y.first));
    }

    static bool unique_condition(Data x, Data y)
    {
        return (x.first == y.first);
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v)
    {
        // TODO: implement this function
        std::stable_sort<>(v.begin(), v.end(), sort_condition);
        auto it_rev = std::unique<>(v.rbegin(), v.rend(), unique_condition);
        auto it = it_rev.base();
        kdTree_helper<0>(it, v.end());
        treeSize = v.end() - it;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that)
    {
        // TODO: implement this function
        if (!that.root)
        {
            root = nullptr;
            treeSize = 0;
            return;
        }
        kdtree_that_helper<0>(root, that.root, nullptr);
        treeSize = that.treeSize;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that)
    {
        // TODO: implement this function
        delete_kdtree(root);
        if (!that.root)
        {
            root = nullptr;
            treeSize = 0;
        }
        else
        {
            kdtree_that_helper<0>(root, that.root, nullptr);
            treeSize = that.treeSize;
        }
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree()
    {
        // TODO: implement this function
        delete_kdtree(root);
    }

    Iterator begin()
    {
        if (!root)
            return end();
        auto node = root;
        while (node->left)
            node = node->left;
        return Iterator(this, node);
    }

    Iterator end()
    {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key)
    {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value)
    {
        insert<0>(key, value, root, nullptr);
    }

    template <size_t DIM>
    Iterator findMin()
    {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim)
    {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template <size_t DIM>
    Iterator findMax()
    {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim)
    {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key)
    {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it)
    {
        if (it == end())
            return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right)
        {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp)
        {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};
