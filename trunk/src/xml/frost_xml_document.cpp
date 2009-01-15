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
            uiLineNbr_ = 1u;
            bValid_ = true;
            bValid_ = LoadDefinition_();
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

Document::~Document()
{
}

s_ptr<Block> Document::GetMainBlock()
{
    return &mMainBlock_;
}

const s_str& Document::GetFileName() const
{
    return sActualFileName_;
}

const s_uint& Document::GetLineNbr() const
{
    return uiLineNbr_;
}

s_bool Document::CheckLineSynthax_( s_str& sLine )
{
    #ifdef FROST_LINUX
    // Fix line endings
    if ((sLine.Length() > 0u) && (sLine.Get(sLine.Length() - 1u) == '\r'))
    {
        sLine.EraseFromEnd(1u);
    }
    #endif

    s_uint uiPos = sLine.Find("<");
    if (uiPos)
    {
        sLine.EraseFromStart(uiPos);
        uiPos = sLine.Find(">");
        if (uiPos)
        {
            sLine.EraseFromEnd(sLine.Size()-uiPos-1);
            return true;
        }
    }

    return false;
}

s_bool Document::LoadDefinition_()
{
    if (bValid_)
    {
        sActualFileName_ = sDefFileName_;
        File mFile(sDefFileName_, FILE_I);

        s_ptr<Block> pActual;
        s_ptr<Block> pParent;

        while (mFile.IsValid())
        {
            s_str sLine = mFile.GetLine();

            if (!sLine.IsEmpty(true))
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
                        s_uint uiMin = 0u;
                        s_uint uiMax = s_uint::INF;
                        s_bool bPreDefining = false;
                        s_bool bRadio = false;
                        s_bool bLoad = false;
                        s_str sParent;

                        // Read commands
                        vector<s_str> lCommands = sName.Cut(":");
                        sName = lCommands.back();
                        lCommands.pop_back();
                        vector<s_str>::iterator iterCommand;
                        foreach (iterCommand, lCommands)
                        {
                            s_str sLetterCode = s_str((*iterCommand)[0]);
                            if (sLetterCode == "d")
                            {
                                // Pre-definintion
                                if (pParent)
                                {
                                    Error(sDefFileName_+":"+uiLineNbr_,
                                        "Can't pre-define a block outside root level (nested \'d\' command forbidden)."
                                    );
                                    return false;
                                }
                                else
                                {
                                    bPreDefining = true;

                                    s_uint uiStart = iterCommand->Find("[");
                                    s_uint uiEnd = iterCommand->Find("]");
                                    if (uiStart && uiEnd)
                                    {
                                        // Inheritance
                                        sParent = iterCommand->ExtractRange(uiStart+1, uiEnd);
                                    }
                                }
                            }
                            else if (sLetterCode == "l")
                            {
                                // Load pre-definition
                                if (!pParent)
                                {
                                    Error(sDefFileName_+":"+uiLineNbr_,
                                        "Can't load a pre-defined block at root level (\'l\' command forbidden)."
                                    );
                                    return false;
                                }
                                else
                                {
                                    bLoad = true;
                                }
                            }
                            else if (sLetterCode == "n")
                            {
                                // Min/max count
                                s_uint uiStart = iterCommand->Find("[");
                                s_uint uiEnd = iterCommand->Find("]");
                                if (uiStart && uiEnd)
                                {
                                    s_str sParams = iterCommand->ExtractRange(uiStart+1, uiEnd);
                                    if (sParams.Find(","))
                                    {
                                        vector<s_str> lMinMax = sParams.Cut(",");
                                        s_str sMin = lMinMax.front();
                                        s_str sMax = lMinMax.back();
                                        if (sMin != ".")
                                            uiMin = s_uint(sMin);
                                        if (sMax != ".")
                                            uiMax = s_uint(sMax);
                                    }
                                    else
                                    {
                                        if (sParams == "*")
                                            bRadio = true;
                                        else
                                        {
                                            Warning(sDefFileName_+":"+uiLineNbr_,
                                                "Unknown param : \""+sParams+"\" for \'n\' command. Skipped."
                                            );
                                        }
                                    }
                                }
                                else
                                {
                                    Warning(sDefFileName_+":"+uiLineNbr_,
                                        "\'n\' command requires some parameters. Correct synthax is : \"n[params]\". Skipped."
                                    );
                                }
                            }
                            else
                            {
                                Warning(sDefFileName_+":"+uiLineNbr_,
                                    "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                );
                            }
                        }

                        // Prepare attributes
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
                            if (bLoad)
                            {
                                if (MAPFIND(sName, lPredefinedBlockList_))
                                {
                                    s_ptr<PredefinedBlock> pAdded;
                                    if (bRadio)
                                        pAdded = pParent->AddPredefinedRadioBlock(&lPredefinedBlockList_[sName]);
                                    else
                                        pAdded = pParent->AddPredefinedBlock(&lPredefinedBlockList_[sName], uiMin, uiMax);

                                    if (!pAdded)
                                        return false;

                                    if (lAttribs.size() != 0)
                                    {
                                        Warning(sDefFileName_+":"+uiLineNbr_,
                                            "Can't add attributes to a loaded pre-defined block."
                                        );
                                    }
                                }
                                else
                                {
                                    Error(sDefFileName_+":"+uiLineNbr_,
                                        "\""+sName+"\" has not (yet?) been pre-defined."
                                    );
                                    return false;
                                }
                            }
                            else
                            {
                                if (!MAPFIND(sName, lPredefinedBlockList_))
                                {
                                    if (bRadio)
                                        pActual = pParent->CreateRadioDefBlock(sName);
                                    else
                                        pActual = pParent->CreateDefBlock(sName, uiMin, uiMax);

                                    if (!pActual)
                                        return false;

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
                                            sDefault.Trim('"');
                                        }

                                        AttrType mType = ATTR_TYPE_STRING;
                                        vector<s_str> lCommands = sAttr.Cut(":");
                                        sAttr = lCommands.back();
                                        lCommands.pop_back();
                                        vector<s_str>::iterator iterCommand;
                                        foreach (iterCommand, lCommands)
                                        {
                                            s_str sLetterCode = s_str(iterCommand->Get(0));
                                            if (sLetterCode == "s")
                                            {
                                                mType = ATTR_TYPE_STRING;
                                            }
                                            else if (sLetterCode == "n")
                                            {
                                                mType = ATTR_TYPE_NUMBER;
                                            }
                                            else if (sLetterCode == "b")
                                            {
                                                mType = ATTR_TYPE_BOOL;
                                            }
                                            else
                                            {
                                                Warning(sDefFileName_+":"+uiLineNbr_,
                                                    "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                                );
                                            }
                                        }

                                        if (!pActual->Add(Attribute(sAttr, bOptional, sDefault, mType)))
                                            return false;
                                    }
                                    pActual = pParent;
                                }
                                else
                                {
                                    Error(sDefFileName_+":"+uiLineNbr_,
                                        "Defining a new block named \""+sName+"\", which is the name "
                                        "of a pre-defined block."
                                    );
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            if (bPreDefining)
                            {
                                // Pre-definition
                                if (!sParent.IsEmpty())
                                {
                                    // Inheritance
                                    if (MAPFIND(sParent, lPredefinedBlockList_))
                                    {
                                        lPredefinedBlockList_[sName] = lPredefinedBlockList_[sParent];
                                        lPredefinedBlockList_[sParent].AddDerivated(sName);
                                    }
                                    else
                                    {
                                        Error(sDefFileName_+":"+uiLineNbr_,
                                            "\""+sParent+"\" has not (yet?) been pre-defined and cannot be "
                                            "inherited."
                                        );
                                    }
                                }
                                pActual = &lPredefinedBlockList_[sName];
                                pActual->SetDocument(this);
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
                                        sDefault.Trim('"');
                                    }

                                    AttrType mType = ATTR_TYPE_STRING;
                                    vector<s_str> lCommands = sAttr.Cut(":");
                                    sAttr = lCommands.back();
                                    lCommands.pop_back();
                                    vector<s_str>::iterator iterCommand;
                                    foreach (iterCommand, lCommands)
                                    {
                                        s_str sLetterCode = s_str(iterCommand->Get(0));
                                        if (sLetterCode == "s")
                                        {
                                            mType = ATTR_TYPE_STRING;
                                        }
                                        else if (sLetterCode == "n")
                                        {
                                            mType = ATTR_TYPE_NUMBER;
                                        }
                                        else if (sLetterCode == "b")
                                        {
                                            mType = ATTR_TYPE_BOOL;
                                        }
                                        else
                                        {
                                            Warning(sDefFileName_+":"+uiLineNbr_,
                                                "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                            );
                                        }
                                    }

                                    if (!pActual->Add(Attribute(sAttr, bOptional, sDefault, mType)))
                                        return false;
                                }
                            }
                            else
                            {
                                // Main block
                                if (!MAPFIND(sName, lPredefinedBlockList_))
                                {
                                    pActual = &mMainBlock_;
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
                                            sDefault.Trim('"');
                                        }

                                        AttrType mType = ATTR_TYPE_STRING;
                                        vector<s_str> lCommands = sAttr.Cut(":");
                                        sAttr = lCommands.back();
                                        lCommands.pop_back();
                                        vector<s_str>::iterator iterCommand;
                                        foreach (iterCommand, lCommands)
                                        {
                                            s_str sLetterCode = s_str(iterCommand->Get(0));
                                            if (sLetterCode == "s")
                                            {
                                                mType = ATTR_TYPE_STRING;
                                            }
                                            else if (sLetterCode == "n")
                                            {
                                                mType = ATTR_TYPE_NUMBER;
                                            }
                                            else if (sLetterCode == "b")
                                            {
                                                mType = ATTR_TYPE_BOOL;
                                            }
                                            else
                                            {
                                                Warning(sDefFileName_+":"+uiLineNbr_,
                                                    "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                                );
                                            }
                                        }

                                        if (!pActual->Add(Attribute(sAttr, bOptional, sDefault, mType)))
                                            return false;
                                    }
                                }
                                else
                                {
                                    Error(sDefFileName_+":"+uiLineNbr_,
                                        "Defining a new block named \""+sName+"\", which is the name "
                                        "of a pre-defined block."
                                    );
                                    return false;
                                }

                                return true;
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
                                if (pActual == &mMainBlock_)
                                {
                                    // It's the main block's end tag
                                    return true;
                                }
                                else
                                {
                                    // It's the end tag of a pre-defined block
                                }
                            }
                        }
                        else
                        {
                            Error(sDefFileName_+":"+uiLineNbr_,
                                "Wrong end tag : \"</"+sLine+">\". Expected : \"</"+pActual->GetName()+">\"."
                            );
                            return false;
                        }
                    }
                    else if (sLine.Find("<!--"))
                    {
                        // A comment, skip it
                    }
                    else
                    {
                        // Multiline block
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut(">").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();
                        s_uint uiMin = 0;
                        s_uint uiMax = s_uint::INF;
                        s_bool bPreDefining = false;
                        s_bool bRadio = false;
                        s_str sParent;

                        vector<s_str> lCommands = sName.Cut(":");
                        sName = lCommands.back();
                        lCommands.pop_back();
                        vector<s_str>::iterator iterCommand;
                        foreach (iterCommand, lCommands)
                        {
                            s_str sLetterCode = s_str((*iterCommand)[0]);
                            if (sLetterCode == "d")
                            {
                                // Pre-definintion
                                if (pParent)
                                {
                                    Error(sDefFileName_+":"+uiLineNbr_,
                                        "Can't pre-define blocks outside root level (\'d\' command forbidden)."
                                    );
                                    return false;
                                }
                                else
                                {
                                    bPreDefining = true;

                                    s_uint uiStart = iterCommand->Find("[");
                                    s_uint uiEnd = iterCommand->Find("]");
                                    if (uiStart && uiEnd)
                                    {
                                        // Inheritance
                                        sParent = iterCommand->ExtractRange(uiStart+1, uiEnd);
                                    }
                                }
                            }
                            else if (sLetterCode == "l")
                            {
                                // Load pre-definition
                                Error(sDefFileName_+":"+uiLineNbr_,
                                    "Can't load a pre-defined block using a multiline block (\'l\' command forbidden)."
                                );
                                return false;
                            }
                            else if (sLetterCode == "n")
                            {
                                // Min/max count
                                s_uint uiStart = iterCommand->Find("[");
                                s_uint uiEnd = iterCommand->Find("]");
                                if (uiStart && uiEnd)
                                {
                                    s_str sParams = iterCommand->ExtractRange(uiStart+1, uiEnd);
                                    if (sParams.Find(","))
                                    {
                                        vector<s_str> lMinMax = sParams.Cut(",");
                                        s_str sMin = lMinMax.front();
                                        s_str sMax = lMinMax.back();
                                        if (sMin != ".")
                                            uiMin = s_uint(sMin);
                                        if (sMax != ".")
                                            uiMax = s_uint(sMax);
                                    }
                                    else
                                    {
                                        if (sParams == "*")
                                            bRadio = true;
                                        else
                                        {
                                            Warning(sDefFileName_+":"+uiLineNbr_,
                                                "Unknown param : \""+sParams+"\" for \'n\' command. Skipped."
                                            );
                                        }
                                    }
                                }
                                else
                                {
                                    Warning(sDefFileName_+":"+uiLineNbr_,
                                        "\'n\' command requires some parameters. Correct synthax is : \"n[params]\". Skipped."
                                    );
                                }
                            }
                            else
                            {
                                Warning(sDefFileName_+":"+uiLineNbr_,
                                    "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                );
                            }
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
                            if (!MAPFIND(sName, lPredefinedBlockList_))
                            {
                                if (bRadio)
                                    pActual = pParent->CreateRadioDefBlock(sName);
                                else
                                    pActual = pParent->CreateDefBlock(sName, uiMin, uiMax);

                                if (!pActual)
                                    return false;

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
                                        sDefault.Trim('"');
                                    }

                                    AttrType mType = ATTR_TYPE_STRING;
                                    vector<s_str> lCommands = sAttr.Cut(":");
                                    sAttr = lCommands.back();
                                    lCommands.pop_back();
                                    vector<s_str>::iterator iterCommand;
                                    foreach (iterCommand, lCommands)
                                    {
                                        s_str sLetterCode = s_str(iterCommand->Get(0));
                                        if (sLetterCode == "s")
                                        {
                                            mType = ATTR_TYPE_STRING;
                                        }
                                        else if (sLetterCode == "n")
                                        {
                                            mType = ATTR_TYPE_NUMBER;
                                        }
                                        else if (sLetterCode == "b")
                                        {
                                            mType = ATTR_TYPE_BOOL;
                                        }
                                        else
                                        {
                                            Warning(sDefFileName_+":"+uiLineNbr_,
                                                "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                            );
                                        }
                                    }

                                    if (!pActual->Add(Attribute(sAttr, bOptional, sDefault, mType)))
                                        return false;
                                }
                                pParent = pActual;
                            }
                            else
                            {
                                Error(sDefFileName_+":"+uiLineNbr_,
                                    "Defining a new block named \""+sName+"\", which is the name "
                                    "of a pre-defined block."
                                );
                                return false;
                            }
                        }
                        else
                        {

                            if (bPreDefining)
                            {
                                // Pre-definition
                                if (!sParent.IsEmpty())
                                {
                                    // Inheritance
                                    if (MAPFIND(sParent, lPredefinedBlockList_))
                                    {
                                        lPredefinedBlockList_[sName] = lPredefinedBlockList_[sParent];
                                        lPredefinedBlockList_[sParent].AddDerivated(sName);
                                    }
                                    else
                                    {
                                        Error(sDefFileName_+":"+uiLineNbr_,
                                            "\""+sParent+"\" has not (yet?) been pre-defined and cannot be "
                                            "inherited."
                                        );
                                    }
                                }
                                pActual = &lPredefinedBlockList_[sName];
                                pActual->SetDocument(this);
                            }
                            else
                            {
                                // Main block
                                pActual = &mMainBlock_;
                            }

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
                                    sDefault.Trim('"');
                                }

                                AttrType mType = ATTR_TYPE_STRING;
                                vector<s_str> lCommands = sAttr.Cut(":");
                                sAttr = lCommands.back();
                                lCommands.pop_back();
                                vector<s_str>::iterator iterCommand;
                                foreach (iterCommand, lCommands)
                                {
                                    s_str sLetterCode = s_str(iterCommand->Get(0));
                                    if (sLetterCode == "s")
                                    {
                                        mType = ATTR_TYPE_STRING;
                                    }
                                    else if (sLetterCode == "n")
                                    {
                                        mType = ATTR_TYPE_NUMBER;
                                    }
                                    else if (sLetterCode == "b")
                                    {
                                        mType = ATTR_TYPE_BOOL;
                                    }
                                    else
                                    {
                                        Warning(sDefFileName_+":"+uiLineNbr_,
                                            "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                                        );
                                    }
                                }

                                if (!pActual->Add(Attribute(sAttr, bOptional, sDefault, mType)))
                                    return false;
                            }
                            pParent = pActual;
                        }
                    }
                }
                else
                {
                    Error(sDefFileName_+":"+uiLineNbr_, "Invalid line :\n    "+sLine);
                    return false;
                }
            }
            uiLineNbr_++;
        }
    }
    else
        return false;

    return true;
}

s_bool Document::Check()
{
    if (bValid_)
    {
        sActualFileName_ = sFileName_;
        File mFile(sFileName_, FILE_I);
        uiLineNbr_ = 1u;

        s_ptr<Block> pActual = &mMainBlock_;
        s_ptr<Block> pParent;
        s_bool       bOpened;
        s_bool       bValue;

        while (mFile.IsValid())
        {
            s_str sLine = mFile.GetLine();

            if (!sLine.IsEmpty(true))
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

s_ptr<Block> Document::GetPredefinedBlock( const s_str& sName )
{
    if (MAPFIND(sName, lPredefinedBlockList_))
    {
        return &lPredefinedBlockList_[sName];
    }
    else
    {
        return NULL;
    }
}
