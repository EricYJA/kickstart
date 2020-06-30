#include <iostream>

template<typename Type> class BinaryTree;

template<typename Type> class BinTreeNode{
 public:
  friend class BinaryTree<Type>;
  BinTreeNode():n_pleft(NULL), n_pright(NULL){}
  BinTreeNode(Type item,BinTreeNode<Type> *left=NULL,BinTreeNode<Type> *right=NULL)
		:n_data(item),n_pleft(left),n_pright(right){}

  Type GetData() const;
  BinTreeNode<Type>* GetLeft() const;
  BinTreeNode<Type>* GetRight() const;

  void SetData(const Type data);
  void SetLeft(const BinTreeNode<Type> *left);
  void SetRight(const BinTreeNode<Type> *right);

  BinTreeNode<Type> *Copy(const BinTreeNode<Type> *copy);	//copy the node
	void Destroy(){		//destroy the tree with the root of the node
		if(this!=NULL){
			this->n_pleft->Destroy();
			this->n_pright->Destroy();
			delete this;
		}
	}

 private:
  BinTreeNode<Type> *n_pleft, *n_pright;
  Type n_data;
};

template<typename Type> Type BinTreeNode<Type>::GetData() const{
  return this != NULL ? n_data : -1;
}

template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::GetLeft() const{
  return this != NULL ? n_pleft : NULL;
}

template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::GetRight() const{
  return this != NULL ? n_pright : NULL;
}

template<typename Type> void BinTreeNode<Type>::SetData(const Type data) {
  if(this != NULL){
    n_data = data;
  }
}

template<typename Type> void BinTreeNode<Type>::SetLeft(const BinTreeNode<Type> *left) {
  if(this != NULL){
    n_pleft = left;
  }
}

template<typename Type> void BinTreeNode<Type>::SetRight(const BinTreeNode<Type> *right) {
  if(this != NULL){
    n_pright = right;
  }
}

template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::Copy(const BinTreeNode<Type> *copy){
	if(copy==NULL){
		return NULL;
	}
 
	BinTreeNode<Type> *temp=new BinTreeNode<Type>(copy->m_data);
	temp->m_pleft=Copy(copy->m_pleft);
	temp->m_pright=Copy(copy->m_pright);
	return temp;
}
