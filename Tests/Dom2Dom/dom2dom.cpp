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
// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>

#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>
#include <vector>

// This is here for memory leak testing. 
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include <sax/SAXException.hpp>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/XalanStdOutputStream.hpp>

#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>

#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>

#include <XalanTransformer/XalanTransformer.hpp>
#include <XalanTransformer/XalanCompiledStylesheetDefault.hpp>

#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>

#include <parsers/DOMParser.hpp>
#include <dom/DOM_Node.hpp>

#include <XercesParserLiaison/XercesDOMSupport.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>


#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif


void
printArgOptions()
{
	cerr << endl
		 << "dom2dom dirname [-out]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl;
}

bool
getParams(int argc, 
		  const char*	argv[],
		  FileUtility& f,
		  XalanDOMString& basedir,
		  XalanDOMString& outdir)
{
	bool fSuccess = true;	// Used to continue argument loop
	bool fSetOut = true;	// Set default output directory

	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		if (f.checkDir(pathSep + XalanDOMString(argv[1])))
		{
			assign(basedir, XalanDOMString(argv[1]));
			insert(basedir, 0, pathSep);
		}
		else
		{
			cout << endl << "Given base directory \"" << argv[1] << "\" does not exist" << endl;
			printArgOptions();
			return false;
		}
	}

	// Get the rest of the arguments in any order.
	for (int i = 2; i < argc && fSuccess == true; ++i)
	{
		if(!stricmp("-out", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(outdir, XalanDOMString(argv[i]));
				insert(outdir, 0, XalanDOMString("\\"));
				append(outdir, XalanDOMString("\\"));
				f.checkAndCreateDir(outdir);
				fSetOut = false;
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else
		{
			printArgOptions();
			fSuccess = false;
		}

	} // End of for-loop

	// Do we need to set the default output directory??
	if (fSetOut)
	{
		unsigned int ii = lastIndexOf(basedir,charAt(pathSep,0));
		outdir = substring(basedir, 0, ii+1);
		append(outdir,XalanDOMString("Dom2Dom-RESULTS\\"));
		f.checkAndCreateDir(outdir);
	}
	
	// Add the path seperator to the end of the base directory
	append(basedir, pathSep);
	return fSuccess;
}

FormatterListener* 
getXMLFormatter(bool					shouldWriteXMLHeader,
				bool					stripCData,
				bool					escapeCData,
				bool					noIndent,
				PrintWriter&			resultWriter,
				int						indentAmount,
				const XalanDOMString&	mimeEncoding,
				const StylesheetRoot*	stylesheet)
{
	FormatterListener*	formatter = 0;

		XalanDOMString	version;
		bool			outputIndent = false;
		XalanDOMString	mediatype;
		XalanDOMString	doctypeSystem;
		XalanDOMString	doctypePublic;
		XalanDOMString	standalone;

		if (stylesheet != 0)
		{
			version = stylesheet->m_version;

			mediatype = stylesheet->m_mediatype;
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->m_standalone;
		}

		FormatterToXML* const	fToXML =
			new FormatterToXML(
					resultWriter,
					version,
					outputIndent,
					indentAmount,
					mimeEncoding,
					mediatype,
					doctypeSystem,
					doctypePublic,
					true,	// xmlDecl
					standalone);

		fToXML->setShouldWriteXMLHeader(shouldWriteXMLHeader);
		fToXML->setStripCData(stripCData);
		fToXML->setEscapeCData(escapeCData);

		formatter = fToXML;
		return formatter;
}

int
main(
		  int			argc,
		  const char*	argv [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	FileUtility		f;

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir;	
	XalanDOMString  outputRoot;
	
	const XalanDOMString mimeEncoding(XALAN_STATIC_UCODE_STRING("UTF-8"));
	const XalanDOMString encoding(XALAN_STATIC_UCODE_STRING("UTF-8"));

	if (getParams(argc, argv, f, baseDir, outputRoot) == true)
	{

		// Get the list of Directories that are below perf
		const FileNameVectorType	dirs = f.getDirectoryNames(baseDir);

		// Generate Unique Run id. (Only used to name the result logfile.)
		const XalanDOMString UniqRunid = f.GenerateUniqRunid();

		// Defined basic constants for file manipulation 

		const XalanDOMString  resultFilePrefix("dom2dom");
		const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);
		
		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Dom2Dom Testing: Treating all inputs and outputs as DOM's. ");

		try
		{
			// Call the static initializers...
			HarnessInit xmlPlatformUtils;
			XalanTransformer::initialize();

			{
				XalanTransformer		transformEngine;
						
				XercesDOMSupport domSupport;
				XercesParserLiaison parserLiaison(domSupport);
					
				// Specify the "test" directory for both input and output.
				const XalanDOMString  xMan("dtod");
				const XalanDOMString  theOutputDir = outputRoot + xMan;
				f.checkAndCreateDir(theOutputDir);

				// Get the files found in the test directory
				const FileNameVectorType	files = f.getTestFileNames(baseDir, xMan,true);

				for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
				{
					// Output file name to result log and console.
					logFile.logTestCaseInit(files[i]);
					cout << files[i] << endl;

					// Set up the input/output files.
					const XalanDOMString  theXSLFile= baseDir + xMan + pathSep + files[i];
					const XalanDOMString  theXMLFile = f.GenerateFileName(theXSLFile,"xml");
					const XalanDOMString  theOutput =  outputRoot + xMan + pathSep + files[i]; 
					const XalanDOMString  theOutputFile = f.GenerateFileName(theOutput, "out");

					// Use a Xerces Dom document to create the XSLTResultTarget. 
					XalanDocument* domOut = parserLiaison.createDocument();
					const XSLTResultTarget domResultTarget(domOut);

					const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
					const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
					
					const XalanCompiledStylesheet* const compiledSS = transformEngine.compileStylesheet(xslInputSource);
					assert( compiledSS != 0);

					// Transform using compiled stylesheet.
					int	theResult =
						transformEngine.transform(xmlInputSource, compiledSS, domResultTarget);

					if(theResult != 0)
					{
						logFile.logTestCaseClose("Done","Fail");
						cerr << "XalanError: \n" << transformEngine.getLastError();
					}
					else
					{
						XalanFileOutputStream myOutput(theOutputFile);
						XalanOutputStreamPrintWriter myResultWriter(myOutput);
						FormatterListener* theFormatter = getXMLFormatter(true,true,true,false,
																	myResultWriter,0,
																	mimeEncoding,
																	compiledSS->getStylesheetRoot());

						FormatterTreeWalker theTreeWalker(*theFormatter);
						theTreeWalker.traverse(domOut);

						delete theFormatter;
						logFile.logTestCaseClose("Done","Pass");
					}	

				}
				
			}

			XalanTransformer::terminate();

			logFile.logTestFileClose("Dom2Dom Testing: ", "Done");
			logFile.close();

		}

		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;

}