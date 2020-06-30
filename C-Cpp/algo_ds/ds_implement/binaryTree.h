#include "binaryTreeNode.h"

template <typename Type> class BinaryTree{
 public:
  BinaryTree():proot(NULL){}
  BinaryTree(BinaryTree<Type>& copy);
	~BinaryTree(){
		proot->Destroy();
	}

	bool IsEmpty(){		//is empty?
		return proot==NULL;
	}

  const BinTreeNode<Type> *GetRoot() const;	//get root
	virtual bool Insert(const Type item);		//insert a new node
	virtual BinTreeNode<Type> *Find(const Type item) const;
  

 private:
   BinTreeNode<Type> *proot;
};

template<typename Type> const BinTreeNode<Type>* BinaryTree<Type>::GetRoot() const{
	return proot;
}

template<typename Type> bool BinaryTree<Type>::Insert(const Type item){
	BinTreeNode<Type> *pstart=proot,*newnode=new BinTreeNode<Type>(item);
	if(proot==NULL){
		proot=newnode;
		return 1;
	}
	while(1){
		if(item==pstart->m_data){
			std::cout<<"The item "<<item<<" is exist!"<<std::endl;
			return 0;
		}
		if(item<pstart->m_data){
			if(pstart->m_pleft==NULL){
				pstart->m_pleft=newnode;
				return 1;
			}
			pstart=pstart->m_pleft;	//if less than the node then insert to the left subtree
		}
		else{
			if(pstart->m_pright==NULL){
				pstart->m_pright=newnode;
				return 1;
			}
			pstart=pstart->m_pright;//if more than the node then insert to the right subtree
		}
	}
}
 
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::Find(const Type item) const{
	BinTreeNode<Type> *pstart=proot;
	while(pstart){
		if(item==pstart->m_data){
			return pstart;
		}
		if(item<pstart->m_data){
			pstart=pstart->m_pleft;	//if less than the node then find in the left subtree
		}
		else{
			pstart=pstart->m_pright;//if more than the node then find in the right subtree
		}
	}
	return NULL;
}

