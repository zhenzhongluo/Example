#pragma once
#include <algorithm>  
#include "ListNode.h"
using namespace std;

namespace Merge_Two_Sorted_Lists_21
{
	/*
		Time:  T(n) = 1 + T(n-1)
		Space: T(n) = O(n)

		Idea:  Recursive Version
		Time:  O(n)
		Space: O(n)
		n = number of ListNodes.
	*/
	class Solution_Recursive_Version
	{
	public:
		ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
		{
			if (!l1 || !l2)
			{
				return (l1) ? (l1) : (l2);
			}

			if (l1->val > l2->val)
			{
				swap(l1, l2);
			}

			l1->next = mergeTwoLists(l1->next, l2);
			return l1;
		}
	};

	/*
		Idea:  Iterative Version
		Time:  O(n)
		Space: O(n)
		n = number of ListNodes.
	*/
	class Solution_Iterative_Version
	{
	public:
		ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
		{
			ListNode dummy(0);
			ListNode* p = &dummy;

			while (l1 && l2)
			{
				if (l1->val > l2->val)
				{
					swap(l1, l2);
				}

				p->next = l1;

				l1 = l1->next;

				p = p->next;
			}

			p->next = (l1) ? l1 : l2;

			return dummy.next;
		}
	};
	
	/*
		Time:  T(n) = 1 + T(n-1)
		Space: T(n) = O(n)
	*/
	class Solution_Recursive_Version_Another_Version
	{
	public:

		ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
		{
			if (!l1 || !l2)
			{
				return l1 ? l1 : l2;
			}

			if (l1->val < l2->val)
			{
				l1->next = mergeTwoLists(l1->next, l2);
				return l1;
			}

			else
			{
				l2->next = mergeTwoLists(l1, l2->next);
				return l2;
			}
		}
	};

	/*
		Time:  O(n)
		Space: O(n)
	*/
	class Solution_Iterative_Version_Another_Version
	{
	public:

		ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
		{
			ListNode dummy(0);
			ListNode* p = &dummy;

			while (l1 && l2)
			{
				if (l1->val <= l2->val)
				{
					p->next = l1;
					l1 = l1->next;
				}
				else
				{
					p->next = l2;
					l2 = l2->next;
				}

				p = p->next;
			}

			// add the remainders
			if (l1) p->next = l1;

			if (l2) p->next = l2;

			return dummy.next;
		}
	};
}
