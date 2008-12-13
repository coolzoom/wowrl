/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Block source              */
/*                                        */
/*                                        */

#include "frost_xml_block.h"
#include "frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::XML;

const s_str Attribute::CLASS_NAME = "Attribute";
const s_str Block::CLASS_NAME = "Block";

Attribute::Attribute()
{
}

Attribute::Attribute(const s_str& name, const s_bool& optional, const s_str& def) :
    sName(name), sDefault(def), bOptional(optional)
{
}

Block::Block()
{
}

Block::Block( const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr ) :
    sName_(sName), uiMaxNumber_(uiMaxNbr), uiMinNumber_(uiMinNbr)
{
}

void Block::Add( const Attribute& mAttrib )
{
    lAttributeList_[mAttrib.sName] = mAttrib;
}

void Block::Add( const Block& mBlock )
{
    lBlockList_[mBlock.GetName()] = mBlock;
}

s_bool Block::CheckAttributes( const s_str& sAttributes )
{
    if (!sAttributes.IsEmpty())
    {
        vector<s_str> lAttribs = sAttributes.Cut(" ");
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
                sAttrValue.EraseFromStart(1);
                sAttrValue.EraseFromEnd(1);
                if (MAPFIND(sAttrName, lAttributeList_))
                {
                    s_ptr<Attribute> pAttr = &lAttributeList_[sAttrName];
                    pAttr->bFound = true;
                    pAttr->sValue = sAttrValue;
                }
                else
                {
                    Error(pDoc_->GetFileName()+":"+pDoc_->GetLineNbr()+" : "+sName_,
                        "Unknown attribute : \""+sAttrName+"\"."
                    );
                    Log("List :");
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
    map<s_str, Block>::iterator iterDefBlock;
    foreach (iterDefBlock, lBlockList_)
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

s_ptr<Block> Block::First( const s_str& sName )
{
    if (sName.IsEmpty())
    {
        if (!lFoundBlockList_.empty())
        {
            mCurrIter_ = lFoundBlockList_.begin();
            mEndIter_ = lFoundBlockList_.end();
            return &mCurrIter_->second;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if (MAPFIND(sName, lFoundBlockList_))
        {
            pair< multimap<s_str, Block>::iterator,
                  multimap<s_str, Block>::iterator > mIterPair;
            mIterPair = lFoundBlockList_.equal_range(sName);
            mCurrIter_ = mIterPair.first;
            mEndIter_ = mIterPair.second;
            return &mCurrIter_->second;
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
            return &mCurrIter_->second;
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
        Error(CLASS_NAME, "Attribute \""+sName+"\" doesn't exist.");
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
        return &lFoundBlockList_.find(sName)->second;
    }
    else
    {
        return NULL;
    }
}

s_bool Block::HasBlock( const s_str& sName )
{
    return MAPFIND(sName, lBlockList_);
}

s_ptr<Block> Block::CreateBlock( const s_str& sName )
{
    if (!bCreating_)
    {
        pNewBlock_ = s_refptr<Block>(new Block(lBlockList_[sName]));
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
        lFoundBlockList_.insert(make_pair(pNewBlock_->GetName(), *pNewBlock_));
        pNewBlock_.SetNull();
        bCreating_ = false;
    }
}

s_ptr<Block> Block::CreateDefBlock( const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr )
{
    lBlockList_[sName] = Block(sName, uiMinNbr, uiMaxNbr);
    s_ptr<Block> pBlock = &lBlockList_[sName];
    pBlock->SetParent(this);
    pBlock->SetDocument(pDoc_);
    return pBlock;
}
