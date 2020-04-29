/** \file
 * \brief Declaration and implementation of EdgeArray class.
 *
 * \author Carsten Gutwenger
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the OGDF root directory for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */

#pragma once

#include <ogdf/basic/Graph_d.h>


namespace ogdf {


//! Abstract base class for edge arrays.
/**
 * Defines the interface for event handling used by the Graph class.
 * Use the parameterized class EdgeArray for creating edge arrays.
 */
class EdgeArrayBase {
	/**
	 * Pointer to list element in the list of all registered edge
	 * arrays which references this array.
	 */
	ListIterator<EdgeArrayBase*> m_it;

public:
	const Graph *m_pGraph; //!< The associated graph.

	//! Initializes an edge array not associated with a graph.
	EdgeArrayBase() : m_pGraph(nullptr) { }

	//! Initializes an edge array associated with \p pG.
	explicit EdgeArrayBase(const Graph *pG) : m_pGraph(pG) {
		if(pG) m_it = pG->registerArray(this);
	}

	//! Moves edge array \p base to this edge array.
	EdgeArrayBase(EdgeArrayBase &base) : m_it(base.m_it), m_pGraph(base.m_pGraph) {
		if(m_pGraph) m_pGraph->moveRegisterArray(m_it, this);
		base.m_pGraph = nullptr;
		base.m_it     = ListIterator<EdgeArrayBase*>();
	}

	// destructor, unregisters the array
	virtual ~EdgeArrayBase() {
		if (m_pGraph) m_pGraph->unregisterArray(m_it);
	}

	// event interface used by Graph
	//! Virtual function called when table size has to be enlarged.
	virtual void enlargeTable(int newTableSize) = 0;
	//! Virtual function called when table has to be reinitialized.
	virtual void reinit(int initTableSize) = 0;
	//! Virtual function called when array is disconnected from the graph.
	virtual void disconnect() = 0;

	//! Associates the array with a new graph.
	void reregister(const Graph *pG) {
		if (m_pGraph) m_pGraph->unregisterArray(m_it);
		if ((m_pGraph = pG) != nullptr) m_it = pG->registerArray(this);
	}

	//! Moves array registration from \p base to this array.
	void moveRegister(EdgeArrayBase &base) {
		if (m_pGraph) m_pGraph->unregisterArray(m_it);
		m_pGraph = base.m_pGraph;
		m_it     = base.m_it;
		base.m_pGraph = nullptr;
		base.m_it     = ListIterator<EdgeArrayBase*>();
		if (m_pGraph != nullptr)
			m_pGraph->moveRegisterArray(m_it, this);
	}
};

//! Dynamic arrays indexed with edges.
/**
 * @ingroup graph-containers
 *
 * Edge arrays represent a mapping from edges to data of type \a T.
 * They adjust their table size automatically when the graph grows.
 *
 * @warn_undef_behavior_array
 *
 * @tparam T is the element type.
 */
template<class T> class EdgeArray : private Array<T>, protected EdgeArrayBase {
	T m_x; //!< The default value for array elements.

public:
	//! The type for array keys.
	using key_type = edge;
	//! The type for array entries.
	using value_type = T;

	//! The type for edge array iterators.
	using iterator = internal::GraphArrayIterator<EdgeArray<T>>;
	//! The type for edge array const iterators.
	using const_iterator = internal::GraphArrayConstIterator<EdgeArray<T>>;


	//! Constructs an empty edge array associated with no graph.
	EdgeArray() : Array<T>(), EdgeArrayBase() { }

	//! Constructs an edge array associated with \p G.
	explicit EdgeArray(const Graph &G) : Array<T>(G.edgeArrayTableSize()), EdgeArrayBase(&G) { }

	//! Constructs an edge array associated with \p G.
	/**
	 * @param G is the associated graph.
	 * @param x is the default value for all array elements.
	 */
	EdgeArray(const Graph &G, const T &x) :
		Array<T>(0,G.edgeArrayTableSize()-1,x), EdgeArrayBase(&G), m_x(x) { }

	//! Constructs an edge array that is a copy of \p A.
	/**
	 * Associates the array with the same graph as \p A and copies all elements.
	 */
	EdgeArray(const EdgeArray<T> &A) : Array<T>(A), EdgeArrayBase(A.m_pGraph), m_x(A.m_x) { }

	//! Constructs an edge array containing the elements of \p A (move semantics).
	/**
	 * Edge array \p A is empty afterwards and not associated with any graph.
	 */
	EdgeArray(EdgeArray<T> &&A) : Array<T>(std::move(A)), EdgeArrayBase(A), m_x(A.m_x) { }


	/**
	 * @name Access methods
	 * These methods provide access to elements, size, and corresponding graph.
	 */
	//@{

	//! Returns true iff the array is associated with a graph.
	bool valid() const { return Array<T>::low() <= Array<T>::high(); }

	//! Returns a pointer to the associated graph.
	const Graph *graphOf() const {
		return m_pGraph;
	}

	//! Returns a reference to the element with index \p e.
	const T &operator[](edge e) const {
		OGDF_ASSERT(e != nullptr);
		OGDF_ASSERT(e->graphOf() == m_pGraph);
		return Array<T>::operator [](e->index());
	}

	//! Returns a reference to the element with index \p e.
	T &operator[](edge e) {
		OGDF_ASSERT(e != nullptr);
		OGDF_ASSERT(e->graphOf() == m_pGraph);
		return Array<T>::operator [](e->index());
	}

	//! Returns a reference to the element with index \p e.
	const T &operator()(edge e) const {
		OGDF_ASSERT(e != nullptr);
		OGDF_ASSERT(e->graphOf() == m_pGraph);
		return Array<T>::operator [](e->index());
	}

	//! Returns a reference to the element with index \p e.
	T &operator()(edge e) {
		OGDF_ASSERT(e != nullptr);
		OGDF_ASSERT(e->graphOf() == m_pGraph);
		return Array<T>::operator [](e->index());
	}

	//! Returns a reference to the element with index edge of \p adj.
	const T &operator[](adjEntry adj) const {
		OGDF_ASSERT(adj != nullptr);
		return Array<T>::operator [](adj->index() >> 1);
	}

	//! Returns a reference to the element with index edge of \p adj.
	T &operator[](adjEntry adj) {
		OGDF_ASSERT(adj != nullptr);
		return Array<T>::operator [](adj->index() >> 1);
	}

	//! Returns a reference to the element with index edge of \p adj.
	const T &operator()(adjEntry adj) const {
		OGDF_ASSERT(adj != nullptr);
		return Array<T>::operator [](adj->index() >> 1);
	}

	//! Returns a reference to the element with index edge of \p adj.
	T &operator()(adjEntry adj) {
		OGDF_ASSERT(adj != nullptr);
		return Array<T>::operator [](adj->index() >> 1);
	}

	//! Returns a reference to the element with index \p index.
	//! \attention Make sure that \p index is a valid index for an edge in the associated graph!
	OGDF_DEPRECATED("Edge arrays should be indexed by an edge, not an integer index.")
	const T &operator[](int index) const
		{ return Array<T>::operator [](index); }

	//! Returns a reference to the element with index \p index.
	//! \attention Make sure that \p index is a valid index for an edge in the associated graph!
	OGDF_DEPRECATED("Edge arrays should be indexed by an edge, not an integer index.")
	T &operator[](int index)
		{ return Array<T>::operator [](index); }

	//@}
	/**
	 * @name Iterators
	 * These methods return bidirectional iterators to elements in the array.
	 */
	//@{

	//! Returns an iterator to the first entry in the edge array.
	/**
	 * If the edge array is empty, a null pointer iterator is returned.
	 */
	iterator begin() { return iterator(m_pGraph->firstEdge(), this); }

	//! Returns a const iterator to the first entry in the edge array.
	/**
	 * If the edge array is empty, a null pointer iterator is returned.
	 */
	const_iterator begin() const { return const_iterator(m_pGraph->firstEdge(), this); }

	//! Returns a const iterator to the first entry in the edge array.
	/**
	 * If the edge array is empty, a null pointer iterator is returned.
	 */
	const_iterator cbegin() const { return const_iterator(m_pGraph->firstEdge(), this); }

	//! Returns an iterator to one-past-last entry in the edge array.
	/**
	 * This is always a null pointer iterator.
	 */
	iterator end() { return iterator(nullptr, this); }

	//! Returns a const iterator to one-past-last entry in the edge array.
	/**
	 * This is always a null pointer iterator.
	 */
	const_iterator end() const { return const_iterator(nullptr, this); }

	//! Returns a const iterator to one-past-last entry in the edge array.
	/**
	 * This is always a null pointer iterator.
	 */
	const_iterator cend() const { return const_iterator(nullptr, this); }

	//@}
	/**
	 * @name Initialization and assignment
	 * These methods can be used to reinitialize the array, or to initialize all elements with a given value.
	 */
	//@{

	//! Reinitializes the array. Associates the array with no graph.
	void init() {
		Array<T>::init(); reregister(nullptr);
	}

	//! Reinitializes the array. Associates the array with \p G.
	void init(const Graph &G) {
		Array<T>::init(G.edgeArrayTableSize()); reregister(&G);
	}

	//! Reinitializes the array. Associates the array with \p G.
	/**
	 * @param G is the associated graph.
	 * @param x is the default value.
	 */
	void init(const Graph &G, const T &x) {
		Array<T>::init(0,G.edgeArrayTableSize()-1, m_x = x); reregister(&G);
	}

	//! Sets all array elements to \p x.
	void fill(const T &x) {
		int high = m_pGraph->maxEdgeIndex();
		if(high >= 0)
			Array<T>::fill(0,high,x);
	}

	//! Assignment operator.
	EdgeArray<T> &operator=(const EdgeArray<T> &a) {
		Array<T>::operator=(a);
		m_x = a.m_x;
		reregister(a.m_pGraph);
		return *this;
	}

	//! Assignment operator (move semantics).
	/**
	 * Edge array \p a is empty afterwards and not associated with any graph.
	 */
	EdgeArray<T> &operator=(EdgeArray<T> &&a) {
		Array<T>::operator=(std::move(a));
		m_x = a.m_x;
		moveRegister(a);
		return *this;
	}


	//@}
	/**
	 * @name Helper functions
	 * These methods are mainly intended for internal use.
	 */
	//@{

	static key_type findSuccKey(key_type key) { return key->succ(); }
	static key_type findPredKey(key_type key) { return key->pred(); }

	//@}

private:
	virtual void enlargeTable(int newTableSize) {
		Array<T>::resize(newTableSize,m_x);
	}

	virtual void reinit(int initTableSize) {
		Array<T>::init(0,initTableSize-1,m_x);
	}

	virtual void disconnect() {
		Array<T>::init();
		m_pGraph = nullptr;
	}

	OGDF_NEW_DELETE
};

//! Bucket function for edges.
/**
 * The bucket of an edge is stored in an edge array which is passed
 * by the user at construction; only a pointer is stored to that array.
 */
class OGDF_EXPORT BucketEdgeArray : public BucketFunc<edge>
{
	const EdgeArray<int> *m_pEdgeArray; //!< Pointer to edge array.

public:
	//! Constructs a bucket function.
	/**
	 * @param edgeArray contains the buckets for the edges. May not be deleted
	 *        as long as the bucket function is used.
	 */
	explicit BucketEdgeArray(const EdgeArray<int> &edgeArray) : m_pEdgeArray(&edgeArray) { }

	//! Returns bucket of edge \p e.
	int getBucket(const edge &e) override { return (*m_pEdgeArray)[e]; }
};

}
