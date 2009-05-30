/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Block source              */
/*                                        */
/*                                        */

#include "xml/frost_xml_block.h"
#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::XML;

const s_str Attribute::CLASS_NAME = "Attribute";
const s_str Block::CLASS_NAME     = "Block";

Attribute::Attribute()
{
}

Attribute::Attribute(const s_str& name, const s_bool& optional, const s_str& def, const AttrType& type) :
    sName(name), sDefault(def), bOptional(optional), mType(type)
{
}

PredefinedBlock::PredefinedBlock()
{
}

PredefinedBlock::PredefinedBlock(s_ptr<Block> block, const s_uint& min, const s_uint& max, const s_bool& radio) :
    pBlock(block), uiMin(min), uiMax(max), bRadio(radio)
{
}

Block::Block()
{
}

Block::Block( const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr, const s_bool& bRadio ) :
    sName_(sName), uiMaxNumber_(uiMaxNbr), uiMinNumber_(uiMinNbr), bRadio_(bRadio)
{
}

Block::Block( const Block& mValue )
{
    sName_ = mValue.sName_;
    uiMaxNumber_ = mValue.uiMaxNumber_;
    uiMinNumber_ = mValue.uiMinNumber_;
    bRadio_ = mValue.bRadio_;
    bRadioChilds_ = mValue.bRadioChilds_;
    sValue_ = mValue.sValue_;
    pDoc_ = mValue.pDoc_;
    pParent_ = mValue.pParent_;
    bCreating_ = mValue.bCreating_;

    lDerivatedList_ = mValue.lDerivatedList_;

    lAttributeList_ = mValue.lAttributeList_;
    lDefBlockList_ = mValue.lDefBlockList_;
    lPreDefBlockList_ = mValue.lPreDefBlockList_;

    multimap< s_str, s_ptr<Block> >::const_iterator iterBlock;
    multimap< s_str, s_ptr<Block> >::iterator iterAdded;
    foreach (iterBlock, mValue.lFoundBlockList_)
    {
        iterAdded = lFoundBlockList_.insert(make_pair(iterBlock->first, new Block(*(iterBlock->second))));
        lFoundBlockStack_.push_back(iterAdded);
        lFoundBlockSortedStacks_[iterBlock->first].push_back(iterAdded);
    }
}

Block::~Block()
{
    multimap< s_str, s_ptr<Block> >::iterator iterBlock;
    foreach (iterBlock, lFoundBlockList_)
    {
        iterBlock->second.Delete();
    }
}

s_bool Block::Add( const Attribute& mAttrib )
{
    if (!MAPFIND(mAttrib.sName, lAttributeList_))
    {
        lAttributeList_[mAttrib.sName] = mAttrib;
        return true;
    }
    else
    {
        Warning(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "Redefining \""+mAttrib.sName+"\" attribute (check inheritance). Skipped."
        );
        return false;
    }
}

s_bool Block::CheckAttributes( const s_str& sAttributes )
{
    if (!sAttributes.IsEmpty())
    {
        vector<s_str> lAttribs;
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

        vector<s_str>::iterator iterAttr;
        foreach (iterAttr, lAttribs)
        {
            if (iterAttr->Find("="))
            {
                vector<s_str> lWords = iterAttr->Cut("=");
                s_str sAttrName = lWords.front();
                sAttrName.Trim(' ');

                s_str sAttrValue = lWords.back();
                sAttrValue.Trim(' ');
                sAttrValue.Trim('"');

                if (MAPFIND(sAttrName, lAttributeList_))
                {
                    s_ptr<Attribute> pAttr = &lAttributeList_[sAttrName];
                    if (pAttr->mType == ATTR_TYPE_BOOL)
                    {
                        if (!sAttrValue.IsBoolean())
                        {
                            Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                                "Attribute \""+sAttrName+"\" has wrong type (boolean expected)."
                            );
                            return false;
                        }
                    }
                    else if (pAttr->mType == ATTR_TYPE_NUMBER)
                    {
                        if (!sAttrValue.IsNumber())
                        {
                            Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
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
                    Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                        "Unknown attribute : \""+sAttrName+"\"."
                    );
                    Log("Listing available possibilities :");
                    map<s_str, Attribute>::iterator iterAttr;
                    foreach (iterAttr, lAttributeList_)
                        Log("    "+iterAttr->first);
                    return false;
                }
            }
            else
            {
                Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                    "Wrong synthax for attributes (missing '=')."
                );
                return false;
            }
        }
    }

    s_bool bGood = true;
    map<s_str, Attribute>::iterator iterAttr2;
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
                Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
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
    if ( bRadioChilds_ && (lFoundBlockList_.size() == 0) )
    {
        Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "This block is meant to contain one radio child, but doesn't contain any."
        );
        return false;
    }
    if ( bRadioChilds_ && (lFoundBlockList_.size() > 1) )
    {
        Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "This block is meant to contain one radio child, but contains several ones."
        );
        return false;
    }

    map<s_str, Block>::iterator iterDefBlock;
    foreach (iterDefBlock, lDefBlockList_)
    {
        s_uint uiCount = lFoundBlockList_.count(iterDefBlock->first);
        if (uiCount < iterDefBlock->second.GetMinCount())
        {
            Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "Too few \"<"+iterDefBlock->first+">\" blocks (expected : at least "+iterDefBlock->second.GetMinCount()+")."
            );
            return false;
        }
        else if (uiCount > iterDefBlock->second.GetMaxCount())
        {
            Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "Too many \"<"+iterDefBlock->first+">\" blocks (expected : at most "+iterDefBlock->second.GetMaxCount()+")."
            );
            return false;
        }
    }

    map<s_str, PredefinedBlock>::iterator iterPreDefBlock;
    foreach (iterPreDefBlock, lPreDefBlockList_)
    {
        s_uint uiCount = lFoundBlockList_.count(iterPreDefBlock->first);
        if (uiCount < iterPreDefBlock->second.uiMin)
        {
            Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "Too few \"<"+iterPreDefBlock->first+">\" blocks (expected : at least "+iterPreDefBlock->second.uiMin+")."
            );
            return false;
        }
        else if (uiCount > iterPreDefBlock->second.uiMax)
        {
            Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
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
    lDerivatedList_.push_back(sName);
}

s_bool Block::HasDerivated( const s_str& sName )
{
    if (VECTORFIND(sName, lDerivatedList_))
    {
        return true;
    }
    else
    {
        vector<s_str>::iterator iter;
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

void Block::SetRadio()
{
    uiMinNumber_ = 0;
    uiMaxNumber_ = 1;
    bRadio_ = true;
}

const s_bool& Block::IsRadio() const
{
    return bRadio_;
}

const s_bool& Block::HasRadioChilds() const
{
    return bRadioChilds_;
}

s_uint Block::GetDefChildNumber() const
{
    return lDefBlockList_.size() + lPreDefBlockList_.size();
}

s_uint Block::GetChildNumber() const
{
    return lFoundBlockList_.size();
}

s_ptr<Block> Block::First( const s_str& sName )
{
    if (sName.IsEmpty())
    {
        if (!lFoundBlockStack_.empty())
        {
            mCurrIter_ = lFoundBlockStack_.begin();
            mEndIter_ = lFoundBlockStack_.end();
            return (*mCurrIter_)->second;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if (MAPFIND(sName, lFoundBlockSortedStacks_))
        {
            mCurrIter_ = lFoundBlockSortedStacks_[sName].begin();
            mEndIter_ = lFoundBlockSortedStacks_[sName].end();
            return (*mCurrIter_)->second;
        }
        else
        {
            return NULL;
        }
    }
}

s_ptr<Block> Block::Next()
{
    if (mCurrIter_ != mEndIter_)
    {
        mCurrIter_++;
        if (mCurrIter_ == mEndIter_)
            return NULL;
        else
            return (*mCurrIter_)->second;
    }
    else
        return NULL;
}

s_str Block::GetAttribute( const s_str& sName )
{
    if (MAPFIND(sName, lAttributeList_))
    {
        return lAttributeList_[sName].sValue;
    }
    else
    {
        Error(CLASS_NAME + "("+sName_+")", "Attribute \""+sName+"\" doesn't exist.");
        Log("List :");
        map<s_str, Attribute>::iterator iterAttr;
        foreach (iterAttr, lAttributeList_)
            Log("    "+iterAttr->first);
        return "";
    }
}

s_ptr<Block> Block::GetBlock( const s_str& sName )
{
    if (MAPFIND(sName, lFoundBlockList_))
    {
        return lFoundBlockList_.find(sName)->second;
    }
    else
    {
        return NULL;
    }
}

s_ptr<Block> Block::GetRadioBlock()
{
    if (bRadioChilds_)
        return lFoundBlockList_.begin()->second;
    else
        return NULL;
}

s_bool Block::HasBlock( const s_str& sName )
{
    if (MAPFIND(sName, lDefBlockList_) || MAPFIND(sName, lPreDefBlockList_))
    {
        return true;
    }
    else
    {
        s_ptr<XML::Block> pGlobal;
        map<s_str, PredefinedBlock>::iterator iterBlock;
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
        if (MAPFIND(sName, lDefBlockList_))
        {
            pNewBlock_ = s_refptr<Block>(new Block(lDefBlockList_[sName]));
        }
        else
        {
            pNewBlock_ = s_refptr<Block>(new Block(*pDoc_->GetPredefinedBlock(sName)));
        }
        pNewBlock_->SetParent(this);
        pNewBlock_->SetDocument(pDoc_);
        bCreating_ = true;
        return s_ptr<Block>(pNewBlock_.Get());
    }
    else
    {
        Error(CLASS_NAME, "Already creating a block.");
        return NULL;
    }
}

void Block::AddBlock()
{
    if (bCreating_)
    {
        multimap< s_str, s_ptr<Block> >::iterator iterAdded;
        // Store the new block
        iterAdded = lFoundBlockList_.insert(make_pair(pNewBlock_->GetName(), new Block(*pNewBlock_)));
        // Position it on the global stack
        lFoundBlockStack_.push_back(iterAdded);
        // Position it on the sorted stack
        lFoundBlockSortedStacks_[pNewBlock_->GetName()].push_back(iterAdded);

        pNewBlock_.SetNull();
        bCreating_ = false;
    }
}

s_ptr<Block> Block::CreateDefBlock( const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr )
{
    if (!HasBlock(sName))
    {
        if (bRadioChilds_)
        {
            Warning(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "A neighbour has been declared as a radio block, but \""+sName+"\" isn't. "
                "I'll be marked as radio block as well."
            );
            lDefBlockList_[sName] = Block(sName, 0, 1, true);
        }
        else
            lDefBlockList_[sName] = Block(sName, uiMinNbr, uiMaxNbr);
        s_ptr<Block> pBlock = &lDefBlockList_[sName];
        pBlock->SetParent(this);
        pBlock->SetDocument(pDoc_);
        return pBlock;
    }
    else
    {
        Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "There is already a \""+sName+"\" block defined."
        );

        return NULL;
    }
}

s_ptr<Block> Block::CreateRadioDefBlock(const s_str& sName)
{
    if (!HasBlock(sName))
    {
        if ( !bRadioChilds_ && (GetDefChildNumber() != 0) )
        {
            Warning(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "\""+sName+"\" has been declared as a radio block, but the previous ones weren't. "
                "All previously defined childs will be marked as radio blocks."
            );

            map<s_str, PredefinedBlock>::iterator iterPreDef;
            foreach (iterPreDef, lPreDefBlockList_)
            {
                iterPreDef->second.bRadio = true;
                iterPreDef->second.uiMin = 0;
                iterPreDef->second.uiMax = 1;
            }

            map<s_str, Block>::iterator iterDef;
            foreach (iterDef, lDefBlockList_)
            {
                iterDef->second.SetRadio();
            }
        }

        bRadioChilds_ = true;
        lDefBlockList_[sName] = Block(sName, 0, 1, true);
        s_ptr<Block> pBlock = &lDefBlockList_[sName];
        pBlock->SetParent(this);
        pBlock->SetDocument(pDoc_);
        return pBlock;
    }
    else
    {
        Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "There is already a \""+sName+"\" block defined."
        );

        return NULL;
    }
}


s_ptr<PredefinedBlock> Block::AddPredefinedBlock( s_ptr<Block> pBlock, const s_uint& uiMinNbr, const s_uint& uiMaxNbr )
{
    if (!HasBlock(pBlock->GetName()))
    {
        if (bRadioChilds_)
        {
            Warning(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "A neighbour has been declared as a radio block, but \""+pBlock->GetName()+"\" isn't. "
                "I'll be marked as radio block as well."
            );
            lPreDefBlockList_[pBlock->GetName()] = PredefinedBlock(pBlock, 0, 1, true);
        }
        else
            lPreDefBlockList_[pBlock->GetName()] = PredefinedBlock(pBlock, uiMinNbr, uiMaxNbr);

        return &lPreDefBlockList_[pBlock->GetName()];
    }
    else
    {
        Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "There is already a \""+pBlock->GetName()+"\" block defined."
        );

        return NULL;
    }
}

s_ptr<PredefinedBlock> Block::AddPredefinedRadioBlock( s_ptr<Block> pBlock )
{
    if (!HasBlock(pBlock->GetName()))
    {
        if ( !bRadioChilds_ && (GetDefChildNumber() != 0) )
        {
            Warning(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                "\""+pBlock->GetName()+"\" has been declared as a radio block, but the previous ones weren't. "
                "All previously defined childs will be marked as radio blocks."
            );

            map<s_str, PredefinedBlock>::iterator iterPreDef;
            foreach (iterPreDef, lPreDefBlockList_)
            {
                iterPreDef->second.bRadio = true;
                iterPreDef->second.uiMin = 0;
                iterPreDef->second.uiMax = 1;
            }

            map<s_str, Block>::iterator iterDef;
            foreach (iterDef, lDefBlockList_)
            {
                iterDef->second.SetRadio();
            }
        }

        bRadioChilds_ = true;
        lPreDefBlockList_[pBlock->GetName()] = PredefinedBlock(pBlock, 0, 1, true);

        return &lPreDefBlockList_[pBlock->GetName()];
    }
    else
    {
        Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
            "There is already a \""+pBlock->GetName()+"\" block defined."
        );

        return NULL;
    }
}
