/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils.h"
#include "frost_utils_file.h"

using namespace std;

namespace Frost
{
    const s_str File::CLASS_NAME = "File";

    File::File( const s_str& sName, const FileType& mType, const s_bool& bBinary )
    {
        sName_ = sName;

        switch (mType)
        {
            case IO :
            {
                if (bBinary)
                    mFile_.open(sName.GetASCII().c_str(), ios_base::in | ios_base::out | ios::binary);
                else
                    mFile_.open(sName.GetASCII().c_str(), ios_base::in | ios_base::out);
                break;
            }
            case I :
            {
                if (bBinary)
                    mFile_.open(sName.GetASCII().c_str(), ios::in | ios::binary);
                else
                    mFile_.open(sName.GetASCII().c_str(), ios::in);
                break;
            }
            case O :
            {
                if (bBinary)
                    mFile_.open(sName.GetASCII().c_str(), ios::out | ios::binary);
                else
                    mFile_.open(sName.GetASCII().c_str(), ios::out);
                break;
            }
        }
    }

    File::~File()
    {
        Close();
    }

    void File::Close()
    {
        mFile_.close();
    }

    s_bool File::IsOpen()
    {
        return mFile_.is_open();
    }

    s_bool File::IsValid()
    {
        return (mFile_.is_open() && !mFile_.eof());
    }

    s_uint File::GetLength()
    {
        uint uiOldPos = mFile_.tellg();

        mFile_.seekg(0, ios::end);
        s_uint uiLength = (uint)mFile_.tellg();
        mFile_.seekg(uiOldPos);

        return uiLength;
    }

    const s_str File::GetName() const
    {
        return sName_;
    }

    s_str File::GetLine()
    {
        s_str sTemp;
        getline(mFile_, sTemp.GetR());
        #ifdef FROST_LINUX
            sTemp.Replace("\r", "");
        #endif
        return sTemp;
    }

    s_uint File::GetReadPos()
    {
        return (uint)mFile_.tellg();
    }

    void File::SetReadPos( const s_uint& uiPos )
    {
        mFile_.seekg(uiPos.Get());
    }

    s_uint File::GetWritePos()
    {
        return (uint)mFile_.tellp();
    }

    void File::SetWritePos( const s_uint& uiPos )
    {
        mFile_.seekp(uiPos.Get());
    }

    string_element File::Get()
    {
        string_element cTemp;
        mFile_.get(cTemp);
        return cTemp;
    }

    void File::Read( s_str& sBuffer, const s_uint& uiSize )
    {
        string_element* sTemp = new string_element[uiSize.Get()];
        mFile_.read(sTemp, uiSize.Get());
        sBuffer = sTemp;
        delete sTemp;
    }

    void File::Read( string_element* sBuffer, const s_uint& uiSize )
    {
        mFile_.read(sBuffer, uiSize.Get());
    }

    void File::Read( void* pBuffer, const s_uint& uiSize )
    {
        mFile_.read((char*)pBuffer, uiSize.Get());
    }

    void File::Write( const string_element* sBuffer, const s_uint& uiSize )
    {
        mFile_.write(sBuffer, uiSize.Get());
    }

    void File::Write( const void* sBuffer, const s_uint& uiSize )
    {
        mFile_.write((const char*)sBuffer, uiSize.Get());
    }

    void File::Write( const string_element& cChar )
    {
        mFile_.put(cChar);
    }

    void File::Write( const s_str& sLine )
    {
        mFile_.write(sLine.c_str(), sLine.GetSize().Get());
    }

    void File::WriteLine( const s_str& sLine )
    {
        mFile_.write(sLine.c_str(), sLine.GetSize().Get());
        mFile_.put('\n');
    }

    void File::Flush()
    {
        mFile_.flush();
    }

    s_bool File::Exists( const s_str& sFileName, const s_bool& bPrint )
    {
        fstream mFile;

        mFile.open(sFileName.GetASCII().c_str(), ios::in);
        if (mFile.is_open())
        {
            mFile.close();
            return true;
        }

        mFile.close();
        return false;
    }
}
