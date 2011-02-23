/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            XML::Block source           */
/*                                        */
/*                                        */

#include "frost_xml_block.h"
#include "frost_xml_document.h"

#include <frost_utils_exception.h>
#include <frost_utils_log.h>
#include <frost_utils_stdhelper.h>

using namespace std;
using namespace Frost;
using namespace Frost::XML;

const s_str Attribute::CLASS_NAME = "XML::Attribute";
const s_str Block::CLASS_NAME     = "XML::Block";

Attribute::Attribute()
{
}

Attribute::Attribute(const s_str& name, const s_bool& optional, const s_str& def, const AttrType& type) :
    sName(name), sDefault(def), bOptional(optional), mType(type)
{
}

PredefinedBlock::PredefinedBlock() : uiRadioGroup(s_uint::NaN)
{
}

PredefinedBlock::PredefinedBlock(s_ptr<Block> block, const s_uint& min, const s_uint& max, const s_uint& radio_group) :
    pBlock(block), uiMin(min), uiMax(max), uiRadioGroup(radio_group)
{
}

Block::Block() : uiRadioGroup_(s_uint::NaN)
{
}

Block::Block( const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr, const s_str& sFile, const s_uint& uiLineNbr, const s_uint& uiRadioGroup ) :
    sName_(sName), uiMaxNumber_(uiMaxNbr), uiMinNumber_(uiMinNbr), uiRadioGroup_(uiRadioGroup), sFile_(sFile), uiLineNbr_(uiLineNbr)
{
}

Block::~Block()
{
    pNewBlock_.Delete();

    s_multimap< s_str, s_ptr<Block> >::iterator iterBlock;
    foreach (iterBlock, lFoundBlockList_)
    {
        iterBlock->second.Delete();
    }
}

void Block::Copy( s_ptr<Block> pBlock )
{
    if (pBlock)
    {
        bRadioChilds_ = pBlock->bRadioChilds_;
        sValue_ = pBlock->sValue_;

        lRadioBlockList_ = pBlock->lRadioBlockList_;

        lAttributeList_ = pBlock->lAttributeList_;
        lDefBlockList_ = pBlock->lDefBlockList_;
        lPreDefBlockList_ = pBlock->lPreDefBlockList_;
    }
}

s_bool Block::Add( const Attribute& mAttrib )
{
    lAttributeList_[mAttrib.sName] = mAttrib;
    return true;
}

void Block::RemoveAttribute( const s_str& sAttributeName )
{
    lAttributeList_.Erase(sAttributeName);
}

void Block::CheckAttributesDef( const s_ctnr<s_str>& lAttribs )
{
    s_ctnr<s_str>::const_iterator iterAttr;
    foreach (iterAttr, lAttribs)
    {
        s_str sAttr = *iterAttr;
        s_str sDefault;
        s_bool bOptional = false;
        if (sAttr.Find("="))
        {
            bOptional = true;
            s_ctnr<s_str> lCut = sAttr.Cut("=");
            sAttr = lCut.Front();
            sDefault = lCut.Back();
            sDefault.Trim('"');
        }

        AttrType mType = ATTR_TYPE_STRING;
        s_ctnr<s_str> lCommands = sAttr.Cut(":");
        sAttr = lCommands.Back();
        lCommands.PopBack();
        s_ctnr<s_str>::iterator iterCommand;
        s_bool bAdd = true;
        foreach (iterCommand, lCommands)
        {
            char cLetterCode = (*iterCommand)[0];
            if (cLetterCode == 's')
            {
                mType = ATTR_TYPE_STRING;
            }
            else if (cLetterCode == 'n')
            {
                mType = ATTR_TYPE_NUMBER;
            }
            else if (cLetterCode == 'b')
            {
                mType = ATTR_TYPE_BOOL;
            }
            else if (cLetterCode == '-')
            {
                RemoveAttribute(sAttr);
                bAdd = false;
            }
            else
            {
                Warning(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                    "Unknown command : \'"+(*iterCommand)+"\'. Skipped."
                );
            }
        }

        if (bAdd)
            Add(Attribute(sAttr, bOptional, sDefault, mType));
    }
}

s_bool Block::CheckAttributes( const s_str& sAttributes )
{
    if (!sAttributes.IsEmpty())
    {
        s_ctnr<s_str> lAttribs;
        s_str::const_iterator iterStr;
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
                        lAttribs.PushBack(sAttr);
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
            lAttribs.PushBack(sAttr);

        s_ctnr<s_str>::iterator iterAttr;
        foreach (iterAttr, lAttribs)
        {
            if (iterAttr->Find("="))
            {
                s_ctnr<s_str> lWords = iterAttr->Cut("=");
                s_str sAttrName = lWords.Front();
                sAttrName.Trim(' ');

                s_str sAttrValue = lWords.Back();
                sAttrValue.Trim(' ');
                sAttrValue.Trim('"');

                if (lAttributeList_.Find(sAttrName))
                {
                    s_ptr<Attribute> pAttr = &lAttributeList_[sAttrName];
                    if (pAttr->mType == ATTR_TYPE_BOOL)
                    {
                        if (!sAttrValue.IsBoolean())
                        {
                            Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                                "Attribute \""+sAttrName+"\" has wrong type (boolean expected)."
                            );
                            return false;
                        }
                    }
                    else if (pAttr->mType == ATTR_TYPE_NUMBER)
                    {
                        if (!sAttrValue.IsNumber())
                        {
                            Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                                "Attribute \""+sAttrName+"\" has wrong type (number expected)."
                            );
                            return false;
                        }
                    }

                    pAttr->bFound = true;
                    pAttr->sValue = sAttrValue;
                }
                else
                {
                    Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                        "Unknown attribute : \""+sAttrName+"\"."
                    );
                    Log("Listing available possibilities :");
                    s_map<s_str, Attribute>::iterator iterAttr;
                    foreach (iterAttr, lAttributeList_)
                        Log("    "+iterAttr->first);
                    return false;
                }
            }
            else
            {
                Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                    "Wrong synthax for attributes (missing '=')."
                );
                return false;
            }
        }
    }

    s_bool bGood = true;
    s_map<s_str, Attribute>::iterator iterAttr2;
    foreach (iterAttr2, lAttributeList_)
    {
        if (!iterAttr2->second.bFound)
        {
            if (iterAttr2->second.bOptional)
            {
                iterAttr2->second.sValue = iterAttr2->second.sDefault;
            }
            else
            {
                Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                    "Missing \""+iterAttr2->second.sName+"\" attribute."
                );
                bGood = false;
            }
        }
    }

    return bGood;
}

s_bool Block::CheckBlocks()
{
    if (bRadioChilds_)
    {
        s_map<s_str, Block>::iterator iterDefBlock;
        foreach (iterDefBlock, lDefBlockList_)
        {
            s_uint uiCount = lFoundBlockList_.Count(iterDefBlock->first);
            s_uint uiGroup = iterDefBlock->second.GetRadioGroup();
            if (uiGroup.IsValid())
            {
                if (uiCount == 1)
                {
                    if (!lRadioBlockList_[uiGroup])
                    {
                        lRadioBlockList_[uiGroup] = lFoundBlockList_.Get(iterDefBlock->first)->second;
                    }
                    else
                    {
                        Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                            "\"<"+iterDefBlock->first+">\" is part of a radio group with "
                            "\"<"+lRadioBlockList_[uiGroup]->GetName()+">\", which has been found first."
                        );
                        return false;
                    }
                }
                else if (uiCount > 1)
                {
                    Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                        "\"<"+iterDefBlock->first+">\" is part of a radio group but has been found "
                        "several times."
                    );
                    return false;
                }
            }
        }

        s_map<s_str, PredefinedBlock>::iterator iterPreDefBlock;
        foreach (iterPreDefBlock, lPreDefBlockList_)
        {
            s_uint uiCount = lFoundBlockList_.Count(iterPreDefBlock->first);
            s_uint uiGroup = iterPreDefBlock->second.uiRadioGroup;
            if (uiGroup.IsValid())
            {
                if (uiCount == 1)
                {
                    if (!lRadioBlockList_[uiGroup])
                    {
                        lRadioBlockList_[uiGroup] = lFoundBlockList_.Get(iterPreDefBlock->first)->second;
                    }
                    else
                    {
                        Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                            "\"<"+iterPreDefBlock->first+">\" is part of a radio group with "
                            "\"<"+lRadioBlockList_[uiGroup]->GetName()+">\", which has been found first."
                        );
                        return false;
                    }
                }
                else
                {
                    Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                        "\"<"+iterPreDefBlock->first+">\" is part of a radio group but has been found "
                        "several times."
                    );
                    return false;
                }
            }
        }

        s_map< s_uint, s_ptr<Block> >::iterator iterRadio;
        foreach (iterRadio, lRadioBlockList_)
        {
            if (!iterRadio->second && !lOptionalRadioGroupList_.Find(iterRadio->first))
            {
                Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                    "No block found for radio group "+iterRadio->first+"."
                );
                return false;
            }
        }
    }

    s_map<s_str, Block>::iterator iterDefBlock;
    foreach (iterDefBlock, lDefBlockList_)
    {
        s_uint uiCount = lFoundBlockList_.Count(iterDefBlock->first);
        if (uiCount < iterDefBlock->second.GetMinCount())
        {
            Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                "Too few \"<"+iterDefBlock->first+">\" blocks (expected : at least "+iterDefBlock->second.GetMinCount()+")."
            );
            return false;
        }
        else if (uiCount > iterDefBlock->second.GetMaxCount())
        {
            Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                "Too many \"<"+iterDefBlock->first+">\" blocks (expected : at most "+iterDefBlock->second.GetMaxCount()+")."
            );
            return false;
        }
    }

    s_map<s_str, PredefinedBlock>::iterator iterPreDefBlock;
    foreach (iterPreDefBlock, lPreDefBlockList_)
    {
        s_uint uiCount = lFoundBlockList_.Count(iterPreDefBlock->first);
        if (uiCount < iterPreDefBlock->second.uiMin)
        {
            Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                "Too few \"<"+iterPreDefBlock->first+">\" blocks (expected : at least "+iterPreDefBlock->second.uiMin+")."
            );
            return false;
        }
        else if (uiCount > iterPreDefBlock->second.uiMax)
        {
            Error(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
                "Too many \"<"+iterPreDefBlock->first+">\" blocks (expected : at most "+iterPreDefBlock->second.uiMax+")."
            );
            return false;
        }
    }

    return true;
}

const s_str& Block::GetName() const
{
    return sName_;
}

void Block::SetName( const s_str& sName )
{
    sName_ = sName;
}

const s_str& Block::GetValue() const
{
    return sValue_;
}

void Block::SetValue( const s_str& sValue )
{
    sValue_ = sValue;
}

s_ptr<Block> Block::GetParent() const
{
    return pParent_;
}

void Block::SetParent( s_ptr<Block> pParent )
{
    pParent_ = pParent;
}

void Block::AddDerivated( const s_str& sName )
{
    lDerivatedList_.PushBack(sName);
}

s_bool Block::HasDerivated( const s_str& sName ) const
{
    if (lDerivatedList_.Find(sName))
    {
        return true;
    }
    else
    {
        s_ctnr<s_str>::const_iterator iter;
        foreach (iter, lDerivatedList_)
        {
            if (pDoc_->GetPredefinedBlock(*iter)->HasDerivated(sName))
            {
                return true;
            }
        }
    }

    return false;
}

void Block::SetDocument( s_ptr<Document> pDoc )
{
    pDoc_ = pDoc;
}

const s_uint& Block::GetMinCount() const
{
    return uiMinNumber_;
}

const s_uint& Block::GetMaxCount() const
{
    return uiMaxNumber_;
}

s_bool Block::IsRadio() const
{
    return uiRadioGroup_.IsValid();
}

const s_uint& Block::GetRadioGroup() const
{
    return uiRadioGroup_;
}

const s_bool& Block::HasRadioChilds() const
{
    return bRadioChilds_;
}

void Block::SetRadioGroupOptional( const s_uint& uiGroup )
{
    if (!lOptionalRadioGroupList_.Find(uiGroup))
        lOptionalRadioGroupList_.PushBack(uiGroup);
    else
    {
        Warning(sFile_+":"+uiLineNbr_+":"+sName_,
            "Radio group "+uiGroup+" has already been flagged as optional. Ignoring."
        );
    }
}

s_uint Block::GetDefChildNumber() const
{
    return lDefBlockList_.GetSize() + lPreDefBlockList_.GetSize();
}

s_uint Block::GetChildNumber() const
{
    return lFoundBlockList_.GetSize();
}

s_uint Block::GetChildNumber( const s_str& sName ) const
{
    return lFoundBlockList_.Count(sName);
}

s_ptr<Block> Block::First( const s_str& sName )
{
    if (sName.IsEmpty())
    {
        if (!lFoundBlockStack_.IsEmpty())
        {
            mCurrIter_ = lFoundBlockStack_.Begin();
            mEndIter_ = lFoundBlockStack_.End();
            return (*mCurrIter_)->second;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        if (lFoundBlockSortedStacks_.Find(sName))
        {
            mCurrIter_ = lFoundBlockSortedStacks_[sName].Begin();
            mEndIter_ = lFoundBlockSortedStacks_[sName].End();
            return (*mCurrIter_)->second;
        }
        else
        {
            return nullptr;
        }
    }
}

s_ptr<Block> Block::Next()
{
    if (mCurrIter_ != mEndIter_)
    {
        mCurrIter_++;
        if (mCurrIter_ == mEndIter_)
            return nullptr;
        else
            return (*mCurrIter_)->second;
    }
    else
        return nullptr;
}

s_str Block::GetAttribute( const s_str& sName )
{
    if (lAttributeList_.Find(sName))
    {
        return lAttributeList_[sName].sValue;
    }
    else
    {
        Error(CLASS_NAME + "("+sName_+")", "Attribute \""+sName+"\" doesn't exist.");
        Log("List :");
        s_map<s_str, Attribute>::iterator iterAttr;
        foreach (iterAttr, lAttributeList_)
            Log("    "+iterAttr->first);
        return "";
    }
}

s_bool Block::IsProvided( const s_str& sName )
{
    if (lAttributeList_.Find(sName))
    {
        return lAttributeList_[sName].bFound;
    }
    else
    {
        return false;
    }
}

s_ptr<Block> Block::GetBlock( const s_str& sName )
{
    s_multimap< s_str, s_ptr<Block> >::iterator iter = lFoundBlockList_.Get(sName);
    if (iter != lFoundBlockList_.End())
    {
        return iter->second;
    }
    else
    {
        return nullptr;
    }
}

s_ptr<Block> Block::GetRadioBlock( const s_uint& uiGroup )
{
    if (bRadioChilds_)
    {
        if (lRadioBlockList_.Find(uiGroup))
        {
            return lRadioBlockList_[uiGroup];
        }
        else
        {
            Warning(sFile_+":"+uiLineNbr_+":"+sName_, "No block in radio group "+uiGroup+".");
            return nullptr;
        }
    }
    else
    {
        Warning(sFile_+":"+uiLineNbr_+":"+sName_, "This block isn't meant to contain radio childs.");
        return nullptr;
    }
}

s_bool Block::HasBlock( const s_str& sName )
{
    return ( lDefBlockList_.Find(sName) || lPreDefBlockList_.Find(sName) );
}

s_bool Block::CanHaveBlock( const s_str& sName )
{
    if (HasBlock(sName))
    {
        return true;
    }
    else
    {
        s_ptr<const XML::Block> pGlobal;
        s_map<s_str, PredefinedBlock>::iterator iterBlock;
        foreach (iterBlock, lPreDefBlockList_)
        {
            pGlobal = pDoc_->GetPredefinedBlock(iterBlock->first);
            if (pGlobal->HasDerivated(sName))
            {
                return true;
            }
        }
    }

    return false;
}

s_ptr<Block> Block::CreateBlock( const s_str& sName )
{
    if (!bCreating_)
    {
        if (lDefBlockList_.Find(sName))
        {
            pNewBlock_ = new Block(lDefBlockList_[sName]);
        }
        else
        {
            pNewBlock_ = new Block(*pDoc_->GetPredefinedBlock(sName));
        }

        pNewBlock_->SetFile(pDoc_->GetCurrentFileName());
        pNewBlock_->SetLineNbr(pDoc_->GetCurrentLineNbr());
        pNewBlock_->SetParent(this);
        pNewBlock_->SetDocument(pDoc_);
        bCreating_ = true;
        return pNewBlock_;
    }
    else
    {
        Error(CLASS_NAME, "Already creating a block.");
        return nullptr;
    }
}

void Block::AddBlock()
{
    if (bCreating_)
    {
        s_multimap< s_str, s_ptr<Block> >::iterator iterAdded;
        // Store the new block
        iterAdded = lFoundBlockList_.Insert(
            pNewBlock_->GetName(), pNewBlock_
        );
        // Position it on the global stack
        lFoundBlockStack_.PushBack(iterAdded);
        // Position it on the sorted stack
        lFoundBlockSortedStacks_[pNewBlock_->GetName()].PushBack(iterAdded);

        pNewBlock_ = nullptr;
        bCreating_ = false;
    }
}

s_ptr<Block> Block::CreateDefBlock( const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr )
{
    lDefBlockList_[sName] = Block(sName, uiMinNbr, uiMaxNbr, pDoc_->GetCurrentFileName(), pDoc_->GetCurrentLineNbr());
    s_ptr<Block> pBlock = &lDefBlockList_[sName];
    pBlock->SetParent(this);
    pBlock->SetDocument(pDoc_);
    return pBlock;
}

s_ptr<Block> Block::CreateRadioDefBlock( const s_str& sName, const s_uint& uiRadioGroup )
{
    if (!HasBlock(sName))
    {
        bRadioChilds_ = true;
        lRadioBlockList_[uiRadioGroup] = nullptr;
        lDefBlockList_[sName] = Block(sName, 0, 1, pDoc_->GetCurrentFileName(), pDoc_->GetCurrentLineNbr(), uiRadioGroup);
        s_ptr<Block> pBlock = &lDefBlockList_[sName];
        pBlock->SetParent(this);
        pBlock->SetDocument(pDoc_);
        return pBlock;
    }
    else
    {
        throw Exception(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
            "There is already a \""+sName+"\" block defined."
        );
    }
}

s_ptr<PredefinedBlock> Block::AddPredefinedBlock( s_ptr<Block> pBlock, const s_uint& uiMinNbr, const s_uint& uiMaxNbr )
{
    if (!HasBlock(pBlock->GetName()))
    {
        lPreDefBlockList_[pBlock->GetName()] = PredefinedBlock(pBlock, uiMinNbr, uiMaxNbr);

        return &lPreDefBlockList_[pBlock->GetName()];
    }
    else
    {
        throw Exception(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
            "There is already a \""+pBlock->GetName()+"\" block defined."
        );
    }
}

s_ptr<PredefinedBlock> Block::AddPredefinedRadioBlock( s_ptr<Block> pBlock, const s_uint& uiRadioGroup )
{
    if (!HasBlock(pBlock->GetName()))
    {
        bRadioChilds_ = true;
        lRadioBlockList_[uiRadioGroup] = nullptr;
        lPreDefBlockList_[pBlock->GetName()] = PredefinedBlock(pBlock, 0, 1, uiRadioGroup);

        return &lPreDefBlockList_[pBlock->GetName()];
    }
    else
    {
        throw Exception(pDoc_->GetCurrentFileName()+":"+pDoc_->GetCurrentLineNbr()+" : "+sName_,
            "There is already a \""+pBlock->GetName()+"\" block defined."
        );
    }
}

const s_str& Block::GetFile() const
{
    return sFile_;
}

const s_uint& Block::GetLineNbr() const
{
    return uiLineNbr_;
}

void Block::SetFile( const s_str& sFile )
{
    sFile_ = sFile;
}

void Block::SetLineNbr( const s_uint& uiLineNbr )
{
    uiLineNbr_ = uiLineNbr;
}
