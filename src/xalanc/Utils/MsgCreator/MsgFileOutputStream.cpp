/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MsgFileOutputStream.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLString.hpp>

#include <cassert>
#include <cerrno>
#include <cstdio>


static  MsgFileOutputStream::HandleType
openFile(const char*    theFileName)
{
    assert(theFileName != 0);

    return fopen(theFileName, "wb");
}



MsgFileOutputStream::MsgFileOutputStream(const char*    theFileName) :
            m_fileName(theFileName),
            m_handle(openFile(theFileName))
{
    if (m_handle == 0)
    {
        throw OpenException(
                    theFileName,
                    errno);
    }
}



MsgFileOutputStream::~MsgFileOutputStream()
{
    if (m_handle != 0)
    {
        fclose(m_handle);
    }
}



void
MsgFileOutputStream::doFlush()
{
    if (fflush(m_handle) != 0)
    {
        throw WriteException(
                m_fileName,
                errno);
    }
}



void
MsgFileOutputStream::writeData(
            const char*     theBuffer,
            unsigned int    theBufferLength)
{
    const size_t    theBytesWritten =
        fwrite(theBuffer,
               1,
               theBufferLength,
               m_handle);

    if (theBytesWritten != theBufferLength)
    {
        throw WriteException(
                m_fileName,
                errno);
    }
}



void
FormatMessageLocal(
            const char*     theMessage,
            const char*     theFileName,
            int             theErrorCode,
            char*           theResult)
{
    assert(theMessage != 0);
    assert(theFileName != 0);
    assert(theResult != 0);

    sprintf(
        theResult,
        "%.100s %.1000s.  The C++ run-time error code (errno) is %d.",
        theMessage,
        theFileName,
        theErrorCode);
}



MsgFileOutputStream::OpenException::OpenException(
        const char*     theFileName,
        int             theErrorCode) :
    m_message()
{
    FormatMessageLocal(
        "Error opening file: ",
        theFileName,
        theErrorCode,
        m_message);    
}



MsgFileOutputStream::OpenException::~OpenException()
{
}



MsgFileOutputStream::WriteException::WriteException(
        const char*     theFileName,
        int             theErrorCode) :
    m_message()
{
    FormatMessageLocal(
        "Error writing file: ",
        theFileName,
        theErrorCode,
        m_message);
}



MsgFileOutputStream::WriteException::~WriteException()
{
}



void
MsgFileOutputStream::write(
            const XMLCh*    theString,
            unsigned int    theLength)
{
    assert(theString != 0);

    writeData(
        (const char*)theString,
        theLength * sizeof(XMLCh));
}



void
MsgFileOutputStream::write(
            const char*     theString,
            unsigned int    theLength)
{
    assert (theString != 0);

    XMLCh*  theUTFString =
        XMLString::transcode(theString);

    write(
        theUTFString,
        theLength);

    XMLString::release(&theUTFString);
}



void
MsgFileOutputStream::writeAsASCII(
            const XMLCh*    theString,
            unsigned int    theLength)
{
    char*   szString =
        XMLString::transcode(theString);

    writeData(
        szString,
        theLength);

    XMLString::release(&szString);
}



void
MsgFileOutputStream::writeAsASCII(
            const char*     theString,
            unsigned int    theLength)
{
    writeData(
        theString,
        theLength);
}



static const XMLCh s_UTF16ByteOrderMark[] =
{
    XMLCh(0xFEFF)
};


void
MsgFileOutputStream::writeUTFPrefix()
{
    write(
        s_UTF16ByteOrderMark,
        1);
}
