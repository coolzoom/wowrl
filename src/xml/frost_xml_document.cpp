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
    s_uint uiPos = sLine.FindPos("<");
    if (uiPos.IsValid())
    {
        sLine.EraseFromStart(uiPos);
        uiPos = sLine.FindPos(">");
        if (uiPos.IsValid())
        {
            sLine.EraseFromEnd(sLine.Size()-uiPos-1);
            return true;
        }
    }

    return false;
}

s_bool Document::ReadPreDefCommands_( s_str& sName, s_str& sParent, s_uint& uiMin, s_uint& uiMax, s_bool& bPreDefining, s_bool& bLoad, s_bool& bRadio, const s_bool& bMultiline, s_ptr<Block> pParent )
{
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

                s_uint uiStart = iterCommand->FindPos("[");
                s_uint uiEnd = iterCommand->FindPos("]");
                if (uiStart.IsValid() && uiEnd.IsValid())
                {
                    // Inheritance
                    sParent = iterCommand->ExtractRange(uiStart+1, uiEnd);
                }
            }
        }
        else if (sLetterCode == "l")
        {
            // Load pre-definition
            if (bMultiline)
            {
                Error(sDefFileName_+":"+uiLineNbr_,
                    "Can't load a pre-defined block using a multiline block (\'l\' command forbidden)."
                );
                return false;
            }
            else
            {
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
        }
        else if (sLetterCode == "n")
        {
            // Min/max count
            s_uint uiStart = iterCommand->FindPos("[");
            s_uint uiEnd = iterCommand->FindPos("]");
            if (uiStart.IsValid() && uiEnd.IsValid())
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

    return true;
}

s_bool Document::ParseArguments_( s_ptr<Block> pActual, const vector<s_str>& lAttribs )
{
    vector<s_str>::const_iterator iterAttr;
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
            s_str sLetterCode = s_str((*iterCommand)[0]);
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

    return true;
}

s_bool Document::LoadDefinition_()
{
    if (bValid_)
    {
        sActualFileName_ = sDefFileName_;
        File mFile(sDefFileName_, FILE_I);

        s_ptr<Block> pActual;
        s_ptr<Block> pParent;
        s_bool bComment;
        s_str sCommentTag;
        s_uint uiCommentTagCount;

        while (mFile.IsValid())
        {
            s_str sLine = mFile.GetLine();

            if (!sLine.IsEmpty(true))
            {
                if (CheckLineSynthax_(sLine))
                {
                    // It's a tag
                    if (sLine.Find("<!"))
                    {
                        // Comment
                        if (!sLine.Find("/>"))
                        {
                            bComment = true;
                            uiCommentTagCount = 1;
                            sLine.EraseFromStart(2);
                            sCommentTag = sLine.Cut(">", 1).front();
                            sCommentTag = sCommentTag.Cut(" ", 1).front();
                        }
                    }
                    else if (sLine.Find("<--"))
                    {
                        // Multi-line comment
                        s_uint uiStartLineNbr = uiLineNbr_;
                        if (!sLine.Find("-->"))
                        {
                            while (mFile.IsValid())
                            {
                                sLine = mFile.GetLine();
                                if (sLine.Find("-->"))
                                    break;

                                uiLineNbr_++;
                            }

                            if (!mFile.IsValid())
                            {
                                Error(sFileName_+":"+uiStartLineNbr,
                                    "Multi-line comment not ended."
                                );
                                bValid_ = false;
                                break;
                            }
                        }
                    }
                    else if (sLine.Find("/>") && !bComment)
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
                        ReadPreDefCommands_(
                            sName, sParent,
                            uiMin, uiMax,
                            bPreDefining,
                            bLoad, bRadio,
                            false, pParent
                        );

                        // Prepare attributes
                        vector<s_str> lAttribs;
                        if (lWords.size() > 1)
                        {
                            s_str sAttribs = lWords.back();
                            sAttribs.Replace(" =", "=");
                            sAttribs.Replace("= ", "=");

                            s_str::iterator iterStr;
                            s_str sAttr;
                            s_bool bString = false;
                            foreach (iterStr, sAttribs)
                            {
                                if (*iterStr == '"')
                                {
                                    sAttr += *iterStr;
                                    bString = !bString;
                                }
                                else if (*iterStr == ' ')
                                {
                                    if (!bString)
                                    {
                                        if (!sAttr.IsEmpty())
                                            lAttribs.push_back(sAttr);
                                        sAttr = "";
                                    }
                                    else
                                        sAttr += *iterStr;
                                }
                                else
                                {
                                    sAttr += *iterStr;
                                }
                            }

                            if (!sAttr.IsEmpty())
                                lAttribs.push_back(sAttr);
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

                                    if (!ParseArguments_(pActual, lAttribs))
                                        return false;

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

                                if (!ParseArguments_(pActual, lAttribs))
                                    return false;
                            }
                            else
                            {
                                // Main block
                                if (!MAPFIND(sName, lPredefinedBlockList_))
                                {
                                    pActual = &mMainBlock_;
                                    pActual->SetName(sName);

                                    if (!ParseArguments_(pActual, lAttribs))
                                        return false;
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
                        if (bComment)
                        {
                            sLine.EraseFromStart(2);
                            sLine = sLine.Cut(">").front();
                            sLine.Trim(' ');
                            if (sLine == sCommentTag)
                            {
                                uiCommentTagCount--;
                                if (uiCommentTagCount == 0)
                                    bComment = false;
                            }
                        }
                        else
                        {
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
                    }
                    else
                    {
                        // Multiline block
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut(">").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();

                        if (bComment)
                        {
                            if (sName == sCommentTag)
                                uiCommentTagCount++;
                        }
                        else
                        {
                            s_uint uiMin = 0;
                            s_uint uiMax = s_uint::INF;
                            s_bool bPreDefining = false;
                            s_bool bRadio = false;
                            s_bool bLoad = false;
                            s_str sParent;

                            ReadPreDefCommands_(
                                sName, sParent,
                                uiMin, uiMax,
                                bPreDefining,
                                bLoad, bRadio,
                                true, pParent
                            );

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

                                    if (!ParseArguments_(pActual, lAttribs))
                                        return false;

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

                                if (!ParseArguments_(pActual, lAttribs))
                                    return false;

                                pParent = pActual;
                            }
                        }
                    }
                }
                else
                {
                    Error(sDefFileName_+":"+uiLineNbr_,
                        "Invalid line :\n    "+sLine
                    );
                    return false;
                }
            }
            uiLineNbr_++;
        }

        if (pParent)
        {
            while (pParent)
            {
                Error(sDefFileName_,
                    "<"+pParent->GetName()+"> block has not been closed !"
                );
                pParent = pParent->GetParent();
            }
            return false;
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
        s_bool       bComment;
        s_str        sCommentTag;
        s_uint       uiCommentTagCount;

        while (mFile.IsValid())
        {
            s_str sLine = mFile.GetLine();

            if (!sLine.IsEmpty(true))
            {
                if (CheckLineSynthax_(sLine))
                {
                    // It's a tag
                    if (sLine.Find("<!"))
                    {
                        // Comment
                        if (!sLine.Find("/>"))
                        {
                            bComment = true;
                            uiCommentTagCount = 1;
                            sLine.EraseFromStart(2);
                            sCommentTag = sLine.Cut(">", 1).front();
                            sCommentTag = sCommentTag.Cut(" ", 1).front();
                        }
                    }
                    else if (sLine.Find("<--"))
                    {
                        // Multi-line comment
                        s_uint uiStartLineNbr = uiLineNbr_;
                        if (!sLine.Find("-->"))
                        {
                            while (mFile.IsValid())
                            {
                                sLine = mFile.GetLine();
                                if (sLine.Find("-->"))
                                    break;

                                uiLineNbr_++;
                            }

                            if (!mFile.IsValid())
                            {
                                Error(sFileName_+":"+uiStartLineNbr,
                                    "Multi-line comment not ended."
                                );
                                bValid_ = false;
                                break;
                            }
                        }
                    }
                    else if (sLine.Find("/>") && !bValue && !bComment)
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
                        if (bComment)
                        {
                            sLine.EraseFromStart(2);
                            sLine = sLine.Cut(">").front();
                            sLine.Trim(' ');
                            if (sLine == sCommentTag)
                            {
                                uiCommentTagCount--;
                                if (uiCommentTagCount == 0)
                                    bComment = false;
                            }
                        }
                        else
                        {
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
                    }
                    else if (!bValue)
                    {
                        // Multiline block
                        bOpened = true;
                        sLine.EraseFromStart(1);
                        sLine = sLine.Cut(">").front();
                        vector<s_str> lWords = sLine.Cut(" ", 1);
                        s_str sName = lWords.front();

                        if (bComment)
                        {
                            if (sName == sCommentTag)
                                uiCommentTagCount++;
                        }
                        else
                        {
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
                    }
                    else if (!bComment)
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

        if (pParent)
        {
            while (pParent)
            {
                Error(sFileName_,
                    "<"+pParent->GetName()+"> block has not been closed !"
                );
                pParent = pParent->GetParent();
            }
            bValid_ = false;
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
