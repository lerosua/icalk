/*
 * =====================================================================================
 * 
 *       Filename:  SmileTree.h
 * 
 *    Description:  表情符号查找树
 * 
 *        Version:  1.0
 *        Created:  2007年10月02日 14时05分58秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  wind (xihe), xihels@gmail.com
 *        Company:  cyclone
 * 
 * =====================================================================================
 */

#ifndef _SMILE_TREE_H_
#define _SMILE_TREE_H_

#include "TreeItem.h"
#include <fstream>
#include <iostream>
#include <gtkmm.h>
#include <map>
#include "pixmaps.h"
using namespace std;
typedef std::map<Glib::ustring,Smile*>  SMILELIST;
class Smile {
	public:
		Smile(const string& filename):val(filename)
		{ 
		}
		string val;
		Gtk::Image* getSmileImage() {return getSmil(val.c_str());}
};

template < typename T >
class SmileTree {
	typedef TreeItem<T> ITEM; 
	public:
		SmileTree(const std::string& theme="default");
		~SmileTree();

		template <typename posT>
		Smile*   find_smile(posT iter, posT end, int& key_len);
		SMILELIST& getSmileList() { return smilelist; }
	private:
		void destroy_item(ITEM* item);
		void parser(const string& line);
		void insert(const Glib::ustring& action, Smile* smile);

		SmileTree(const SmileTree&);
		SmileTree& operator = (const SmileTree&);

		ITEM head;
		SMILELIST smilelist;
};

inline Smile* get_smile(const string& filename)
{
	return new Smile(filename);
}

template <typename T>
TreeItem < T >* insert_item(TreeItem < T >* head, TreeItem< T >* item)
{
	item->right= head;
	return item;
}

template < typename T >
TreeItem< T >* find_item(TreeItem<T>* head, T k)
{
	for (; head && *head != k; head = head->right)NULL;
	return head;
}


template <typename T>
template <typename posT>
Smile* SmileTree<T>::find_smile(posT iter, posT end, int& key_len)
{
	posT key = iter;
	posT tmp = key;
	ITEM* item = &head;
	for (key_len = 0; tmp != end ; ++tmp, key_len++) {
		item = find_item(item->left, *tmp);
		if (NULL == item)
			return NULL;
		else if (item->is_smile())
			break;
	}
	return *item;
}

template <typename T>
SmileTree<T>::SmileTree(const std::string& theme):
	head('\0')
{
    char buf[512];
    snprintf(buf, 512, DATA_DIR"/smiley/%s/theme",theme.c_str());
    std::ifstream file(buf);
	std::string line;
	while (getline(file, line)) {
		parser(line);
	}
}

template <typename T>
SmileTree<T>::~SmileTree()
{
	destroy_item(head.left);
	destroy_item(head.right);
}

template <typename T>
void SmileTree<T>::destroy_item(ITEM* item)
{
	if (NULL == item)
		return;
	destroy_item(item->left);
	destroy_item(item->right);
	delete item;
}

template <typename T>
void SmileTree<T>::parser(const string& line)
{
	size_t pos = line.find_first_of("\t ");
	if ( pos == string::npos)
		return;
	string filename = line.substr(0, pos);

	pos = line.find_first_not_of("\t ", pos);
	if (pos == string::npos)
		return;

	Glib::ustring action = line.substr(pos);

	insert(action, get_smile(filename));
	smilelist.insert(smilelist.end(),SMILELIST::value_type(action,get_smile(filename)));
}

template <typename T>
void SmileTree<T>::insert(const Glib::ustring& action, Smile* smile)
{
	Glib::ustring::const_iterator text = action.begin();
	ITEM* last = NULL;
	ITEM* first = &head;
	for(; text != action.end(); ++text) {
		last = find_item(first->left, *text);
		if (NULL == last){
			last = new ITEM(*text);
			last->right = first->left;
			first->left = last;
		} 
		first = last;
	}
	last->set_smile(smile);
}

#endif // _SMILE_TREE_H_

