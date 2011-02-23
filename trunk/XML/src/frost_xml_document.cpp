/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          XML::Document source          */
/*                                        */
/*                                        */

#include "frost_xml_document.h"
#include "frost_xml_block.h"

#include <frost_utils_file.h>
#include <frost_utils_exception.h>
#include <frost_utils_log.h>
#include <frost_utils_stdhelper.h>

using namespace std;
using namespace Frost;
using namespace Frost::XML;

const s_str Document::CLASS_NAME = "XML::Document";

Document::Document( const s_str& sDefFileName ) :
    sDefFileName_(sDefFileName)
{
    mXMLState_.SetDocument(this);
    mDefState_.SetDocument(this);

    if (File::Exists(sDefFileName_))
    {
        mMainBlock_.SetDocument(this);
        bValid_ = true;

        try
        {
            LoadDefinition_();
        }
        catch (const Exception& e)
        {
            Error("", e.GetDescription());
            bValid_ = false;
        }
    }
    else
    {
        Error(CLASS_NAME, "Can't find \""+sDefFileName_+"\".");
        bValid_ = false;
    }
}

Document::Document( const s_str& sFileName, const s_str& sDefFileName ) :
    sFileName_(sFileName), sDefFileName_(sDefFileName)
{
    mXMLState_.SetDocument(this);
    mDefState_.SetDocument(this);
    if (File::Exists(sFileName_))
    {
        if (File::Exists(sDefFileName_))
        {
            mMainBlock_.SetDocument(this);
            bValid_ = true;

            try
            {
                LoadDefinition_();
            }
            catch (const Exception& e)
            {
                Error("", e.GetDescription());
                bValid_ = false;
            }
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

void Document::SetFileName( const s_str& sFileName )
{
    if (File::Exists(sFileName))
    {
        sFileName_ = sFileName;
    }
    else
    {
        Error(CLASS_NAME, "Can't find \""+sFileName+"\".");
        bValid_ = false;
    }
}

void Document::SetSourceString( const s_str& sString )
{
    sSourceString_ = sString;
}

s_ptr<Block> Document::GetMainBlock()
{
    return &mMainBlock_;
}

const s_str& Document::GetCurrentFileName() const
{
    return sCurrentFileName_;
}

const s_uint& Document::GetCurrentLineNbr() const
{
    return uiCurrentLineNbr_;
}

s_str Document::GetCurrentLocation() const
{
    return sCurrentFileName_+":"+uiCurrentLineNbr_;
}

void Document::LoadDefinition_()
{
    // Open de def file
    File mFile(sDefFileName_, File::I);
    sCurrentFileName_ = sDefFileName_;
    uiCurrentLineNbr_ = 0u;
    s_str sLine;
    pState_ = &mDefState_;

    // Start parsing line by line
    while (mFile.IsValid() && bValid_)
    {
        sLine += mFile.GetLine();
        ++uiCurrentLineNbr_;

        if (!sLine.IsEmpty(true))
        {
            // Read all tags on this line
            ReadTags_(sLine);
        }
    }
}

s_ctnr<s_str> ReadPreProcessorCommands( const s_str& sCommands )
{
    s_ctnr<s_str> lPreProcessorCommands;
    if (sCommands != "")
    {
        lPreProcessorCommands = sCommands.Cut(",");
        s_ctnr<s_str>::iterator iter;
        foreach (iter, lPreProcessorCommands)
        {
            iter->Trim(' ');
        }
    }
    return lPreProcessorCommands;
}

void Document::ReadOpeningTag_( s_str& sTagContent )
{
    if (sTagContent.StartsWith("#")) // Preprocessor
    {
        ++uiPreProcessorCount_;
        if (!bPreProcessor_)
        {
            // Extract the required identifiers
            s_uint uiStart = sTagContent.FindPos("[");
            s_uint uiEnd   = sTagContent.FindPos("]");
            if (uiStart.IsValid() && uiEnd.IsValid() && (uiStart < uiEnd) && (uiEnd-uiStart > 1))
            {
                s_ctnr<s_str> lIDList = sTagContent.ExtractRange(uiStart+1, uiEnd).Cut(",");
                s_ctnr<s_str>::iterator iterID;
                foreach (iterID, lIDList)
                {
                    iterID->Trim(' ');
                    s_str sCommand = *iterID;
                    if (iterID->StartsWith("!"))
                        sCommand.EraseFromStart(1);

                    // Compare with the preprocessor commands given by the user
                    if (lPreProcessorCommands_.Find(sCommand) == iterID->StartsWith("!"))
                    {
                        // Skip all the following code until the proper ending tag is found
                        bPreProcessor_ = true;
                        uiSkippedPreProcessorCount_ = 1u;
                    }
                }
            }
            else
            {
                Warning(sCurrentFileName_+":"+uiCurrentLineNbr_,
                    "PreProcessor command with no argument is always 'true' "
                    "(expected \"<#[some parameter]>\")."
                );
            }
        }
        else
        {
            ++uiSkippedPreProcessorCount_;
        }
    }
    else if (!bPreProcessor_)
    {
        if (sTagContent.StartsWith("!")) // Smart comment
        {
            if (!bMultilineComment_ && !bSmartComment_)
            {
                bSmartComment_ = true;
                // Remove the exclamation mark
                sTagContent.EraseFromStart(1);
                // Extract the commented tag's name
                sSmartCommentTag_ = sTagContent.Cut(" ", 1).Front();
                uiSmartCommentCount_ = 1u;
            }
        }
        else // Regular opening tag
        {
            s_str sName = pState_->ReadTagName(sTagContent);
            if (bSmartComment_)
            {
                if (sName == sSmartCommentTag_)
                    ++uiSmartCommentCount_;
            }
            else
            {
                try
                {
                    pState_->ReadOpeningTag(sTagContent);
                }
                catch (const Exception& e)
                {
                    if (pState_->GetID() == State::STATE_DEF)
                    {
                        // The definition stage throws exceptions
                        // that are catched in the constructor.
                        throw;
                    }
                    else
                    {
                        // The loading stage just prints things in
                        // the log file.
                        Error("", e.GetDescription());
                    }
                }
            }
        }
    }
}

void Document::ReadSingleTag_( s_str& sTagContent )
{
    // Remove the trailling slash
    sTagContent.EraseFromEnd(1);

    if (sTagContent.StartsWith("#")) // PreProcessor
    {
        Warning(sCurrentFileName_+":"+uiCurrentLineNbr_,
            "Single block PreProcessor tag is useless. Skipped."
        );
    }
    else if (!bPreProcessor_)
    {
        if (sTagContent.StartsWith("!")) // Smart comment
        {
            // Smart comment on a single tag, just ignore the tag
        }
        else if (!bSmartComment_) // Regular single tag
        {
            try
            {
                pState_->ReadSingleTag(sTagContent);
            }
            catch (const Exception& e)
            {
                if (pState_->GetID() == State::STATE_DEF)
                {
                    // The definition stage throws exceptions
                    // that are catched in the constructor.
                    throw;
                }
                else
                {
                    // The loading stage just prints things in
                    // the log file.
                    Error("", e.GetDescription());
                }
            }
        }
    }
}

void Document::ReadEndingTag_( s_str& sTagContent )
{
    // Remove the starting slash
    sTagContent.EraseFromStart(1);

    if (sTagContent.StartsWith("#")) // PreProcessor
    {
        if (uiPreProcessorCount_ != 0)
        {
            --uiPreProcessorCount_;
            if (bPreProcessor_)
            {
                --uiSkippedPreProcessorCount_;
                if (uiSkippedPreProcessorCount_ == 0)
                {
                    // End of the skiped code
                    bPreProcessor_ = false;
                }
            }
        }
        else
        {
            Warning(sCurrentFileName_+":"+uiCurrentLineNbr_,
                "PreProcessor end tag in excess. Skipped."
            );
        }
    }
    else if (!bPreProcessor_) // Regular end tag
    {
        s_str sName = pState_->ReadTagName(sTagContent);
        if (bSmartComment_)
        {
            if (sName == sSmartCommentTag_)
            {
                // It has the same name as the current "smart
                // commented" block
                --uiSmartCommentCount_;
                if (uiSmartCommentCount_ == 0)
                    bSmartComment_ = false;
            }
        }
        else
        {
            try
            {
                pState_->ReadEndingTag(sTagContent);
            }
            catch (const Exception& e)
            {
                if (pState_->GetID() == State::STATE_DEF)
                {
                    // The definition stage throws exceptions
                    // that are catched in the constructor.
                    throw;
                }
                else
                {
                    // The loading stage just prints things in
                    // the log file.
                    Error("", e.GetDescription());
                }
            }
        }
    }
}

void Document::ReadTags_( s_str& sLine )
{
    if (!bMultilineComment_)
    {
        // Search for multiline comments
        s_uint uiFirst = sLine.FindPos("<--");
        s_uint uiSecond = sLine.FindPos("-->");
        if (uiSecond < uiFirst)
        {
            // Check there are no misplaced comment end token
            Warning(sCurrentFileName_+":"+uiCurrentLineNbr_,
                "Multiline comment end token in excess (\"-->\"). Ignored."
            );
            s_str sTemp = sLine.ExtractRange(0, uiFirst);
            sLine.EraseRange(0, uiFirst);
            sTemp.Replace("-->", "");
            sLine = sTemp + sLine;
            uiFirst = sLine.FindPos("<--");
            uiSecond = sLine.FindPos("-->");
        }

        while (uiFirst.IsValid())
        {
            // Multiline comment started
            uiSecond = sLine.FindPos("-->", uiFirst);
            if (uiSecond.IsValid())
            {
                // It ends on the same line
                sLine.EraseRange(uiFirst, uiSecond+3);
            }
            else
            {
                // It ends later (hopefuly)
                bMultilineComment_ = true;
                break;
            }

            uiFirst = sLine.FindPos("<--");
        }

        if (!bMultilineComment_)
        {
            // Check there are no misplaced comment end token
            if (sLine.Find("-->"))
            {
                Warning(sCurrentFileName_+":"+uiCurrentLineNbr_,
                    "Multiline comment end token in excess (\"-->\"). Ignored."
                );
                sLine.Replace("-->", "");
            }

            // Parse the tag as usual
            uiFirst = sLine.FindPos("<");
            uiSecond = sLine.FindPos(">", uiFirst);
            while (uiFirst.IsValid() && uiSecond.IsValid() && bValid_)
            {
                pState_->AddContent(sLine.ExtractRange(0, uiFirst));
                s_str sTagContent = sLine.ExtractRange(uiFirst+1, uiSecond);

                if (sTagContent.StartsWith("/"))    // Ending tag
                {
                    ReadEndingTag_(sTagContent);
                    pState_->ResetContent();
                }
                else if (sTagContent.EndsWith("/")) // Single tag
                {
                    ReadSingleTag_(sTagContent);
                }
                else                                // Opening tag
                {
                    ReadOpeningTag_(sTagContent);
                }

                sLine.EraseFromStart(uiSecond+1);
                uiFirst = sLine.FindPos("<");
                uiSecond = sLine.FindPos(">", uiFirst);
            }
        }
        else
        {
            uiMultilineCommentCount_ = sLine.CountOccurences("<--")+1;
        }
    }
    else
    {
        // Search the line for an ending token
        s_uint uiFirst = sLine.FindPos("<--");
        s_uint uiSecond = sLine.FindPos("-->");

        while (uiSecond.IsValid())
        {
            if ( uiFirst.IsValid() && (uiFirst < uiSecond) )
            {
                uiMultilineCommentCount_ += sLine.ExtractRange(uiFirst, uiSecond).CountOccurences("<--");
            }
            sLine.EraseFromStart(uiSecond+3);

            --uiMultilineCommentCount_;
            if (uiMultilineCommentCount_ == 0)
            {
                bMultilineComment_ = false;
                ReadTags_(sLine);
                break;
            }

            uiFirst = sLine.FindPos("<--");
            uiSecond = sLine.FindPos("-->");
        }

        if (!uiSecond.IsValid())
        {
            uiMultilineCommentCount_ += sLine.CountOccurences("<--");
        }
    }
}

s_bool Document::Check( const s_str& sPreProcCommands )
{
    if (!bValid_)
        return false;

    // Parse preprocessor commands given by the user
    lPreProcessorCommands_ = ReadPreProcessorCommands(sPreProcCommands);

    if (!sFileName_.IsEmpty())
    {
        // Open the XML file
        File mFile(sFileName_, File::I);
        sCurrentFileName_ = sFileName_;
        uiCurrentLineNbr_ = 0u;
        s_str sLine;
        pState_ = &mXMLState_;
        pState_->SetCurrentBlock(&mMainBlock_);

        // Start parsing line by line
        while (mFile.IsValid() && bValid_)
        {
            sLine += mFile.GetLine() + "\n";
            ++uiCurrentLineNbr_;

            if (!sLine.IsEmpty(true))
            {
                // Read all tags on this line
                ReadTags_(sLine);
            }
        }
    }
    else
    {
        // Read the source string
        sCurrentFileName_ = "source string";
        uiCurrentLineNbr_ = 0u;
        s_str sLine;
        pState_ = &mXMLState_;
        pState_->SetCurrentBlock(&mMainBlock_);

        s_ctnr<s_str> lLines = sSourceString_.CutEach("\n");
        s_ctnr<s_str>::iterator iterLine;

        // Start parsing line by line
        foreach (iterLine, lLines)
        {
            sLine += (*iterLine) + "\n";
            ++uiCurrentLineNbr_;

            if (!sLine.IsEmpty(true))
            {
                // Read all tags on this line
                ReadTags_(sLine);
            }

            if (!bValid_)
                break;
        }
    }

    return bValid_;
}

void Document::CreatePredefinedBlock( const s_str& sName, const s_str& sInheritance )
{
    if (sInheritance.IsEmpty())
        lPredefinedBlockList_[sName] = Block();
    else
        lPredefinedBlockList_[sName] = lPredefinedBlockList_[sInheritance];
}

s_ptr<Block> Document::GetPredefinedBlock( const s_str& sName )
{
    s_map<s_str, Block>::iterator iter = lPredefinedBlockList_.Get(sName);
    if (iter != lPredefinedBlockList_.End())
    {
        return &iter->second;
    }
    else
    {
        return nullptr;
    }
}

void Document::SetInvalid()
{
    bValid_ = false;
}

Document::State::State()
{
}

Document::State::~State()
{
}

void Document::State::SetDocument( s_ptr<Document> pDoc )
{
    pDoc_ = pDoc;
}

void Document::State::SetCurrentBlock( s_ptr<Block> pBlock )
{
    pCurrentBlock_ = pBlock;
}

void Document::State::SetCurrentParentBlock( s_ptr<Block> pParentBlock )
{
    pCurrentParentBlock_ = pParentBlock;
}

void Document::State::AddContent( const s_str& sContent )
{
    sBlockContent_ += sContent;
}

void Document::State::ResetContent()
{
    sBlockContent_ = "";
}

const Document::State::ID& Document::State::GetID() const
{
    return mID_;
}

Document::XMLState::XMLState() : State()
{
    mID_ = STATE_XML;
}

s_str Document::XMLState::ReadTagName( const s_str& sTagContent ) const
{
    return sTagContent.Cut(" ", 1).Front();
}

void Document::XMLState::ReadSingleTag( const s_str& sTagContent )
{
    s_ctnr<s_str> lWords = sTagContent.Cut(" ", 1);
    s_str sName = lWords.Front();

    s_str sAttributes;
    if (lWords.GetSize() > 1)
    {
        sAttributes = lWords.Back();
        sAttributes.Replace(" =", "=");
        sAttributes.Replace("= ", "=");
    }

    if (pCurrentParentBlock_)
    {
        if (pCurrentParentBlock_->CanHaveBlock(sName))
        {
            pCurrentBlock_ = pCurrentParentBlock_->CreateBlock(sName);
            if (!pCurrentBlock_->CheckAttributes(sAttributes))
                pDoc_->SetInvalid();
            if (!pCurrentBlock_->CheckBlocks())
                pDoc_->SetInvalid();
            pCurrentBlock_ = pCurrentParentBlock_;
            pCurrentParentBlock_->AddBlock();
        }
        else
        {
            pDoc_->SetInvalid();
            throw Exception(pDoc_->GetCurrentLocation(), "Unexpected content : \"<"+sName+">\".");
        }
    }
    else
    {
        // This is the main block
        if (sName == pDoc_->GetMainBlock()->GetName())
        {
            if (!pDoc_->GetMainBlock()->CheckAttributes(sAttributes))
                pDoc_->SetInvalid();
            if (!pDoc_->GetMainBlock()->CheckBlocks())
                pDoc_->SetInvalid();
        }
        else
        {
            pDoc_->SetInvalid();
            throw Exception(pDoc_->GetCurrentLocation(),
                "Wrong main block : \"<"+sName+">\". Expected : \"<"+pDoc_->GetMainBlock()->GetName()+">\"."
            );
        }
    }
}

void Document::XMLState::ReadEndingTag( const s_str& sTagContent )
{
    s_ctnr<s_str> lWords = sTagContent.Cut(" ", 1);
    s_str sName = lWords.Front();

    if (lWords.GetSize() > 1)
    {
        Warning(pDoc_->GetCurrentLocation(),
            "An end tag should only contain the closed block's name."
        );
    }

    if (sName == pCurrentBlock_->GetName())
    {
        // This is the expected end tag
        pCurrentBlock_->SetValue(sBlockContent_);
        pCurrentParentBlock_ = pCurrentBlock_->GetParent();
        if (pCurrentParentBlock_)
        {
            // It's a regular block's end tag
            if (!pCurrentBlock_->CheckBlocks())
                pDoc_->SetInvalid();
            pCurrentParentBlock_->AddBlock();
            pCurrentBlock_ = pCurrentParentBlock_;
        }
        else
        {
            // It's the main block's end tag
            if (!pCurrentBlock_->CheckBlocks())
                pDoc_->SetInvalid();
        }
    }
    else
    {
        pDoc_->SetInvalid();
        throw Exception(pDoc_->GetCurrentLocation(),
            "Wrong end tag : \"</"+sName+">\". Expected : \"</"+
            pCurrentBlock_->GetName()+">\"."
        );
    }
}

void Document::XMLState::ReadOpeningTag( const s_str& sTagContent )
{
    s_ctnr<s_str> lWords = sTagContent.Cut(" ", 1);
    s_str sName = lWords.Front();

    s_str sAttributes;
    if (lWords.GetSize() > 1)
    {
        sAttributes = lWords.Back();
        sAttributes.Replace(" =", "=");
        sAttributes.Replace("= ", "=");
    }

    if (pCurrentParentBlock_)
    {
        if (pCurrentParentBlock_->CanHaveBlock(sName))
        {
            pCurrentBlock_ = pCurrentParentBlock_->CreateBlock(sName);
            if (!pCurrentBlock_->CheckAttributes(sAttributes))
                pDoc_->SetInvalid();
            pCurrentParentBlock_ = pCurrentBlock_;
        }
        else
        {
            pDoc_->SetInvalid();
            throw Exception(pDoc_->GetCurrentLocation(), "Unexpected content : \"<"+sName+">\".");
        }
    }
    else
    {
        // This is the main block
        if (sName == pDoc_->GetMainBlock()->GetName())
        {
            if (!pDoc_->GetMainBlock()->CheckAttributes(sAttributes))
                pDoc_->SetInvalid();
            pCurrentParentBlock_ = pDoc_->GetMainBlock();
        }
        else
        {
            pDoc_->SetInvalid();
            throw Exception(pDoc_->GetCurrentLocation(),
                "Wrong main block : \"<"+sName+">\". Expected : \"<"+pDoc_->GetMainBlock()->GetName()+">\"."
            );
        }
    }
}

Document::DefState::DefState() : State()
{
    mID_ = STATE_DEF;
}

s_str Document::DefState::ReadTagName( const s_str& sTagContent ) const
{
    return sTagContent.Cut(" ", 1).Front().Cut(":").Back();
}

void Document::DefState::ReadPreDefCommands_( s_str& sName, s_str& sParent, s_uint& uiMin, s_uint& uiMax, s_bool& bCopy, s_bool& bPreDefining, s_bool& bLoad, s_uint& uiRadioGroup, const s_bool& bMultiline )
{
    s_ctnr<s_str> lCommands = sName.Cut(":");
    sName = lCommands.Back();
    lCommands.PopBack();
    s_ctnr<s_str>::iterator iterCommand;
    foreach (iterCommand, lCommands)
    {
        s_char sLetterCode = *iterCommand->Begin();
        if ((sLetterCode == 'd') || (sLetterCode == 'c'))
        {
            if (sLetterCode == 'c')
                bCopy = true;

            // Pre-definintion
            if ( pCurrentParentBlock_ && (sLetterCode == 'd') )
            {
                throw Exception(pDoc_->GetCurrentLocation(),
                    "Can't pre-define a block outside root level (nested \'d\' command forbidden)."
                );
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
        else if (sLetterCode == 'l')
        {
            // Load pre-definition
            if (bMultiline)
            {
                throw Exception(pDoc_->GetCurrentLocation(),
                    "Can't load a pre-defined block using a multiline block (\'l\' command forbidden)."
                );
            }
            else
            {
                if (!pCurrentParentBlock_)
                {
                    throw Exception(pDoc_->GetCurrentLocation(),
                        "Can't load a pre-defined block at root level (\'l\' command forbidden)."
                    );
                }
                else
                {
                    bLoad = true;
                }
            }
        }
        else if (sLetterCode == 'n')
        {
            // Min/max count
            s_uint uiStart = iterCommand->FindPos("[");
            s_uint uiEnd = iterCommand->FindPos("]");
            if (uiStart.IsValid() && uiEnd.IsValid())
            {
                s_str sParams = iterCommand->ExtractRange(uiStart+1, uiEnd);
                if (sParams.Find(","))
                {
                    s_ctnr<s_str> lMinMax = sParams.Cut(",");
                    s_str sMin = lMinMax.Front();
                    s_str sMax = lMinMax.Back();
                    if (sMin == "*")
                    {
                        uiRadioGroup = s_uint(sMax);
                    }
                    else
                    {
                        if (sMin != ".")
                            uiMin = s_uint(sMin);
                        if (sMax != ".")
                            uiMax = s_uint(sMax);
                    }
                }
                else
                {
                    if (sParams == "*")
                    {
                        uiRadioGroup = 0;
                    }
                    else if (sParams.IsNumber())
                    {
                        uiMax = uiMin = s_uint(sParams);
                    }
                    else
                    {
                        Warning(pDoc_->GetCurrentLocation(),
                            "Unknown param : \""+sParams+"\" for \'n\' command. Skipped."
                        );
                    }
                }
            }
            else
            {
                Warning(pDoc_->GetCurrentLocation(),
                    "\'n\' command requires some parameters. Correct synthax is : \"n[params]\". Skipped."
                );
            }
        }
        else
        {
            Warning(pDoc_->GetCurrentLocation(),
                "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
            );
        }
    }
}

void Document::DefState::ReadSingleTag( const s_str& sTagContent )
{
    s_ctnr<s_str> lWords = sTagContent.Cut(" ", 1);
    s_str sName = lWords.Front();

    s_uint uiMin = 0u;
    s_uint uiMax = s_uint::INF;
    s_bool bPreDefining = false;
    s_bool bCopy = false;
    s_uint uiRadioGroup = s_uint::NaN;
    s_bool bLoad = false;
    s_str sParent;

    // Read commands
    ReadPreDefCommands_(
        sName, sParent, uiMin, uiMax, bCopy, bPreDefining, bLoad, uiRadioGroup, false
    );

    if (uiRadioGroup.IsValid() && sName == ".")
    {
        pCurrentParentBlock_->SetRadioGroupOptional(uiRadioGroup);
        return;
    }

    // Prepare attributes
    s_ctnr<s_str> lAttributes;
    if (lWords.GetSize() > 1)
    {
        s_str sAttributes = lWords.Back();
        sAttributes.Replace(" =", "=");
        sAttributes.Replace("= ", "=");

        s_str::iterator iterStr;
        s_str sAttr;
        s_bool bString = false;
        foreach (iterStr, sAttributes)
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
                        lAttributes.PushBack(sAttr);
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
            lAttributes.PushBack(sAttr);
    }

    if (pCurrentParentBlock_)
    {
        if (bLoad)
        {
            if (pDoc_->GetPredefinedBlock(sName))
            {
                s_ptr<PredefinedBlock> pAdded;
                if (uiRadioGroup.IsValid())
                    pAdded = pCurrentParentBlock_->AddPredefinedRadioBlock(pDoc_->GetPredefinedBlock(sName), uiRadioGroup);
                else
                    pAdded = pCurrentParentBlock_->AddPredefinedBlock(pDoc_->GetPredefinedBlock(sName), uiMin, uiMax);

                if (lAttributes.GetSize() != 0)
                {
                    Warning(pDoc_->GetCurrentLocation(),
                        "Can't add attributes to a loaded pre-defined block."
                    );
                }
            }
            else
            {
                throw Exception(pDoc_->GetCurrentLocation(),
                    "\""+sName+"\" has not (yet?) been pre-defined."
                );
            }
        }
        else if (bCopy)
        {
            if (uiRadioGroup.IsValid())
                pCurrentBlock_ = pCurrentParentBlock_->CreateRadioDefBlock(sName, uiRadioGroup);
            else
                pCurrentBlock_ = pCurrentParentBlock_->CreateDefBlock(sName, uiMin, uiMax);

            // Pre-definition
            if (!sParent.IsEmpty())
            {
                // Inheritance
                if (pDoc_->GetPredefinedBlock(sParent))
                {
                    pCurrentBlock_->Copy(pDoc_->GetPredefinedBlock(sParent));
                }
                else
                {
                    Error(pDoc_->GetCurrentLocation(),
                        "\""+sParent+"\" has not (yet?) been pre-defined and cannot be "+
                        "copied."
                    );
                }
            }

            pCurrentBlock_->CheckAttributesDef(lAttributes);
            pCurrentBlock_ = pCurrentParentBlock_;
        }
        else
        {
            if (!pDoc_->GetPredefinedBlock(sName))
            {
                if (uiRadioGroup.IsValid())
                    pCurrentBlock_ = pCurrentParentBlock_->CreateRadioDefBlock(sName, uiRadioGroup);
                else
                    pCurrentBlock_ = pCurrentParentBlock_->CreateDefBlock(sName, uiMin, uiMax);

                pCurrentBlock_->CheckAttributesDef(lAttributes);
                pCurrentBlock_ = pCurrentParentBlock_;
            }
            else
            {
                throw Exception(pDoc_->GetCurrentLocation(),
                    "Defining a new block named \""+sName+"\", which is the name "
                    "of a pre-defined block (use the 'l' command to load it)."
                );
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
                if (pDoc_->GetPredefinedBlock(sParent))
                {
                    pDoc_->CreatePredefinedBlock(sName, sParent);
                    if (!bCopy)
                        pDoc_->GetPredefinedBlock(sParent)->AddDerivated(sName);
                }
                else
                {
                    throw Exception(pDoc_->GetCurrentLocation(),
                        "\""+sParent+"\" has not (yet?) been pre-defined and cannot be "+
                        (bCopy ? s_str("copied.") : s_str("inherited."))
                    );
                }
            }
            else
                pDoc_->CreatePredefinedBlock(sName);
            pCurrentBlock_ = pDoc_->GetPredefinedBlock(sName);
            pCurrentBlock_->SetDocument(pDoc_);
            pCurrentBlock_->SetName(sName);

            pCurrentBlock_->CheckAttributesDef(lAttributes);
        }
        else
        {
            // Main block
            if (!pDoc_->GetPredefinedBlock(sName))
            {
                pCurrentBlock_ = pDoc_->GetMainBlock();
                pCurrentBlock_->SetName(sName);

                pCurrentBlock_->CheckAttributesDef(lAttributes);
            }
            else
            {
                throw Exception(pDoc_->GetCurrentLocation(),
                    "Defining a new block named \""+sName+"\", which is the name "
                    "of a pre-defined block (use the 'l' command to load it)."
                );
            }
        }
    }
}

void Document::DefState::ReadEndingTag( const s_str& sTagContent )
{
    s_ctnr<s_str> lWords = sTagContent.Cut(" ", 1);
    s_str sName = lWords.Front();

    if (lWords.GetSize() > 1)
    {
        Warning(pDoc_->GetCurrentLocation(),
            "An end tag should only contain the closed block's name."
        );
    }

    if (sName == pCurrentBlock_->GetName())
    {
        // It's the expected end tag
        if (!sBlockContent_.IsEmpty(true))
        {
            Warning(pDoc_->GetCurrentLocation(),
                "In a definition file, blocks cannot contain any data. Ignored."
            );
        }

        pCurrentParentBlock_ = pCurrentBlock_->GetParent();
        if (pCurrentParentBlock_)
        {
            pCurrentBlock_ = pCurrentParentBlock_;
        }
        else
        {
            // It's either the main block's end tag or a
            // pre-defined block's end tag.
            // Anyway, there is nothing to do.
        }
    }
    else
    {
        throw Exception(pDoc_->GetCurrentLocation(),
            "Wrong end tag : \"</"+sName+">\". Expected : \"</"+pCurrentBlock_->GetName()+">\"."
        );
    }
}

void Document::DefState::ReadOpeningTag( const s_str& sTagContent )
{
    s_ctnr<s_str> lWords = sTagContent.Cut(" ", 1);
    s_str sName = lWords.Front();

    s_uint uiMin = 0;
    s_uint uiMax = s_uint::INF;
    s_bool bPreDefining = false;
    s_bool bCopy = false;
    s_uint uiRadioGroup = s_uint::NaN;
    s_bool bLoad = false;
    s_str sParent;

    ReadPreDefCommands_(
        sName, sParent, uiMin, uiMax, bCopy, bPreDefining, bLoad, uiRadioGroup, true
    );

    if (uiRadioGroup.IsValid() && sName == ".")
    {
        pCurrentParentBlock_->SetRadioGroupOptional(uiRadioGroup);
        return;
    }

    s_ctnr<s_str> lAttributes;
    if (lWords.GetSize() > 1)
    {
        s_str sAttributes = lWords.Back();
        sAttributes.Replace(" =", "=");
        sAttributes.Replace("= ", "=");
        lAttributes = sAttributes.Cut(" ");
    }

    if (pCurrentParentBlock_)
    {
        if (bCopy)
        {
            if (uiRadioGroup.IsValid())
                pCurrentBlock_ = pCurrentParentBlock_->CreateRadioDefBlock(sName, uiRadioGroup);
            else
                pCurrentBlock_ = pCurrentParentBlock_->CreateDefBlock(sName, uiMin, uiMax);

            // Copy
            if (!sParent.IsEmpty())
            {
                // Inheritance
                if (pDoc_->GetPredefinedBlock(sParent))
                {
                    pCurrentBlock_->Copy(pDoc_->GetPredefinedBlock(sParent));
                }
                else
                {
                    throw Exception(pDoc_->GetCurrentLocation(),
                        "\""+sParent+"\" has not (yet?) been pre-defined and cannot be "
                        "copied."
                    );
                }
            }

            pCurrentParentBlock_ = pCurrentBlock_;
        }
        else if (!pDoc_->GetPredefinedBlock(sName))
        {
            if (uiRadioGroup.IsValid())
                pCurrentBlock_ = pCurrentParentBlock_->CreateRadioDefBlock(sName, uiRadioGroup);
            else
                pCurrentBlock_ = pCurrentParentBlock_->CreateDefBlock(sName, uiMin, uiMax);

            pCurrentBlock_->CheckAttributesDef(lAttributes);
            pCurrentParentBlock_ = pCurrentBlock_;
        }
        else
        {
            throw Exception(pDoc_->GetCurrentLocation(),
                "Defining a new block named \""+sName+"\", which is the name "
                "of a pre-defined block."
            );
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
                if (pDoc_->GetPredefinedBlock(sParent))
                {
                    pDoc_->CreatePredefinedBlock(sName, sParent);
                    if (!bCopy)
                        pDoc_->GetPredefinedBlock(sParent)->AddDerivated(sName);
                }
                else
                {
                    throw Exception(pDoc_->GetCurrentLocation(),
                        "\""+sParent+"\" has not (yet?) been pre-defined and cannot be "+
                        (bCopy ? s_str("copied.") : s_str("inherited."))
                    );
                }
            }
            else
                pDoc_->CreatePredefinedBlock(sName);

            pCurrentBlock_ = pDoc_->GetPredefinedBlock(sName);
            pCurrentBlock_->SetDocument(pDoc_);
        }
        else
        {
            // Main block
            pCurrentBlock_ = pDoc_->GetMainBlock();
        }

        pCurrentBlock_->SetName(sName);

        pCurrentBlock_->CheckAttributesDef(lAttributes);

        pCurrentParentBlock_ = pCurrentBlock_;
    }
}
