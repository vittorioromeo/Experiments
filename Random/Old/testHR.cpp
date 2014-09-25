#include <iostream>
#include <memory>
#include <cassert>

template<typename T> class BST
{
	private:
		std::unique_ptr<BST> left{nullptr}, right{nullptr};
		BST* parent{nullptr};
		T data;

	public:
		BST(T mData) : data{std::move(mData)} { }
		
		void insert(const T& mData)
		{
			if(mData < data)
			{
				if(left == nullptr)
				{
					left = std::move(std::unique_ptr<BST<T>>(new BST{mData}));
					left->parent = this;
				}
				else
				{
					left->insert(mData);
				}
			}
			else
			{
				if(right == nullptr)
				{
					right = std::move(std::unique_ptr<BST<T>>(new BST{mData}));
					right->parent = this;
				}
				else
				{
					right->insert(mData);
				}
			}
		}		

		void print(int mDepth = 0)
		{
			for(int i{0}; i < mDepth; ++i)
			{
				std::cout << " - \t";
			}

			std::cout << data;
			std::cout << std::endl;

			if(left) { std::cout << "L "; left->print(mDepth + 1); }
			if(right) { std::cout << "R "; right->print(mDepth +1); }

		}
};

int main()
{
	BST<int> bst(5);
	bst.insert(1);
	bst.insert(3);
	bst.insert(2);
	bst.insert(12);
	bst.insert(33);
	bst.insert(11);
bst.print();
	return 0;
} 
