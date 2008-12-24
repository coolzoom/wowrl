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
const s_str Block::CLASS_NAME     = "Block";

Attribute::Attribute()
{
}

Attribute::Attribute(const s_str& name, const s_bool& optional, const s_str& def) :
    sName(name), sDefault(def), bOptional(optional)
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
        bNamedIteration_ = false;
        if (!lFoundBlockStack_.empty())
        {
            mCurrIter_ = lFoundBlockStack_.begin();
            mEndIter_ = lFoundBlockStack_.end();
            return *mCurrIter_;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        bNamedIteration_ = true;
        if (MAPFIND(sName, lFoundBlockList_))
        {
            pair< multimap<s_str, Block>::iterator,
                  multimap<s_str, Block>::iterator > mIterPair;
            mIterPair = lFoundBlockList_.equal_range(sName);
            mCurrNIter_ = mIterPair.first;
            mEndNIter_ = mIterPair.second;
            return &mCurrNIter_->second;
        }
        else
        {
            return NULL;
        }
    }
}

s_ptr<Block> Block::Next()
{
    if (bNamedIteration_)
    {
        if (mCurrNIter_ != mEndNIter_)
        {
            mCurrNIter_++;
            if (mCurrNIter_ == mEndNIter_)
                return NULL;
            else
                return &mCurrNIter_->second;
        }
        else
            return NULL;
    }
    else
    {
        if (mCurrIter_ != mEndIter_)
        {
            mCurrIter_++;
            if (mCurrIter_ == mEndIter_)
                return NULL;
            else
                return *mCurrIter_;
        }
        else
            return NULL;
    }
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

s_ptr<Block> Block::GetRadioBlock()
{
    if (bRadioChilds_)
        return &lFoundBlockList_.begin()->second;
    else
        return NULL;
}

s_bool Block::HasBlock( const s_str& sName )
{
    return (MAPFIND(sName, lDefBlockList_) || MAPFIND(sName, lPreDefBlockList_));
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
            pNewBlock_ = s_refptr<Block>(new Block(*(lPreDefBlockList_[sName].pBlock)));
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
        multimap<s_str, Block>::iterator iterAdded;
        iterAdded = lFoundBlockList_.insert(make_pair(pNewBlock_->GetName(), *pNewBlock_));
        lFoundBlockStack_.push_back(&iterAdded->second);
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
        if (!bRadioChilds_ && !GetDefChildNumber().IsNull())
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
        if (!bRadioChilds_ && !GetDefChildNumber().IsNull())
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
