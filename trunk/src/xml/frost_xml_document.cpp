/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Document source             */
/*                                        */
/*                                        */

#include "frost_xml_document.h"
#include "frost_xml_block.h"

using namespace std;
using namespace Frost;
using namespace Frost::XML;

const s_str Document::CLASS_NAME = "Document";

Document::Document( const s_str& sFileName, const s_str& sDefFileName ) :
    sFileName_(sFileName), sDefFileName_(sDefFileName)
{
    if (File::Exists(sFileName_))
    {
        if (File::Exists(sDefFileName_))
        {
            mMainBlock_.SetDocument(this);
            uiLineNbr_ = 1;
            bValid_ = true;

            LoadDefinition_();
        }
        else
        {
            Error(CLASS_NAME, "Can't find \""+sDefFileName_+"\".");
            bValid_ = false;
        }
    }
    else
    {
        Error(CLASS_NAME, "Can't find \""+sFileName_+"\".");
        bValid_ = false;
    }
}

s_ptr<Block> Document::GetMainBlock()
{
    return &mMainBlock_;
}

const s_str& Document::GetFileName() const
{
    return sFileName_;
}

const s_uint& Document::GetLineNbr() const
{
    return uiLineNbr_;
}

s_bool Document::CheckLineSynthax_( const s_str& sLine )
{
    return ( (sLine.Get(0) == '<') && sLine.Find(">") );
}

s_bool Document::LoadDefinition_()
{
    if (bValid_)
    {
        File mFile(sDefFileName_, FILE_I);

        s_ptr<Block> pActual = &mMainBlock_;
        s_ptr<Block> pParent;

        while (mFile.IsValid())
        {
            s_str sLine = mFile.GetLine();

            #ifdef FROST_LINUX
            // Fix line endings
            if ((sLine.Length() > 0) && (sLine.Get(sLine.Length()-s_uint(1u)) == '\r'))
            {
                sLine.EraseFromEnd(1u);
            }
            #endif

            sLine.Trim(' ');

            if (!sLine.IsEmpty())
            {
                if (CheckLineSynthax_(sLine))
                {
                    // It's a tag
                    if (sLine.Find("/>"))
                    {
                        // Monoline block
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut("/>").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();
                        s_uint uiMax = s_uint::INF;
                        s_uint uiMin = 0u;
                        if (sName[0] == '*')
                        {
                            sName.EraseFromStart(1);
                            uiMax = uiMin = 1u;
                        }
                        else if (sName[0] == '$')
                        {
                            sName.EraseFromStart(1);
                            uiMax = 1u;
                        }
                        else if (sName[0] == '&')
                        {
                            sName.EraseFromStart(1);
                            uiMin = 1u;
                        }
                        vector<s_str> lAttribs;
                        if (lWords.size() > 1)
                        {
                            s_str sAttribs = lWords.back();
                            sAttribs.Replace(" =", "=");
                            sAttribs.Replace("= ", "=");
                            lAttribs = sAttribs.Cut(" ");
                        }

                        if (pParent)
                        {
                            pActual = pParent->CreateDefBlock(sName, uiMin, uiMax);
                            vector<s_str>::iterator iterAttr;
                            foreach (iterAttr, lAttribs)
                            {
                                s_str sAttr = *iterAttr;
                                s_str sDefault;
                                s_bool bOptional = false;
                                if (sAttr.Find("="))
                                {
                                    bOptional = true;
                                    vector<s_str> lCut = sAttr.Cut("=");
                                    sAttr = lCut.front();
                                    sDefault = lCut.back();
                                }
                                pActual->Add(Attribute(sAttr, bOptional, sDefault));
                            }
                            pActual = pParent;
                        }
                        else
                        {
                            // Main block
                            pActual->SetName(sName);
                            vector<s_str>::iterator iterAttr;
                            foreach (iterAttr, lAttribs)
                            {
                                s_str sAttr = *iterAttr;
                                s_str sDefault;
                                s_bool bOptional = false;
                                if (sAttr.Find("="))
                                {
                                    bOptional = true;
                                    vector<s_str> lCut = sAttr.Cut("=");
                                    sAttr = lCut.front();
                                    sDefault = lCut.back();
                                }
                                pActual->Add(Attribute(sAttr, bOptional));
                            }
                        }
                    }
                    else if (sLine.Find("</"))
                    {
                        // End tag
                        sLine.EraseFromStart(2);
                        sLine = sLine.Cut(">").front();
                        sLine.Trim(' ');

                        if (sLine == pActual->GetName())
                        {
                            pParent = pActual->GetParent();
                            if (pParent)
                            {
                                pActual = pParent;
                            }
                            else
                            {
                                // It's the main block's end tag
                                break;
                            }
                        }
                        else
                        {
                            Error(sFileName_+":"+uiLineNbr_,
                                "Wrong end tag : \"</"+sLine+">\". Expected : \"</"+pActual->GetName()+">\"."
                            );
                            bValid_ = false;
                            break;
                        }
                    }
                    else if (sLine.Find("<!--"))
                    {
                        // A comment
                    }
                    else
                    {
                        // Multiline block
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut(">").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();
                        s_uint uiMax = s_uint::INF;
                        s_uint uiMin = 0;
                        if (sName[0] == '*')
                        {
                            sName.EraseFromStart(1);
                            uiMax = uiMin = 1u;
                        }
                        else if (sName[0] == '$')
                        {
                            sName.EraseFromStart(1);
                            uiMax = 1u;
                        }
                        else if (sName[0] == '&')
                        {
                            sName.EraseFromStart(1);
                            uiMin = 1u;
                        }
                        vector<s_str> lAttribs;
                        if (lWords.size() > 1)
                        {
                            s_str sAttribs = lWords.back();
                            sAttribs.Replace(" =", "=");
                            sAttribs.Replace("= ", "=");
                            lAttribs = sAttribs.Cut(" ");
                        }

                        if (pParent)
                        {
                            pActual = pParent->CreateDefBlock(sName, uiMin, uiMax);
                            vector<s_str>::iterator iterAttr;
                            foreach (iterAttr, lAttribs)
                            {
                                s_str sAttr = *iterAttr;
                                s_str sDefault;
                                s_bool bOptional = false;
                                if (sAttr.Find("="))
                                {
                                    bOptional = true;
                                    vector<s_str> lCut = sAttr.Cut("=");
                                    sAttr = lCut.front();
                                    sDefault = lCut.back();
                                }
                                pActual->Add(Attribute(sAttr, bOptional));
                            }
                            pParent = pActual;
                        }
                        else
                        {
                            // Main block
                            pActual->SetName(sName);
                            pActual->SetName(sName);
                            vector<s_str>::iterator iterAttr;
                            foreach (iterAttr, lAttribs)
                            {
                                s_str sAttr = *iterAttr;
                                s_str sDefault;
                                s_bool bOptional = false;
                                if (sAttr.Find("="))
                                {
                                    bOptional = true;
                                    vector<s_str> lCut = sAttr.Cut("=");
                                    sAttr = lCut.front();
                                    sDefault = lCut.back();
                                }
                                pActual->Add(Attribute(sAttr, bOptional));
                            }
                            pParent = pActual;
                        }
                    }
                }
                else
                {
                    Error(sFileName_+":"+uiLineNbr_, "Invalid line.");
                    bValid_ = false;
                    break;
                }
            }
        }
    }

    return bValid_;
}

s_bool Document::Check()
{
    if (bValid_)
    {
        File mFile(sFileName_, FILE_I);

        s_ptr<Block> pActual = &mMainBlock_;
        s_ptr<Block> pParent;
        s_bool       bOpened;
        s_bool       bValue;

        while (mFile.IsValid())
        {
            s_str sLine = mFile.GetLine();

            #ifdef FROST_LINUX
            // Fix line endings
            if ((sLine.Length() > 0) && (sLine.Get(sLine.Length()-s_uint(1u)) == '\r'))
            {
                sLine.EraseFromEnd(1u);
            }
            #endif

            sLine.Trim(' ');

            if (!sLine.IsEmpty())
            {
                if (CheckLineSynthax_(sLine))
                {
                    // It's a tag
                    if (sLine.Find("/>") && !bValue)
                    {
                        // Monoline block
                        bOpened = false;
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut("/>").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();
                        s_str sAttribs;
                        if (lWords.size() > 1)
                        {
                            sAttribs = lWords.back();
                            sAttribs.Replace(" =", "=");
                            sAttribs.Replace("= ", "=");
                        }

                        if (pParent)
                        {
                            if (pParent->HasBlock(sName))
                            {
                                pActual = pParent->CreateBlock(sName);
                                if (!pActual->CheckAttributes(sAttribs))
                                {
                                    bValid_ = false;
                                    break;
                                }
                                if (!pActual->CheckBlocks())
                                {
                                    bValid_ = false;
                                    break;
                                }
                                pActual = pParent;
                                pParent->AddBlock();
                            }
                            else
                            {
                                Error(sFileName_+":"+uiLineNbr_, "Unexpected content : \"<"+sName+">\".");
                                bValid_ = false;
                                break;
                            }
                        }
                        else
                        {
                            // Main block
                            if (sName == pActual->GetName())
                            {
                                if (!pActual->CheckAttributes(sAttribs))
                                {
                                    bValid_ = false;
                                }
                                if (!pActual->CheckBlocks())
                                {
                                    bValid_ = false;
                                }
                                break;
                            }
                            else
                            {
                                Error(sFileName_+":"+uiLineNbr_,
                                    "Wrong content : \"<"+sName+">\". Expected : \"<"+pActual->GetName()+">\"."
                                );
                                bValid_ = false;
                                break;
                            }
                        }
                    }
                    else if (sLine.Find("</"))
                    {
                        // End tag
                        bOpened = false;
                        bValue = false;
                        sLine.EraseFromStart(2);
                        sLine = sLine.Cut(">").front();
                        sLine.Trim(' ');
                        if (sLine == pActual->GetName())
                        {
                            pParent = pActual->GetParent();
                            if (pParent)
                            {
                                if (!pActual->CheckBlocks())
                                {
                                    bValid_ = false;
                                    break;
                                }
                                pParent->AddBlock();
                                pActual = pParent;
                            }
                            else
                            {
                                // It's the main block's end tag
                                if (!pActual->CheckBlocks())
                                {
                                    bValid_ = false;
                                    break;
                                }
                                break;
                            }
                        }
                        else
                        {
                            Error(sFileName_+":"+uiLineNbr_,
                                "Wrong end tag : \"</"+sLine+">\". Expected : \"</"+pActual->GetName()+">\"."
                            );
                            bValid_ = false;
                            break;
                        }
                    }
                    else if (sLine.Find("<!--"))
                    {
                        // A comment
                    }
                    else if (!bValue)
                    {
                        // Multiline block
                        bOpened = true;
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut(">").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();
                        s_str sAttribs;
                        if (lWords.size() > 1)
                        {
                            sAttribs = lWords.back();
                            sAttribs.Replace(" =", "=");
                            sAttribs.Replace("= ", "=");
                        }

                        if (pParent)
                        {
                            if (pParent->HasBlock(sName))
                            {
                                pActual = pParent->CreateBlock(sName);
                                if (!pActual->CheckAttributes(sAttribs))
                                {
                                    bValid_ = false;
                                    break;
                                }
                                pParent = pActual;
                            }
                            else
                            {
                                Error(sFileName_+":"+uiLineNbr_, "Unexpected content : \"<"+sName+">\".");
                                bValid_ = false;
                                break;
                            }
                        }
                        else
                        {
                            // Main block
                            if (sName == pActual->GetName())
                            {
                                if (!pActual->CheckAttributes(sAttribs))
                                {
                                    bValid_ = false;
                                    break;
                                }
                                pParent = pActual;
                            }
                            else
                            {
                                Error(sFileName_+":"+uiLineNbr_,
                                    "Wrong content : \"<"+sName+">\". Expected : \"<"+pActual->GetName()+">\"."
                                );
                                bValid_ = false;
                                break;
                            }
                        }
                    }
                    else
                    {
                        Error(sFileName_+":"+uiLineNbr_, "Invalid line.");
                        bValid_ = false;
                        break;
                    }
                }
                else if (bOpened)
                {
                    pActual->SetValue(pActual->GetValue() + "\n" + sLine);
                    bValue = true;
                }
                else
                {
                    Error(sFileName_+":"+uiLineNbr_, "Invalid line.");
                    bValid_ = false;
                    break;
                }
            }

            uiLineNbr_++;
        }
    }

    return bValid_;
}
