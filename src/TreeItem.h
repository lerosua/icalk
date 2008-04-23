/*
* =====================================================================================
*
*       Filename:  TreeItem.h
*
*    Description:  树结点结构
*
*        Version:  1.0
*        Created:  2007年10月02日 13时43分50秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:  cyclone
*
* =====================================================================================
*/
#ifndef _TREE_ITEM_H_
#define _TREE_ITEM_H

class Smile;

template < typename T >

struct TreeItem
{
        explicit TreeItem(T _val) :
                        val(_val)
        {
                left = 0;
                right = 0;
                smile = 0;
        }

        void set_smile(Smile* _smile)
        {
                smile = _smile;
        }

        bool is_smile() const
        {
                return smile != 0;
        }

        operator T () const
        {
                return val;
        }

        operator Smile* () const
        {
                return smile;
        }

        bool operator == (const TreeItem& other) const
        {
                return val == other.val && smile == other.smile;
        }

        TreeItem(const TreeItem&);
        TreeItem& operator = (const TreeItem&);

        TreeItem* left;
        TreeItem* right;
        Smile* smile;
        T val;
};

#endif // _TREE_ITEM_H_

