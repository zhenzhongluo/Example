#pragma once
#include <algorithm>  
#include "ListNode.h"
using namespace std;

/*
	Reference:
	- https://zxi.mytechroad.com/blog/leetcode/leetcode-21-merge-two-sorted-lists/

	11/10/2019
	Microsoft 2019:
	本人亲自做过的OA题
	https://www.1point3acres.com/bbs/forum.php?mod=viewthread&tid=569170&extra=page%3D1%26filter%3Dsortid%26sortid%3D311%26searchoption%5B3046%5D%5Bvalue%5D%3D3%26searchoption%5B3046%5D%5Btype%5D%3Dradio%26sortid%3D311%26orderby%3Ddateline
*/
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

/*
	Microsoft 2020 2nd Round
	07/16/2020
	Merge LinkedList (Recursion + Iteration两种方法)
	https://www.1point3acres.com/bbs/forum.php?mod=viewthread&tid=652145&extra=page%3D1%26filter%3Dsortid%26sortid%3D311%26searchoption%5B3046%5D%5Bvalue%5D%3D3%26searchoption%5B3046%5D%5Btype%5D%3Dradio%26sortid%3D311%26orderby%3Ddateline
	
	Facebook Phone 2020
	01/22/2020
	2. 利口21; 返回第k小的数从m个升序排列的arrays
	https://www.1point3acres.com/bbs/forum.php?mod=viewthread&tid=584662&extra=page%3D1%26filter%3Dsortid%26sortid%3D311%26searchoption%5B3046%5D%5Bvalue%5D%3D2%26searchoption%5B3046%5D%5Btype%5D%3Dradio%26sortid%3D311%26orderby%3Ddateline

	Facebook 2019
	LC 23: Merge K sorted list, so this could be the previous version of the interview problem
	https://www.1point3acres.com/bbs/forum.php?mod=viewthread&tid=473617&extra=page%3D1%26filter%3Dsortid%26sortid%3D311%26searchoption%5B3046%5D%5Bvalue%5D%3D2%26searchoption%5B3046%5D%5Btype%5D%3Dradio%26sortid%3D311%26orderby%3Ddateline

	再写merge 2 sorted LL
	https://www.1point3acres.com/bbs/forum.php?mod=viewthread&tid=474118&extra=page%3D1%26filter%3Dsortid%26sortid%3D311%26searchoption%5B3046%5D%5Bvalue%5D%3D2%26searchoption%5B3046%5D%5Btype%5D%3Dradio%26sortid%3D311%26orderby%3Ddateline
*/