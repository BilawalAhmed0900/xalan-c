/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "XercesToXalanNodeMap.hpp"



#include <algorithm>
#include <cassert>



#include <dom/DOM_Document.hpp>



#include <XalanDOM/XalanDOMException.hpp>
#include <XalanDOM/XalanNode.hpp>



XercesToXalanNodeMap::XercesToXalanNodeMap() :
	m_xalanMap(),
	m_xercesMap(),
	m_counter(0)
{
}



XercesToXalanNodeMap::~XercesToXalanNodeMap()
{
}



XercesToXalanNodeMap::addAssociation(
			const DOM_Node&		theXercesNode,
			XalanNode*			theXalanNode,
			bool				fAssignIndex)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::make_pair;
#endif

	// Space the indices, just in case nodes are
	// added.  With any luck, we may not need to
	// reindex the nodes.
	const unsigned long		theIncrement = 5;

	NodeImpl* const		theImpl = XercesDOM_NodeHack::getImpl(theXercesNode);

	m_xercesMap.insert(make_pair(theImpl, theXalanNode));

	try
	{
		// Unindexed nodes are assigned an index of 0.
		unsigned long	theIndex = 0;

		// Have we been told to assign an index?
		if (fAssignIndex == true)
		{
			// Never index attribute nodes or their childern...
			if (theXalanNode->getNodeType() != XalanNode::ATTRIBUTE_NODE)
			{
				const XalanNode* const	theParent =
					theXalanNode->getParentNode();

				if (theParent == 0 || theParent->getNodeType() != XalanNode::ATTRIBUTE_NODE)
				{
					theIndex = m_counter += theIncrement;
				}
			}
		}

		m_xalanMap.insert(make_pair(theXalanNode, XalanNodeMapEntryType(theImpl, theIndex)));
	}
	catch(...)
	{
		m_xercesMap.erase(m_xercesMap.find(theImpl));

		throw;
	}
}



XercesToXalanNodeMap::clear()
{
	m_xalanMap.clear();
	m_xercesMap.clear();
}



bool
XercesToXalanNodeMap::isNodeAfter(
			const XalanNode*	theFirstXalanNode,
			const XalanNode*	theSecondXalanNode) const
{
	assert(theFirstXalanNode != 0);
	assert(theSecondXalanNode != 0);
	assert(theFirstXalanNode->getOwnerDocument() == theSecondXalanNode->getOwnerDocument());

	bool fResult = false;

	const XalanNodeMapType::const_iterator	i =
			m_xalanMap.find(theFirstXalanNode);

	if (i == m_xalanMap.end())
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
	else
	{
		const XalanNodeMapType::const_iterator	j =
			m_xalanMap.find(theSecondXalanNode);

		if (i == m_xalanMap.end())
		{
			throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
		}
		else
		{
			if (i->second.m_index > j->second.m_index)
			{
				fResult = true;
			}
		}
	}

	return fResult;
}
