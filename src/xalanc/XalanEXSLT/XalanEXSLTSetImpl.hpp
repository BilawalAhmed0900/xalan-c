/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2002-2003 The Apache Software Foundation.  All rights 
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
#if !defined(EXSLT_SETIMPL_HEADER_GUARD_1357924680)
#define EXSLT_SETIMPL_HEADER_GUARD_1357924680



#include <xalanc/XalanEXSLT/XalanEXSLTDefinitions.hpp>



#include <xalanc/XPath/Function.hpp>



#include <xalanc/XalanExtensions/FunctionDifference.hpp>
#include <xalanc/XalanExtensions/FunctionDistinct.hpp>
#include <xalanc/XalanExtensions/FunctionIntersection.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionDifference : public FunctionDifference
{
public:

	XalanEXSLTFunctionDifference() :
		FunctionDifference()
	{
	}

	virtual
	~XalanEXSLTFunctionDifference()
	{
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionDifference*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionDifference(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function difference() accepts two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionDifference&
	operator=(const XalanEXSLTFunctionDifference&);

	bool
	operator==(const XalanEXSLTFunctionDifference&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionDistinct : public FunctionDistinct
{
public:

	XalanEXSLTFunctionDistinct() :
		FunctionDistinct()
	{
	}

	virtual
	~XalanEXSLTFunctionDistinct()
	{
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionDistinct*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionDistinct(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function distinct() accepts one argument"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionDistinct&
	operator=(const XalanEXSLTFunctionDistinct&);

	bool
	operator==(const XalanEXSLTFunctionDistinct&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionHasSameNode : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionHasSameNode() :
		Function()
	{
	}

	virtual
	~XalanEXSLTFunctionHasSameNode()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionHasSameNode*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionHasSameNode(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function has-same-node() accepts two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionHasSameNode&
	operator=(const XalanEXSLTFunctionHasSameNode&);

	bool
	operator==(const XalanEXSLTFunctionHasSameNode&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionIntersection : public FunctionIntersection
{
public:

	XalanEXSLTFunctionIntersection() :
		FunctionIntersection()
	{
	}

	virtual
	~XalanEXSLTFunctionIntersection()
	{
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionIntersection*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionIntersection(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function distinct() function accepts two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionIntersection&
	operator=(const XalanEXSLTFunctionIntersection&);

	bool
	operator==(const XalanEXSLTFunctionIntersection&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionLeading : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionLeading() :
		Function()
	{
	}

	virtual
	~XalanEXSLTFunctionLeading()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionLeading*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionLeading(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function leading() function accepts two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionLeading&
	operator=(const XalanEXSLTFunctionLeading&);

	bool
	operator==(const XalanEXSLTFunctionLeading&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionTrailing : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionTrailing() :
		Function()
	{
	}

	virtual
	~XalanEXSLTFunctionTrailing()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionTrailing*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionTrailing(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function trailing() function accepts two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionTrailing&
	operator=(const XalanEXSLTFunctionTrailing&);

	bool
	operator==(const XalanEXSLTFunctionTrailing&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// EXSLT_SETIMPL_HEADER_GUARD_1357924680