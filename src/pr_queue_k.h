//----------------------------------------------------------------------
// File:			pr_queue_k.h
// Programmer:		Sunil Arya and David Mount
// Description:		Include file for priority queue with k items.
// Last modified:	01/04/05 (Version 1.0)
//----------------------------------------------------------------------
// Copyright (c) 1997-2005 University of Maryland and Sunil Arya and
// David Mount.  All Rights Reserved.
// 
// This software and related documentation is part of the Approximate
// Nearest Neighbor Library (ANN).  This software is provided under
// the provisions of the Lesser GNU Public License (LGPL).  See the
// file ../ReadMe.txt for further information.
// 
// The University of Maryland (U.M.) and the authors make no
// representations about the suitability or fitness of this software for
// any purpose.  It is provided "as is" without express or implied
// warranty.
//----------------------------------------------------------------------
// History:
//	Revision 0.1  03/04/98
//		Initial release
//----------------------------------------------------------------------

#ifndef PR_QUEUE_K_H
#define PR_QUEUE_K_H

#include <ANN/ANNx.h>					// all ANN includes
#include <ANN/ANNperf.h>				// performance evaluation

#include <math.h>
#include <R.h>
#include <vector>
#include <random>
#include <algorithm>

//----------------------------------------------------------------------
//	Basic types
//----------------------------------------------------------------------
typedef ANNdist			PQKkey;			// key field is distance
typedef int				PQKinfo;		// info field is int

//----------------------------------------------------------------------
//	Constants
//		The NULL key value is used to initialize the priority queue, and
//		so it should be larger than any valid distance, so that it will
//		be replaced as legal distance values are inserted.  The NULL
//		info value must be a nonvalid array index, we use ANN_NULL_IDX,
//		which is guaranteed to be negative.
//----------------------------------------------------------------------

const PQKkey	PQ_NULL_KEY = ANN_DIST_INF;	// nonexistent key value
const PQKinfo	PQ_NULL_INFO = ANN_NULL_IDX;	// nonexistent info value


//----------------------------------------------------------------------
//	ANNmin_k
//		An ANNmin_k structure is one which maintains the smallest
//		k values (of type PQKkey) and associated information (of type
//		PQKinfo).  The special info and key values PQ_NULL_INFO and
//		PQ_NULL_KEY means that thise entry is empty.
//
//		It is currently implemented using an array with k items.
//		Items are stored in increasing sorted order, and insertions
//		are made through standard insertion sort.  (This is quite
//		inefficient, but current applications call for small values
//		of k and relatively few insertions.)
//		
//		Note that the list contains k+1 entries, but the last entry
//		is used as a simple placeholder and is otherwise ignored.
//----------------------------------------------------------------------

class ANNmin_k {
  struct mk_node {					// node in min_k structure
    PQKkey			key;			// key value
    PQKinfo			info;			// info field (user defined)
    
    mk_node(PQKkey new_key, PQKinfo	new_info): key(new_key), info(new_info)
    {}
    
    inline friend bool operator<(const mk_node& mk1, const mk_node& mk2)
    {
      return mk1.key < mk2.key && !isNearlyEqual(mk1.key, mk2.key);
    }
  };
  
  int			k;						// max number of keys to store
  int			n;						// number of keys currently active
  std::vector<mk_node>	mk;			// the list itself
  
  // tie breaking tools
  int tie_ind;
  std::vector<mk_node> tie_bucket;	//vectors outperform lists for small elements like integers
  std::mt19937 RNG;
  
public:
  
  // constructor (given max size)
  ANNmin_k(int max) : 
    k(max),											// max size
    n(0),											// no elements in list
    mk(max, mk_node(PQ_NULL_KEY, PQ_NULL_INFO)),	// initialize with full size and placeholder values
    tie_ind(0),										// tie index equals zero
    tie_bucket(),									// begin with empty tie bucket
    RNG(unif_rand()*INT_MAX)
  {}
  
  
  PQKkey ANNmin_key()					// return minimum key
  {
    return (n > 0 ? mk.front().key : PQ_NULL_KEY);
  }
  
  PQKkey max_key()					// return maximum key
  {
    return (n == k ? mk.back().key : PQ_NULL_KEY);
  }
  
  PQKkey ith_smallest_key(int i)		// ith smallest key (i in [0..n-1])
  {
    return (i < n ? mk[i].key : PQ_NULL_KEY);
  }
  
  PQKinfo ith_smallest_info(int i)	// info for ith smallest (i in [0..n-1])
  {
    if (i < n)
    {
      if (i < tie_ind)
          return mk[i].info;
      else if (tie_bucket.size() == 1)
      {
        return tie_bucket.front().info;
      }
      else
      {
        std::uniform_int_distribution<> dis(0, std::distance(tie_bucket.begin(), tie_bucket.end()) - 1);
        auto it = tie_bucket.begin() + dis(RNG);
        PQKinfo res = it->info;
        tie_bucket.erase(it);
        return res;
      }
      
    }
    else
      return PQ_NULL_INFO;
  }
  
  inline void insert(			// insert item (inlined for speed)
      PQKkey kv,						// key value
      PQKinfo inf)					// item info
  {
    if (isNearlyEqual(mk[k - 1].key, kv))
      tie_bucket.emplace_back(kv, inf);
    else if (kv > mk.back().key)
      return;
    else
    {
      mk_node mk_new(kv, inf);
      if (mk[k-2] < mk_new)         // also means that tie_ind == k-1
      {
        mk.back() = mk_new;
        tie_bucket.clear();
        tie_bucket.push_back(mk_new);
      }
      else
      {
        // create new node and insert it at its correct place
        auto pos_mknew = std::lower_bound(mk.begin(), mk.begin() + tie_ind, mk_new);
        mk.insert(pos_mknew, mk_new);
        mk.pop_back();  //vector grows by inserted element, hence pop last element
        
       // check whether tie index has been at the end of the vector
       if (tie_ind == k - 1)
       {
         // reset tie index and throw all elements starting at this point into the bucket
         auto pos = std::lower_bound(pos_mknew, mk.end() - 1, mk[k - 1]);
         tie_ind = std::distance(mk.begin(), pos);
         tie_bucket = std::vector<mk_node>(pos, mk.end());
       }
       else
       {
         // if tie index wasn't last index, we just have to increment, as elements with same distance move one step back
         ++tie_ind;
       }
        
        ANN_FLOP(static_cast<int>(log2(k)))				// increment floating ops
      }
     
    }
    if (n < k) ++n;				// increment number of items
  }
  
};



#endif
